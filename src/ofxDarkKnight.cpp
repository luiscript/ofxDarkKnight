/*
 Copyright (C) 2018 Luis Fernando Garc’a [http://luiscript.com]
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "ofxDarkKnight.hpp"

ofxDarkKnight::ofxDarkKnight()
{
    
}

ofxDarkKnight::~ofxDarkKnight()
{
    
}

void ofxDarkKnight::setup(unordered_map<string, Module*> * pool)
{
    loadWires = shiftKey = altKey = cmdKey = midiMapMode = drawing = showExplorer = false;
    translation = { 0, 0 };
    resolution = { 1920, 1080 };
	zoom = 1.0;
    modulesPool = *pool;
	moduleId = 1;
    
    for(pair<string, Module*> module : modulesPool )
        poolNames.push_back(module.first);
    
    poolNames.sort();

	float browserWidth = ofGetWidth() * 0.8;
	float browserHeight = ofGetHeight() * 0.8;

	componentsList = new ofxDatGuiScrollView("MODULES", 11);
	componentsList->setWidth(browserWidth / 3);
	componentsList->setHeight(poolNames.size() * 25.0);
	componentsList->setPosition(ofGetWidth() * 0.1 + browserWidth/3, ofGetHeight() * 0.1);
	componentsList->onScrollViewEvent(this, &ofxDarkKnight::onComponentListChange);

    for(auto it = poolNames.begin(); it != poolNames.end(); it++) componentsList->add(*it);

    ofAddListener(ofEvents().mousePressed, this, &ofxDarkKnight::handleMousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &ofxDarkKnight::handleMouseDragged);
    ofAddListener(ofEvents().mouseScrolled, this, &ofxDarkKnight::handleMouseScrolled);
	ofAddListener(ofEvents().mouseReleased, this, &ofxDarkKnight::handleMouseReleased);

	ofAddListener(ofEvents().keyPressed, this, &ofxDarkKnight::handleKeyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxDarkKnight::handleKeyReleased);
	ofAddListener(ofEvents().fileDragEvent, this, &ofxDarkKnight::handleDragEvent);
	
	#ifdef TARGET_OSX
	darkKnightMidiOut.openVirtualPort("ofxDarkKnight");
	#else
	if (darkKnightMidiOut.getNumOutPorts() > 0)
	{
		darkKnightMidiOut.openPort(darkKnightMidiOut.getNumOutPorts()-1);
	}
	#endif

    DarkKnightConfig* config = new DarkKnightConfig;
    config->setupModule("PROJECT", resolution);
    config->gui->setPosition(ofGetWidth() - ofGetWidth()/6, 0);
    config->setModuleMidiMapMode(midiMapMode);
    ofAddListener(config->onResolutionChangeEvent, this, &ofxDarkKnight::onResolutionChange);
	config->setModuleId(getNextModuleId());
	modules.insert({"PROJECT", config});
}


void ofxDarkKnight::update()
{    
    for (auto wire : wires)
        if(wire.inputModule->getModuleEnabled() &&
           wire.outputModule->getModuleEnabled())
            wire.update();
    
    for(pair<string, Module*> module : modules )
        if(module.second->getModuleEnabled())
        {
            module.second->updateModule(translation.x, translation.y, zoom);
            for(auto msg : module.second->outMidiMessages)
            {
                sendMidiMessage(*msg);
            }
            module.second->outMidiMessages.clear();
        }
    
	if (showExplorer) {
	//	assetsList->update();
		//collectionList->update();
		componentsList->update();
	}
}


void ofxDarkKnight::draw()
{
    ofPushMatrix();
    ofTranslate(translation.x, translation.y);
	ofScale(zoom);
    for(auto module : modules )
        if(!module.second->moduleIsChild && module.second->getModuleEnabled())
            module.second->drawModule();

    if(midiMapMode)
    {
        for(auto wire : wires) wire.draw();
        if(drawing) currentWire->drawCurrentWire(pointer);
    }

    ofPopMatrix();
    
	componentsList->setVisible(showExplorer);
	showExplorer ? componentsList->draw() : false;

}

void ofxDarkKnight::toggleList()
{
    showExplorer = !showExplorer;
}

void ofxDarkKnight::toggleMappingMode()
{
    midiMapMode = !midiMapMode;
    
    for(pair<string, Module*> module : modules )
    {
        module.second->toggleMidiMap();
		if (module.second->getModuleHasChild())
		{
			MediaPool* mp = static_cast<MediaPool*>(module.second);
			mp->drawMediaPool();
		}
    }
}

void ofxDarkKnight::onComponentListChange(ofxDatGuiScrollViewEvent e)
{
    addModule(e.target->getName());
    toggleList();
}

void ofxDarkKnight::handleKeyPressed(ofKeyEventArgs& keyboard)
{
	if (keyboard.key == OF_KEY_COMMAND || keyboard.key == OF_KEY_CONTROL) cmdKey = true;
	if (keyboard.key == OF_KEY_SHIFT) shiftKey = true;
	if (keyboard.key == OF_KEY_ALT) altKey = true;

	// toggle show explorer
	if (cmdKey && keyboard.keycode == 257 && !keyboard.isRepeat)
	{
		toggleList();
	}

	// cmd+shift+m  -> Toggle midiMap on all layers
	if (cmdKey && keyboard.keycode == 77 && !keyboard.isRepeat) {
		toggleMappingMode();
	}

	if (cmdKey && keyboard.key == OF_KEY_BACKSPACE)
	{
		deleteFocusedModule();
	}

	//cmd + shift + 's' to save preset
	if (shiftKey && cmdKey && keyboard.key == 's')
	{
		savePreset();
	}

	//cmd + r reset translation and zoom
	if (cmdKey && keyboard.keycode == 82)
	{
		translation = { 0, 0 };
		zoom = 1.0;
	}
}

void ofxDarkKnight::handleKeyReleased(ofKeyEventArgs& keyboard)
{
	if (keyboard.key == OF_KEY_COMMAND || keyboard.key == OF_KEY_CONTROL) cmdKey = false;
	if (keyboard.key == OF_KEY_SHIFT) shiftKey = false;
	if (keyboard.key == OF_KEY_ALT) altKey = false;
}

void ofxDarkKnight::handleMousePressed(ofMouseEventArgs &mouse)
{
	int x = (int)(mouse.x - translation.x) / zoom;
	int y = (int)(mouse.y - translation.y) / zoom;

	if (midiMapMode) checkOutputConnection(x, y, "*");

	if (shiftKey)
	{
		#ifdef _WIN32
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		#endif
		startX = mouse.x;
		startY = mouse.y;
	}
	
    for(pair<string, Module*> module : modules )
    {
        //send mouse arguments to modules with childs (like Media Pool)
        if(module.second->getModuleHasChild())
        {
            MediaPool * mp = static_cast<MediaPool*>(module.second);
            mp->mousePressed(mouse);
        }
    }
    
	//right click?
	if (mouse.button == 2)
	{
		toggleList();
	}

	//middle click
	if (mouse.button == 1)
	{
		toggleMappingMode();
	}

}

void ofxDarkKnight::handleMouseDragged(ofMouseEventArgs & mouse)
{
	if (shiftKey)
	{
		#ifdef _WIN32
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		#endif
		translation.x += mouse.x - startX;
		translation.y += mouse.y - startY;

		startX = mouse.x;
		startY = mouse.y;
	}

    if(drawing)
    {
        pointer.x = (mouse.x - translation.x) / zoom;
        pointer.y = (mouse.y - translation.y) / zoom;
    }
}

void ofxDarkKnight::handleMouseReleased(ofMouseEventArgs & mouse)
{
    int x = (int) (mouse.x - translation.x) / zoom;
    int y = (int) (mouse.y - translation.y) / zoom;

    if( midiMapMode ) checkInputConnection(x, y, "*");
}

void ofxDarkKnight::handleMouseScrolled(ofMouseEventArgs & mouse)
{
    //translation is not fully supported with ofxDarkKnightMapping, be careful.
	if (shiftKey)
	{
		if (zoom >= 0.15)
		{
			zoom += mouse.scrollY * 0.05;
		}
		else
		{
			zoom = 0.15;
		}
	}
}

void ofxDarkKnight::checkOutputConnection(float x, float y, string moduleName)
{
    WireConnection * output;
    WireConnection * input;
    
    for(pair<string, Module*> module : modules )
    {
        if(!module.second->getModuleEnabled()) continue;
        output = module.second->getOutputConnection(x, y);
        
        //true if we click on output connection
        if(output != nullptr && module.second->getModuleEnabled() &&
           (module.second->getName() == moduleName || moduleName == "*" ))
        {
			currentWireConnectionType = output->getConnectionType();
            currentWire = new Wire;
            currentWire->setOutputConnection(output);
            currentWire->setConnectionType(currentWireConnectionType);
            currentWire->outputModule = module.second;
            if(currentWireConnectionType == ConnectionType::DK_FBO)
            {
				output->setFbo(module.second->getFbo());
                currentWire->fbo = module.second->getFbo();
			}
			else if (currentWireConnectionType == ConnectionType::DK_LIGHT)
			{
				output->setLight(module.second->getLight());
				currentWire->light = module.second->getLight();
			}
            pointer.x = x;
            pointer.y = y;
            drawing = true;
            break;
        }
        
        input = module.second->getInputConnection(x, y);
        //true if we click on input node
        if(input != nullptr)
        {
            //loop all the wires to find an input match
            for(auto it = wires.begin(); it != wires.end(); it++)
            {
                //true if the input clicked is on the list
                // we need to disconect the wire and delete it from the list
                if(it->getInput() == input)
                {
                    currentWire = new Wire;
                    currentWire->setConnectionType(input->getConnectionType());
                    currentWire->setOutputConnection(it->getOutput());
                    currentWire->outputModule = it->outputModule;
                    //if the disconected input is an FBO remove reference
                    if (currentWire->getConnectionType() == ConnectionType::DK_FBO)
                    {
						input->setFbo(nullptr);
                        it->inputModule->setFbo(nullptr);
                        currentWire->fbo = it->outputModule->getFbo();
					}
					else if (currentWire->getConnectionType() == ConnectionType::DK_LIGHT)
					{
						input->setLight(nullptr);
						it->inputModule->setLight(nullptr);
						currentWire->light = it->output->getLight();
					}
                    
                    pointer.x = x;
                    pointer.y = y;
                    
                    wires.erase(it);
                    drawing = true;
                    break;
                }
            }
        }
    }
}


void ofxDarkKnight::checkInputConnection(float x, float y, string moduleName)
{
    WireConnection * input;
    
    for(pair<string, Module*> module : modules )
    {
        if(!module.second->getModuleEnabled()) continue;
        input = module.second->getInputConnection(x, y);
        //true if user released the wire on input connection
        if(input != nullptr && currentWire != nullptr &&
           (module.second->getName() == moduleName || moduleName == "*"))
        {
            currentWire->setInputConnection(input);
            currentWire->setInputModule(module.second);
            
            if (currentWire->getConnectionType() == ConnectionType::DK_SLIDER)
            {
                currentWire->slider = static_cast<ofxDatGuiSlider*>(module.second->getInputComponent(x, y));
            }
            else if(currentWire->getConnectionType() == ConnectionType::DK_FBO)
            {
				module.second->getInputConnection(x, y)->setFbo(currentWire->fbo);
                module.second->setFbo(currentWire->fbo);
			}
			else if (currentWire->getConnectionType() == ConnectionType::DK_LIGHT)
			{
				module.second->getInputConnection(x, y)->setLight(currentWire->light);
				module.second->setLight(currentWire->light);
			}
            
            
            wires.push_back(*currentWire);
            drawing = false;
            currentWire = nullptr;
            break;
        }
    }
    
    currentWire = nullptr;
    drawing = false;
}

void ofxDarkKnight::handleDragEvent(ofDragInfo & dragInfo)
{
    bool mediaPoolFounded = false;
    if(dragInfo.files.size() > 0)
    {
        ofFile file(dragInfo.files[0]);
        if(file.exists())
        {
            string fileExtension = ofToUpper(file.getExtension());
            if(fileExtension == "MOV" || fileExtension == "MP4")
            {
                DarkKnightHap * hapPlayer = new DarkKnightHap;
                
                for(pair<string, Module*> module : modules )
                    //this is true only for Media Pool modules
                    if(module.second->getModuleHasChild())
                    {
                        MediaPool * mp = static_cast<MediaPool*>(module.second);
                        mp->addItem(hapPlayer, "thumbnails/terrain.jpg", "video player");
                        mediaPoolFounded = true;
                        hapPlayer->gui->setWidth(ofGetWidth()/6);
                        hapPlayer->loadFile(file.getAbsolutePath());
                        modules.insert({"HAP: " + file.getFileName(), hapPlayer});
                        mp->drawMediaPool();
                        return;
                    }
                
                if(!mediaPoolFounded)
                {
                    MediaPool * newPool = new MediaPool;
                    
                    newPool->setCollectionName("Collection 1");
                    newPool->setupModule("SKETCH POOL 1", resolution, false);
                    newPool->addItem(hapPlayer, "thumbnails/terrain.jpg", "HAP: " + file.getFileName());
                    newPool->init();
                    newPool->gui->setPosition(ofGetMouseX() + 15, ofGetMouseY() + 15);
                    newPool->setModulesReference(&modules);
                    newPool->setTranslationReferences(&translation, &zoom);
                    newPool->setModuleMidiMapMode(midiMapMode);
                    modules.insert({"SKETCH POOL 1", newPool});
                    
                    hapPlayer->gui->setWidth(ofGetWidth()/6);
                    hapPlayer->loadFile(file.getAbsolutePath());
                    hapPlayer->setModuleMidiMapMode(midiMapMode);
                    modules.insert({"HAP: " + file.getFileName(), hapPlayer});
                    newPool->drawMediaPool();

                    return;
                }
            }
        }
    }
}


void ofxDarkKnight::addModuleTest(string moduleName)
{
	cout << moduleName << endl;
}

void ofxDarkKnight::addModule(string moduleName, Module * module)
{
    module->setModuleMidiMapMode(midiMapMode);
	module->setModuleId(getNextModuleId());
    modules.insert({moduleName, module});
}

Module * ofxDarkKnight::addModule(string moduleName)
{
    unordered_map<string, Module*>::iterator it;
    it = modules.find(moduleName);
    if (it == modules.end())
    {
        it = modulesPool.find(moduleName);
        Module * newModule = createModule(it->first);
        if(newModule == nullptr) newModule = it->second;
        
        newModule->setupModule(it->first, resolution);
        newModule->gui->setPosition((ofGetMouseX() - translation.x)/zoom - 100/zoom, (ofGetMouseY() - translation.y)/zoom - 15/zoom);
        newModule->setModuleMidiMapMode(midiMapMode);
		int moduleId = getNextModuleId();
		newModule->setModuleId(moduleId);
		string uniqueModuleName = it->first + "#" + ofToString(newModule->getModuleId());

        if(moduleName == "SCREEN OUTPUT")
        {
            ScreenOutput * so = static_cast<ScreenOutput*>(newModule);
            so->mainWindow = mainWindow;
            modules.insert({uniqueModuleName, so});
        }
        else if(moduleName == "MIDI CONTROL IN")
        {
            DarkKnightMidiControlIn * controller = static_cast<DarkKnightMidiControlIn*>(newModule);
            ofAddListener(controller->sendMidi, this, &ofxDarkKnight::newMidiMessage);
            modules.insert({uniqueModuleName, controller});
        } else if(moduleName == "CONFIG")
        {
            DarkKnightConfig* config = static_cast<DarkKnightConfig*>(newModule);
            ofAddListener(config->onResolutionChangeEvent, this, &ofxDarkKnight::onResolutionChange);
            modules.insert({uniqueModuleName, config});
        }
        else
        {
            modules.insert({uniqueModuleName, newModule});
        }
        if(it->second->getModuleHasChild())
        {
            MediaPool * mp = static_cast<MediaPool*>(it->second);
            mp->setModulesReference(&modules);
            mp->setTranslationReferences(&translation, &zoom);
            int mIndex = 0;
            for(CollectionItem item : mp->collection)
            {
                Module * m = item.canvas;
                if(mIndex > 0) m->disable();
                string childName = it->second->getName() + "/" + m->getName();
                m->setModuleMidiMapMode(midiMapMode);
				m->setModuleId(getNextModuleId());
                modules.insert({childName + ofToString(m->getModuleId()), m});
                mIndex ++;
            }
        }
        return newModule;
    }
    return nullptr;
}

void ofxDarkKnight::deleteModule(string moduleName)
{
    modules.erase(moduleName);
}

//delete wires connected to focused component and then delete the component
void ofxDarkKnight::deleteFocusedModule()
{
    //iterate all the modules to get the focused one
    for(pair<string, Module*> module : modules )
    {
        //focused module
        if(module.second->gui->getFocused())
        {
            //get all the items of the focused component
            vector<ofxDatGuiComponent*> components = module.second->gui->getItems();
            
            //iterate all the components to compare with the wires list
            for(auto component : components)
            {
                //true if component has children (it's a folder)
                if(component->children.size() > 0)
                {
                    //iterate all component's children
                    for(auto childComponent : component->children)
                    {
                        deleteComponentWires(childComponent, module.second->getModuleId());
                    }
                }
                //component doesn't have children
                else {
                    deleteComponentWires(component, module.second->getModuleId());
                }
            }
            vector<Wire>::iterator itw = wires.begin();
            while(itw != wires.end())
            {
                if(module.second == itw->outputModule || module.second == itw->inputModule)
                {
                    wires.erase(itw);
                    itw = wires.begin();
                } else {
                    itw++;
                }
            }
            // now that we deleted all the module's wires procede to unmount and delete the module it self
            module.second->inputs.clear();
            module.second->outputs.clear();
            module.second->gui->deleteItems();
            modules.erase(module.first);
            module.second->unMount();
            break;
        }
    }
}

void ofxDarkKnight::deleteComponentWires(ofxDatGuiComponent * component, int deletedModuleId)
{
    //compare each component with all the stored wires
    vector<Wire>::iterator itw = wires.begin();
    while(itw != wires.end())
    {
        if((component->getName() == itw->getInput()->getName() ||
           component->getName() == itw->getOutput()->getName()) &&
			(itw->outputModule->getModuleId() == deletedModuleId || 
			 itw->inputModule->getModuleId() == deletedModuleId))
        {
            wires.erase(itw);
            itw = wires.begin();
        } else {
            itw++;
        }
    }
}

void ofxDarkKnight::onResolutionChange(ofVec2f & newResolution)
{
    resolution = newResolution;
    for(pair<string, Module*> module : modules )
    {
        module.second->setResolution(newResolution.x, newResolution.y);
        module.second->setup();
    }
}

void ofxDarkKnight::close()
{
    for(pair<string, Module*> module : modules )
    {
        module.second->unMount();
    }
    modules.clear();
}

void ofxDarkKnight::newMidiMessage(ofxMidiMessage & msg)
{
    //send midi message to media pool.
    for(pair<string, Module*> module : modules )
        if(module.second->getModuleHasChild())
        {
            MediaPool * mp = static_cast<MediaPool*>(module.second);
            mp->gotMidiMessage(&msg);
        }
    
    string mapping;
    if(msg.control > 0)
    {
        mapping = ofToString(msg.channel) + "/"
                + ofToString(msg.control);
    } else {
        mapping = ofToString(msg.channel) + "/"
                + ofToString(msg.pitch);

        if(msg.status == MIDI_NOTE_ON)
            for(pair<string, Module*> module : modules )
                if(module.second->getModuleHasChild())
                {
                    MediaPool * mp = static_cast<MediaPool*>(module.second);
                    mp->gotMidiMapping(mapping);
                }
    }
}

void ofxDarkKnight::sendMidiMessage(ofxMidiMessage & msg)
{
    darkKnightMidiOut.sendControlChange(msg.channel, msg.control, msg.value);
}


void ofxDarkKnight::savePreset()
{
    for(pair<string, Module*> module : modules)
    {
        if(module.second->getModuleHasChild())
        {
            MediaPool * mediaPool = static_cast<MediaPool*>(module.second);
            mediaPool->savePreset();
        }
    }
}

Module * ofxDarkKnight::createModule(string name)
{
    if(name == "PREVIEW") return new Preview;
	else if (name == "CONFIG") return new DarkKnightConfig;
	else if (name == "COLOR SHADER") return new ColorShader;
	else if(name == "INVERTER") return new ParamInverter;
    else if(name == "LFO") return new LfoSlider;
    else if(name == "MAPPING TOOLS") return new ofxDarkKnightMapping;
    else if(name == "MIDI CONTROL IN") return new DarkKnightMidiControlIn;
    else if(name == "MIDI CONTROL OUT") return new DarkKnightMidiControlOut;
    else if(name == "MIXER") return new Mixer;
    else if(name == "PERLIN NOISE") return new NoiseSlider;
    else if(name == "OSC CLIENT") return new DarkKnightOscClient;
    else if(name == "TEXTURE SERVER") return new DarkKnightTextureServer;
    else if(name == "TEXTURE CLIENT") return new DarkKnightTextureClient;
    
    return nullptr;
}

int ofxDarkKnight::getNextModuleId()
{
	return moduleId++;
}
//   ofxDarkKnight

void ofxDarkKnight::loadProjectFromXml(ofXml xml)
{
	modules.clear();

	auto project = xml.getChild("project");
	if (!project)
	{
		cout << "Incorrect file format" << endl;
		return;
	}
	auto resolutionObject = project.getChild("resolution");
	ofVec2f resolutionVector =
	{
		(float)resolutionObject.getAttribute("width").getFloatValue(),
		(float)resolutionObject.getAttribute("height").getFloatValue()
	};
	resolution = resolutionVector;

	auto gui = project.getChild("gui");
	translation.x = (float)gui.getAttribute("translationX");
	translation.y = (float)gui.getAttribute("translationY");
	zoom = (float)gui.getAttribute("zoom");

	auto modulesObjects = project.getChild("modules");
	if (!modulesObjects) {
		cout << "This file does not have any modules" << endl;
		return;
	}

	for (auto moduleObject : modulesObjects.getChildren())
	{
		auto name = moduleObject.getAttribute("name");
		string moduleName = name.getValue();
		int tempModuleId = moduleObject.getAttribute("id").getIntValue();
		
		if (!moduleObject.getAttribute("isChild").getBoolValue())
		{
			auto pos = moduleObject.getChild("position");
			float posx = pos.getAttribute("x").getFloatValue();
			float posy = pos.getAttribute("y").getFloatValue();
			string theName = moduleName.substr(0, moduleName.find("#"));
			auto newModule = addModule(theName);
			newModule->gui->setPosition(posx, posy);
		}
	}
}

void ofxDarkKnight::setTranslation(ofVec2f t)
{
	translation = t;
}

void ofxDarkKnight::setZoom(float z)
{
	zoom = z;
}

unordered_map<string, Module*>* ofxDarkKnight::getModulesReference()
{
	return &modules;
}

vector<Wire>* ofxDarkKnight::getWiresReference()
{
	return &wires;
}
