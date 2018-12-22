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
    
    modulesPool = *pool;
    const int s = modulesPool.size();
    
    for(pair<string, Module*> module : modulesPool )
        poolNames.push_back(module.first);
    
    poolNames.sort();
    
    componentsList = new ofxDatGuiScrollView("Modules list", 10);
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
}


void ofxDarkKnight::update()
{
    for(pair<string, Module*> module : modules )
        if(module.second->getModuleEnabled())
            module.second->updateModule(translation.x, translation.y);
    
    if(loadWires)
    {
        loadProjectWires();
        loadWires = false;
    }
    
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
            if(module.second->getModuleEnabled())
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
    }
    
    if( midiMapMode )
        checkOutputConnection(x, y, "*");
    
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
    int x = (int) mouse.x - translation.x;
    int y = (int) mouse.y - translation.y;

    if( midiMapMode ) checkInputConnection(x, y, "*");
}

void ofxDarkKnight::handleMouseScrolled(ofMouseEventArgs & mouse)
{
    //translation is not fully supported with ofxDarkKnightMapping, be careful.
    //translation.x += 2*mouse.scrollX;
    //translation.y += 2*mouse.scrollY;
}

void ofxDarkKnight::checkOutputConnection(int x, int y, string moduleName)
{
    WireConnection * output;
    WireConnection * input;
    
    for(pair<string, Module*> module : modules )
    {
       // if(!module.second->getModuleEnabled()) return;
        
        output = module.second->getOutputConnection(x, y);

        //true if we click on output node
        if(output != nullptr &&
           (module.second->getName() == moduleName || moduleName == "*" ))
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
            for(auto it = wires.begin(); it != wires.end(); it++)
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
}

void ofxDarkKnight::checkInputConnection(int x, int y, string moduleName)
{
    WireConnection * input;

    for(pair<string, Module*> module : modules )
    {
        //if(!module.second->getModuleEnabled()) return;
        input = module.second->getInputConnection(x, y);
        //true if user released the wire on input connection
        if(input != nullptr && currentWire != nullptr &&
           (module.second->getName() == moduleName || moduleName == "*"))
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
                        hapPlayer->gui->setWidth(450);
                        hapPlayer->loadFile(file.getAbsolutePath());
                        modules.insert({"HAP: " + file.getFileName(), hapPlayer});
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
                    newPool->setTranslationReferences(&translation);
                    modules.insert({"SKETCH POOL 1", newPool});
                    
                    hapPlayer->gui->setWidth(450);
                    hapPlayer->loadFile(file.getAbsolutePath());
                    modules.insert({"HAP: " + file.getFileName(), hapPlayer});
                    return;
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
        
        newModule->setupModule(it->first, resolution);
        newModule->gui->setPosition(ofGetMouseX() - 100, ofGetMouseY() - 15);
        
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
            int mIndex = 0;
            for(CollectionItem item : mp->collection)
            {
                Module * m = item.canvas;
                if(mIndex > 0) m->disable();
                string childName = it->second->getName() + "/" + m->getName();
                modules.insert({childName, m});
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
    if(shiftKey && (keyboard.key == 'm' || keyboard.key == '<')){
        toggleMappingMode();
    }

    if (cmdKey && keyboard.key == OF_KEY_BACKSPACE)
    {
        deleteFocusedModule();
    }
    
    //cmd + 's' to save the project
    if( cmdKey && keyboard.key == 's')
    {
        ofFileDialogResult saveFileResult =
            ofSystemSaveDialog("project.batmapp", "Save project");
        
        if (saveFileResult.bSuccess){
            saveProject(saveFileResult.getPath(), saveFileResult.getName());
        }
    }
    
    //cmd + o
    if(cmdKey && keyboard.key == 'o')
    {
            ofFileDialogResult loadFileResult = ofSystemLoadDialog("Open batmapp project");
            if(loadFileResult.bSuccess)
            {
               loadProject(loadFileResult.getPath(), loadFileResult.getName());
            }
    }
    
    //cmd + shift + 's' to save preset
    if( shiftKey && cmdKey && keyboard.key == 's')
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

void ofxDarkKnight::saveProject(string fileName, string path)
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
            
            if(module.second->getModuleHasChild())
            {
                MediaPool * mediaPool = static_cast<MediaPool*>(module.second);
                xml.setValue("INDEX", mediaPool->getCurrentIndex(), moduleIndex);
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
                    int compIndex = 0;
                    for(ofxDatGuiComponent * component : item.canvas->params->children)
                    {
                        xml.addTag("PARAM");
                        xml.pushTag("PARAM", compIndex);
                        
                        string compName = component->getName();
                        std::transform(compName.begin(), compName.end(), compName.begin(), [](char ch){
                            return ch == ' ' ? '_' : ch;
                        });
                        xml.setValue("NAME", compName, compIndex);
                        xml.setValue("SCALE", component->getComponentScale(), compIndex);
                        
                        xml.popTag();
                        compIndex++;
                    }
                    itemIndex++;
                    xml.popTag();
                    xml.popTag();
                }
                
                xml.popTag();
            }
            moduleIndex++;
            xml.popTag();
        }
    }
    xml.popTag();
    
    xml.addTag("WIRES");
    xml.pushTag("WIRES");
    int wIndex = 0;
    for (auto wire : wires)
    {
        xml.addTag("WIRE");
        xml.pushTag("WIRE", wIndex);
        ofPoint po = wire.getOutput()->getWireConnectionPos();
        ofPoint pi = wire.getInput()->getWireConnectionPos();
        xml.setValue("OINDEX", wire.outputModule->getModuleIndex(), wIndex);
        xml.setValue("XO",  po.x, wIndex);
        xml.setValue("YO",  po.y, wIndex);
        xml.setValue("IINDEX", wire.inputModule->getModuleIndex(), wIndex);
        xml.setValue("XI",  pi.x, wIndex);
        xml.setValue("YI",  pi.y, wIndex);
        xml.popTag();
        wIndex++;
    }
    xml.popTag();
    xml.popTag();
    
    xml.save(path);
    xml.saveFile(fileName);
    
}

void ofxDarkKnight::loadProject(string path, string file)
{
    xml.load(path);
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
            
            Module * tempModule = addModule(module);
            if (tempModule != nullptr)
            {
                tempModule->gui->setPosition(x, y);
                if(tempModule->getModuleHasChild())
                {
                    MediaPool * mediaPool = static_cast<MediaPool*>(tempModule);
                    xml.pushTag("ITEMS");
                    int numItems = xml.getNumTags("ITEM");
                    for(int i=0; i < numItems; i++)
                    {
                        xml.pushTag("ITEM", i);
                        xml.pushTag("PARAMS");
                        int numParams = xml.getNumTags("PARAM");
                        for(int p=0; p < numParams; p++)
                        {
                            xml.pushTag("PARAM", p);
                            string name = xml.getValue("NAME","");
                            std::transform(name.begin(), name.end(), name.begin(), [](char ch){
                                return ch == '_' ? ' ' : ch;
                            });
                            double scale = xml.getValue("SCALE", 0.0);
                        
                            for(ofxDatGuiComponent * comp : mediaPool->collection[i].canvas->params->children)
                            {
                                if(comp->getName() == name)
                                {
                                    comp->setComponentScale(scale);
                                }
                            }
                            xml.popTag();
                        }
                        xml.popTag();
                        xml.popTag();
                    }
                    xml.popTag();
                }
            }
            
            
            xml.popTag();
        }
        xml.popTag();
        
        xml.popTag();
        
        //wires needs to be loaded after all modules are "updated"
        loadWires = true;
    } else
    {
        cout << "file not loaded" <<endl;
    }
}

void ofxDarkKnight::loadProjectWires()
{
    xml.pushTag("PROJECT");
    xml.pushTag("WIRES");
    int numWires = xml.getNumTags("WIRE");
    
    for (int i=0; i<numWires; i++)
    {
        xml.pushTag("WIRE", i);
        int output = xml.getValue("OINDEX", -1);
        int xo = xml.getValue("XO",0);
        int yo = xml.getValue("YO",0);
        int input = xml.getValue("IINDEX", -1);
        int xi = xml.getValue("XI",0);
        int yi = xml.getValue("YI",0);
        
        checkOutputConnection(xo, yo, "*");
        checkInputConnection(xi, yi, "*");
        xml.popTag();
    }
    //wires
    xml.popTag();
    xml.popTag();
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
    else if(name == "MAPPING TOOLS") return new ofxDarkKnightMapping;
    else if(name == "MIDI CONTROLLER") return new ofxDarkKnightMidi;
    else if(name == "OSC CONTROLLER") return new ofxDarkKnightOsc;
    else if(name == "SYPHON SERVER") return new ofxDarkKnightSyphon;
    else if(name == "SYPHON CLIENT") return new DarkKnightSyphonClient;
    
    return nullptr;
}



//   ofxDarkKnightMidi /


