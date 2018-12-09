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
    theme = new ofxDatGuiThemeCharcoal();
    shiftKey = altKey = cmdKey = midiMapMode = drawing = showExplorer = false;
    translation = { 0, 0 };
    resolution = { 1920, 1080 };
    
    modulesPool = *pool;
    const int s = modulesPool.size();
    
    for(pair<string, Module*> module : modulesPool )
        poolNames.push_back(module.first);
    
    poolNames.sort();
    
    componentsList = new ofxDatGuiScrollView("Modules list", 10);
    componentsList->setTheme(theme);
    componentsList->setWidth(800);
    componentsList->setHeight(600);
    componentsList->setPosition(ofGetScreenWidth() - 400, ofGetScreenHeight() - 300);
    
    for(list<string>::iterator it = poolNames.begin(); it != poolNames.end(); it++)
        componentsList->add(*it);
    
    componentsList->onScrollViewEvent(this, &ofxDarkKnight::onComponentListChange);
    ofAddListener(ofEvents().mousePressed, this, &ofxDarkKnight::handleMousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &ofxDarkKnight::handleMouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &ofxDarkKnight::handleMouseReleased);
    ofAddListener(ofEvents().mouseScrolled, this, &ofxDarkKnight::handleMouseScrolled);
    
    ofAddListener(ofEvents().keyPressed, this, &ofxDarkKnight::handleKeyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxDarkKnight::handleKeyReleased);
    ofAddListener(ofEvents().fileDragEvent, this, &ofxDarkKnight::handleDragEvent);
    
    loadProject("mySettings.batmapp");
}


void ofxDarkKnight::update()
{
    for(pair<string, Module*> module : modules )
        if(module.second->getModuleEnabled())
            module.second->updateModule(translation.x, translation.y);

    for (auto wire : wires)
        if(wire.inputModule->getModuleEnabled() && wire.outputModule->getModuleEnabled())
            wire.update();
    
    if (showExplorer) componentsList->update();
}


void ofxDarkKnight::draw()
{

    ofPushMatrix();
    
    ofTranslate(translation.x, translation.y);
    
    for(pair<string, Module*> module : modules )
        if(!module.second->moduleIsChild)
            module.second->drawModule();

    if(midiMapMode)
    {
        for(auto wire : wires) wire.draw();
        if(drawing) currentWire->drawCurrentWire(pointer);
    }

    ofPopMatrix();
    
    if (showExplorer) {
        componentsList->setVisible(true);
        componentsList->draw();
    }
    else componentsList->setVisible(false);

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
    }
}

void ofxDarkKnight::onComponentListChange(ofxDatGuiScrollViewEvent e)
{
    addModule(e.target->getName());
    toggleList();
}

void ofxDarkKnight::handleMousePressed(ofMouseEventArgs &mouse)
{
    WireConnection * output;
    WireConnection * input;
    
    int x = (int) mouse.x - translation.x;
    int y = (int) mouse.y - translation.y;
    
    for(pair<string, Module*> module : modules )
    {
        //send mouse arguments to modules with childs (like Media Pool)
        if(module.second->getModuleHasChild())
        {
            MediaPool * mp = static_cast<MediaPool*>(module.second);
            mp->mousePressed(mouse);
        }
        output = module.second->getOutputConnection(x, y);
        
        //true if we click on output node
        if(output != nullptr)
        {
            currentWire = new Wire;
            currentWire->setOutput(output);
            currentWire->type = "scale";
            currentWire->outputModule = module.second;
            pointer.x = x;
            pointer.y = y;
            drawing = true;
            break;
        }
        
        input = module.second->getInputConnection(x, y);
        //true if we click on input node
        if(input != nullptr)
        {
            for(vector<Wire>::iterator it = wires.begin(); it != wires.end(); it++)
            {
                if(it->getInput() == input)
                {
                    //disconect the wire and delete it from the list
                    currentWire = new Wire;
                    currentWire->type = "scale";
                    currentWire->setOutput(it->getOutput());
                    currentWire->outputModule = it->outputModule;

                    pointer.x = x;
                    pointer.y = y;
                    
                    wires.erase(it);
                    drawing = true;
                    break;
                }
            }
        }
        //check if we clicked on a main output
        else
        {
            output = module.second->getMainOutput(x, y);
            input = module.second->getMainInput(x, y);
            
            //true if user clicked in main output
            if(output != nullptr)
            {
                currentWire = new Wire;
                currentWire->setOutput(output);
                currentWire->type = "fbo";
                currentWire->fbo = module.second->getFbo();
                currentWire->outputModule = module.second;
                
                pointer.x = x;
                pointer.y = y;
                drawing = true;
                break;
            }
            //true if user clicked in main input
            else if(input != nullptr)
            {
                //check if we already have this input on the wires list
                for(vector<Wire>::iterator it = wires.begin(); it != wires.end(); it++)
                {
                    
                    //true if wire is listed
                    if(it->getInput() == input)
                    {
                        //disconect the wire and delete it from the list
                        
                        currentWire = new Wire;
                        currentWire->setOutput(it->getOutput());
                        currentWire->type = "fbo";
                        currentWire->fbo = it->outputModule->getFbo();

                        //currentWire->setInput(it->getInput());
                        currentWire->setInput(nullptr);
                        module.second->setFbo(nullptr);
                        
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
    
    //right click?
    if(mouse.button == 2)
    {
        toggleList();
    }
    
    //middle click
    if(mouse.button == 1)
    {
        toggleMappingMode();
    }
    
}

void ofxDarkKnight::handleMouseDragged(ofMouseEventArgs & mouse)
{
    if(drawing)
    {
        pointer.x = mouse.x - translation.x;
        pointer.y = mouse.y - translation.y;
    }
}

void ofxDarkKnight::handleMouseReleased(ofMouseEventArgs & mouse)
{
    WireConnection * input;
    int x = (int) mouse.x - translation.x;
    int y = (int) mouse.y - translation.y;
    
    for(pair<string, Module*> module : modules )
    {
        input = module.second->getInputConnection(x, y);
        
        //true if user released the wire on input connection
        if(input != nullptr && currentWire != nullptr)
        {
            currentWire->setInput(input);
            currentWire->slider = static_cast<ofxDatGuiSlider*>(module.second->getInputComponent(x, y));
            currentWire->inputModule = module.second;
            wires.push_back(*currentWire);
            drawing = false;
            currentWire = nullptr;
            break;
        }
        
        input = module.second->getMainInput(x,y);
        //true if user released the cable in main input connection
        if(input != nullptr)
        {
            currentWire->setInput(input);
            module.second->setFbo(currentWire->fbo);
            currentWire->inputModule = module.second;
            wires.push_back(*currentWire);
            drawing = false;
            currentWire = nullptr;
            break;
        }
    }
    
    currentWire = nullptr;
    drawing = false;
}

void ofxDarkKnight::handleMouseScrolled(ofMouseEventArgs & mouse)
{
    //translation is not fully supported with ofxDarkKnightMapping, be careful.
    //translation.x += 2*mouse.scrollX;
    //translation.y += 2*mouse.scrollY;
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
            if(fileExtension == "MOV")
            {
                DarkKnightHap * hapPlayer = new DarkKnightHap;
                
                for(pair<string, Module*> module : modules )
                    //this is true only for Media Pool modules
                    if(module.second->getModuleHasChild())
                    {
                        MediaPool * mp = static_cast<MediaPool*>(module.second);
                        mp->addItem(hapPlayer, "thumbnails/terrain.jpg", "video player");
                        mediaPoolFounded = true;
                        
//                        hapPlayer->setupModule("HAP: " + file.getFileName(), theme, resolution, true);
                        hapPlayer->loadFile(file.getAbsolutePath());
                    }
                
                if(!mediaPoolFounded)
                {
                    MediaPool * newPool = new MediaPool;
                    
                    newPool->setCollectionName("Collection 1");
                    newPool->setupModule("SKETCH POOL 1", theme, resolution, false);
                    newPool->addItem(hapPlayer, "thumbnails/terrain.jpg", "HAP: " + file.getFileName());
                    newPool->init();
                    
                    hapPlayer->gui->setTheme(theme);
                    hapPlayer->gui->setWidth(450);
                    hapPlayer->loadFile(file.getAbsolutePath());
                    
                    newPool->gui->setPosition(ofGetMouseX() + 15, ofGetMouseY() + 15);
                    newPool->setModulesReference(&modules);
                    newPool->setTranslationReferences(&translation);
                    
                    modules.insert({"SKETCH POOL 1", newPool});
                    modules.insert({"SKETCH POOL 1/HAP VIDEO PLAYER", hapPlayer});
                }
            }
        }
    }
}

void ofxDarkKnight::addModule(string moduleName, Module * module)
{
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
        
        newModule->setupModule(it->first, theme, resolution);
        newModule->gui->setPosition(ofGetMouseX() + 15, ofGetMouseY() + 15);
        
        if(moduleName == "SCREEN OUTPUT")
        {
            ScreenOutput * so = static_cast<ScreenOutput*>(newModule);
            so->mainWindow = mainWindow;
            modules.insert({it->first + ofToString(ofGetElapsedTimef()), so});
        }
        else if(moduleName == "MIDI CONTROLLER")
        {
            ofxDarkKnightMidi * controller = static_cast<ofxDarkKnightMidi*>(newModule);
            ofAddListener(controller->sendMidi, this, &ofxDarkKnight::newMidiMessage);
            modules.insert({it->first + ofToString(ofGetElapsedTimef()), controller});
        }
        else
        {
            modules.insert({it->first + ofToString(ofGetElapsedTimef()), newModule});
        }
        
        if(it->second->getModuleHasChild())
        {
            MediaPool * mp = static_cast<MediaPool*>(it->second);
            mp->setModulesReference(&modules);
            mp->setTranslationReferences(&translation);
            Module * m = it->second->getChildModule();
            string childName = it->second->getName() + "/" + m->getName();
            modules.insert({childName, m});
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
                        deleteComponentWires(childComponent);
                    }
                }
                //component doesn't have children
                else {
                    deleteComponentWires(component);
                }
            }
            
            vector<Wire>::iterator itw = wires.begin();
            while(itw != wires.end())
            {
                if(module.second->fboOutput == itw->getOutput() || module.second->fboInput == itw->getInput())
                {
                    wires.erase(itw);
                    itw = wires.begin();
                } else {
                    itw++;
                }
                
            }
            
            // now that we deleted all the module's wires procede to unmount and delete the module it self
            modules.erase(module.first);
            module.second->unMount();
            break;
        }
        
        
    }
}

void ofxDarkKnight::deleteComponentWires(ofxDatGuiComponent * component)
{
    //compare each component with all the stored wires
    vector<Wire>::iterator itw = wires.begin();
    while(itw != wires.end())
    {
        if(component->getName() == itw->getInput()->getName() ||
           component->getName() == itw->getOutput()->getName()  )
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
    for(pair<string, Module*> module : modules )
    {
        module.second->setResolution(newResolution.x, newResolution.y);
    }
}

void ofxDarkKnight::handleKeyPressed(ofKeyEventArgs &keyboard)
{
    
    if(keyboard.key == OF_KEY_COMMAND) cmdKey = true;
    if(keyboard.key == OF_KEY_SHIFT) shiftKey = true;
    if(keyboard.key == OF_KEY_ALT) altKey = true;
        

    // toggle show explorer
    if(cmdKey && (keyboard.key == OF_KEY_RETURN))
    {
        toggleList();
    }
    
    // cmd+shift+m || cmd+shift+<  -> Toggle midiMap on all layers
    if(shiftKey && (keyboard.key == 109 || keyboard.key == 77)){
        toggleMappingMode();
    }

    if (keyboard.key == OF_KEY_BACKSPACE)
    {
        deleteFocusedModule();
    }
    
    //cmd + 's' to save the project
    if( cmdKey && keyboard.key == 19)
    {
        saveProject();
    }
    
    //cmd + shift + 's' to save preset
    if( shiftKey && cmdKey && keyboard.key == 19)
    {
        savePreset();
    }
}

void ofxDarkKnight::handleKeyReleased(ofKeyEventArgs &keyboard)
{
    if(keyboard.key == OF_KEY_COMMAND) cmdKey = false;
    if(keyboard.key == OF_KEY_SHIFT) shiftKey = false;
    if(keyboard.key == OF_KEY_ALT) altKey = false;
}

void ofxDarkKnight::close()
{
    for(pair<string, Module*> module : modules )
    {
        module.second->unMount();
    }
    modules.clear();
}

//  comment this when ofxDarkKnightMidi is not included

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

void ofxDarkKnight::saveProject()
{
    xml.clear();
    
    xml.addTag("PROJECT");
    xml.pushTag("PROJECT");
    
    xml.addTag("MODULES");
    xml.pushTag("MODULES");
    int moduleIndex = 0;
    for(pair<string, Module*> module : modules )
    {
        if(!module.second->moduleIsChild)
        {
            xml.addTag("MODULE");
            xml.pushTag("MODULE", moduleIndex);
            string mName = module.second->getName();
            std::transform(mName.begin(),mName.end(),mName.begin(), [](char ch){
                return ch == ' ' ? '_' : ch;
            });
            xml.setValue("NAME", mName, moduleIndex);
            xml.setValue("X", module.second->gui->getPosition().x, moduleIndex);
            xml.setValue("Y", module.second->gui->getPosition().y, moduleIndex);
            
            moduleIndex++;
            if(module.second->getModuleHasChild())
            {
                MediaPool * mediaPool = static_cast<MediaPool*>(module.second);
                xml.addTag("ITEMS");
                xml.pushTag("ITEMS");
                int itemIndex = 0;
                for(CollectionItem item : mediaPool->collection)
                {
                    string sName = item.name;
                    std::transform(sName.begin(), sName.end(), sName.begin(), [](char ch){
                        return ch == ' ' ? '_' : ch;
                    });
                    xml.addTag("ITEM");
                    xml.pushTag("ITEM", itemIndex);
                    xml.setValue("NAME", sName, itemIndex);
                    
                    xml.addTag("PARAMS");
                    xml.pushTag("PARAMS");
                    for(ofxDatGuiComponent * component : item.canvas->params->children)
                    {
                        string compName = component->getName();
                        std::transform(compName.begin(), compName.end(), compName.begin(), [](char ch){
                            return ch == ' ' ? '_' : ch;
                        });
                        xml.setValue(compName, component->getComponentScale(), itemIndex);
                    }
                    itemIndex++;
                    xml.popTag();
                    xml.popTag();
                }
                
                xml.popTag();
            }
            xml.popTag();
        }
    }
    xml.popTag();
    xml.popTag();
    
    xml.saveFile("mySettings.batmapp");
}

void ofxDarkKnight::loadProject(string file)
{
    if ( xml.loadFile(file) )
    {
        xml.pushTag("PROJECT");
        xml.pushTag("MODULES");
        int numModules = xml.getNumTags("MODULE");
     
        for (int i=0; i<numModules; i++)
        {
            xml.pushTag("MODULE", i);
            string module = xml.getValue("NAME", "");
            int x = xml.getValue("X", 0);
            int y = xml.getValue("Y", 0);
            
            std::transform(module.begin(), module.end(), module.begin(), [](char ch){
                return ch == '_' ? ' ' : ch;
            });
            addModule(module)->gui->setPosition(x, y);
            xml.popTag();
        }
        xml.popTag();
        xml.popTag();
    } else
    {
        cout << "file not loaded" <<endl;
    }
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
    
    cout << "Preset saved " << endl;
}

Module * ofxDarkKnight::createModule(string name)
{
    if(name == "PREVIEW") return new Preview;
    if(name == "MAPPING TOOLS") return new ofxDarkKnightMapping;
    if(name == "MIDI CONTROLLER") return new ofxDarkKnightMidi;
    if(name == "OSC CONTROLLER") return new ofxDarkKnightOsc;
    
    return nullptr;
}



//   ofxDarkKnightMidi /


