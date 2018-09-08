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
    // the modules that you can load in your app
   
    
    theme = new ofxDatGuiThemeCharcoal();
    cmdKey = midiMapMode = drawing = showExplorer = false;
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
    componentsList->setPosition(ofGetScreenWidth()/2 - 400, ofGetScreenHeight()/2 - 400);
    
    for(list<string>::iterator it = poolNames.begin(); it != poolNames.end(); it++)
        componentsList->add(*it);
    
    componentsList->onScrollViewEvent(this, &ofxDarkKnight::onComponentListChange);
    ofAddListener(ofEvents().mousePressed, this, &ofxDarkKnight::handleMousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &ofxDarkKnight::handleMouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &ofxDarkKnight::handleMouseReleased);
    ofAddListener(ofEvents().mouseScrolled, this, &ofxDarkKnight::handleMouseScrolled);
    
    ofAddListener(ofEvents().keyPressed, this, &ofxDarkKnight::handleKeyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxDarkKnight::handleKeyReleased);
    
}


void ofxDarkKnight::update()
{
    for(pair<string, Module*> module : modules )
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
    unordered_map<string, Module*>::iterator it;
    it = modules.find(e.target->getName());
    
    if (it == modules.end())
    {
        
        it = modulesPool.find(e.target->getName());
        it->second->setupModule(it->first, theme, resolution);
        it->second->gui->setPosition(ofGetMouseX() + 15, ofGetMouseY() + 15);
  
        if(e.target->getName() == "SCREEN OUTPUT")
        {
            ScreenOutput * so = static_cast<ScreenOutput*>(it->second);
            so->mainWindow = mainWindow;
            modules.insert({it->first, so});
        }
//        else if(e.target->getName() == "MIDI CONTROLLER")
//        {
//            ofxDarkKnightMidi * controller = static_cast<ofxDarkKnightMidi*>(it->second);
//            ofAddListener(controller->sendMidi, this, &ofxDarkKnight::onMidiMappingArrive);
//            modules.insert({it->first, controller});
//        }
        else
        {
            modules.insert({it->first, it->second});
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

    }

    toggleList();
}


//void ofxDarkKnight::onMidiMappingArrive(ofxMidiMessage & msg)
//{
//    string mapping;
//    if(msg.control > 0)
//    {
//        mapping = ofToString(msg.channel) + "/" + ofToString(msg.control);
//    } else {
//        mapping = ofToString(msg.channel) + "/" + ofToString(msg.pitch);
//        
//        if(msg.status == MIDI_NOTE_ON)
//            for(pair<string, Module*> module : modules )
//                if(module.second->getModuleHasChild())
//                {
//                    MediaPool * mp = static_cast<MediaPool*>(module.second);
//                    mp->gotMidiPitch(mapping);
//                }
//    }
//}

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
    translation.x += 2*mouse.scrollX;
    translation.y += 2*mouse.scrollY;
}

void ofxDarkKnight::addModule(string moduleName, Module * module)
{
    modules.insert({moduleName, module});
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
    
    // toggle show explorer
    if(cmdKey && keyboard.key == OF_KEY_RETURN)
    {
        toggleList();
    }
    
    // cmd+shift+m -> Toggle midiMap on all layers
    if(keyboard.key == 'm' && cmdKey && shiftKey){
        toggleMappingMode();
    }

    if (keyboard.key == OF_KEY_BACKSPACE)
    {
        deleteFocusedModule();
    }
    
}

void ofxDarkKnight::handleKeyReleased(ofKeyEventArgs &keyboard)
{
    if(keyboard.key == OF_KEY_COMMAND) cmdKey = false;
    if(keyboard.key == OF_KEY_SHIFT) shiftKey = false;
}

void ofxDarkKnight::close()
{
    for(pair<string, Module*> module : modules )
    {
        module.second->unMount();
    }
    modules.clear();
}

/********   uncomment this when ofxDarkKnightMidi is included
 
 void ofxDarkKnight::onMidiMappingArrive(ofxMidiMessage & msg)
{
    string mapping;
    if(msg.control > 0)
    {
        mapping = ofToString(msg.channel) + "/" + ofToString(msg.control);
    } else {
        mapping = ofToString(msg.channel) + "/" + ofToString(msg.pitch);

        if(msg.status == MIDI_NOTE_ON)
            for(pair<string, Module*> module : modules )
                if(module.second->getModuleHasChild())
                {
                    MediaPool * mp = static_cast<MediaPool*>(module.second);
                    mp->gotMidiPitch(mapping);
                }
    }
}
*********/
