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

#include "module.hpp"


void Module::setupModule(string name, ofVec2f resolution, bool child = false)
{
    moduleIsChild = child;
    setupCommon(name, resolution);
}

void Module::setupModule(string name, ofVec2f resolution)
{
    setupCommon(name, resolution);
}

void Module::setupCommon(string name, ofVec2f resolution)
{
    moduleName = name;
    moduleIndex = -1;
    
    moduleMidiMapMode = false;
    moduleEnabled = true;
    moduleWidth = resolution.x;
    moduleHeight = resolution.y;
    
    selectedComponent = nullptr;
    setupGui();
    setup();
    
    float x = gui->getPosition().x + gui->getWidth() + 17;
    float y = gui->getPosition().y + 13;
    
//    fboOutput = new WireConnection;
//    fboOutput->setup(ofPoint(x,y), "fboOutput");
//
//    x = gui->getPosition().x - 17;
//    y = gui->getPosition().y + 13;
//
//    fboInput = new WireConnection;
//    fboInput->setup(ofPoint(x,y), "fboInput");
    
    moduleIsChild = false;
    moduleInitialized = true;
}

void Module::setupGui()
{
    gui = new ofxDatGui();
    gui->addHeader(moduleName);
    
    if(moduleIsChild) {
        params = gui->addFolder("PARAMS");
        params->expand();
    }
    
    addModuleParameters();
    gui->setWidth(450);
}

void Module::updateModule(float tx, float ty)
{
    //translation.x = tx;
    //translation.y = ty;
    gui->setTranslation(tx, ty);
    updateModule();
}

void Module::updateModule()
{
    gui->update();
    if (moduleEnabled) {
        update();
    }
    
    float x = gui->getPosition().x + gui->getWidth() + 17;
    float y = gui->getPosition().y + 13;

    for (int o=0; o<outputs.size(); o++) {
        outputs[o]->setWireConnectionPos(ofPoint(x,y + o*30));
    }
    
    
    
    //fboOutput->setWireConnectionPos(ofPoint(x,y));
    x = gui->getPosition().x - 17;
    y = gui->getPosition().y + 13;
    
    for (int i=0; i<inputs.size(); i++) {
        inputs[i]->setWireConnectionPos(ofPoint(x,y + i*30));
    }
    
    
    //fboInput->setWireConnectionPos(ofPoint(x,y));
    
    
}

void Module::drawModule()
{
    gui->draw();
    //drawMasterInput();
    //drawMasterOutput();
    
    if(getModuleMidiMapMode())
    {
        for(auto out : outputs) out->draw();
        for(auto inp : inputs ) inp->draw();
    }
    if (moduleEnabled) draw();
}


void Module::toggleMidiMap()
{
    moduleMidiMapMode = !moduleMidiMapMode;
    gui->toggleMidiMode();
}

string Module::getName()
{
    return moduleName;
}

int Module::getModuleIndex()
{
    return moduleIndex;
}
float Module::getModuleWidth()
{
    return moduleWidth;
}

float Module::getModuleHeight()
{
    return moduleHeight;
}

bool Module::getModuleMidiMapMode()
{
    return moduleMidiMapMode;
}

ofxDatGuiComponent * Module::getOutputComponent(int x, int y)
{
    return gui->getOutputComponent(x, y);
}

ofxDatGuiComponent * Module::getInputComponent(int x, int y)
{
    return gui->getInputComponent(x, y);
}

WireConnection * Module::getOutputConnection(float x , float y)
{
    WireConnection * foundedWireConnection = nullptr;
    for(auto out : outputs)
    {
        foundedWireConnection = out->testWireConnection(x, y);
        if(foundedWireConnection != nullptr)
        {
            return foundedWireConnection;
        }
    }
    return gui->testOutputConnection(x, y);
}

WireConnection * Module::getInputConnection(float x , float y)
{
    WireConnection * foundedWireConnection = nullptr;
    for(auto inp : inputs)
    {
        foundedWireConnection = inp->testWireConnection(x, y);
        if(foundedWireConnection != nullptr)
        {
            return foundedWireConnection;
        }
    }
    return gui->testInputConnection(x, y);
}

ofxDatGuiTheme * Module::getGuiTheme()
{
    return nullptr;
}


WireConnection * Module::getMainOutput(int x, int y)
{
    float xC = gui->getPosition().x + gui->getWidth() + 17;
    float yC = gui->getPosition().y + 13;
//    fboOutput->setWireConnectionPos(ofPoint(xC, yC));
//
//    return ofDist(x, y, xC, yC) <= 15.0 ? fboOutput : nullptr;
    return nullptr;
}

WireConnection * Module::getMainInput(int x, int y)
{
    float xC = gui->getPosition().x - 17;
    float yC = gui->getPosition().y + 13;
    
//    fboInput->setWireConnectionPos(ofPoint(xC, yC));
//
//    return ofDist(x, y, xC, yC) <= 15.0 ? fboInput : nullptr;
    return nullptr;
}

bool Module::getModuleInitialized()
{
    return moduleInitialized;
}

bool Module::getModuleEnabled()
{
    return moduleEnabled;
}

bool Module::getModuleHasChild()
{
    return moduleHasChild;
}

//ofPoint Module::getTranslation()
//{
//    return translation;
//}

void Module::onSliderEventParent(ofxDatGuiSliderEvent e)
{
    if(e.target->getMidiMode())
    {
        //reset midiMapMode for all compoenents and set current component to midiMapMode
        gui->resetMidiMap();
        e.target->toggleMidiMap(true);
        selectedComponent = e.target;
    }
}

void Module::onEnableChange(ofxDatGuiToggleEvent e)
{
    moduleEnabled = e.target->getChecked();
}

void Module::setModuleWidth(float w)
{
    moduleWidth = w;
}

void Module::setModuleHeight(float h)
{
    moduleHeight = h;
}

void Module::setModuleMidiMapMode(bool m)
{
    if(getModuleMidiMapMode() != m)
    {
        moduleMidiMapMode = m;
        gui->toggleMidiMode();
    }
}

void Module::setModuleEnabled(bool e)
{
    moduleEnabled = e;
}

void Module::setModuleHasChild(bool c)
{
    moduleHasChild = c;
}

void Module::setModuleIndex(int index)
{
    
    moduleIndex = index;
}

void Module::setResolution(int w, int h)
{
    setModuleWidth(w);
    setModuleHeight(h);
}

void Module::drawOutputConnection()
{
    if(getModuleMidiMapMode())
    {
        
        float x = gui->getPosition().x + gui->getWidth() + 17;
        float y = gui->getPosition().y + 13;
        
        ofPushStyle();
        ofSetColor(180,180,0);
        ofDrawCircle(x, y, 10);
        ofSetColor(0);
        ofDrawCircle(x, y, 6);
        ofPopStyle();
    }
}

void Module::drawInputConnection()
{
    if(getModuleMidiMapMode())
    {
        
        float x = gui->getPosition().x - 17;
        float y = gui->getPosition().y + 13;

        ofPushStyle();
        ofSetColor(180,180,0);
        ofDrawCircle(x, y, 10);
        ofSetColor(0);
        ofDrawCircle(x, y, 6);
        ofPopStyle();
        
    }

}

void Module::enable()
{
    gui->setEnabled(true);
    gui->setVisible(true);
    moduleEnabled = true;
}

void Module::disable()
{
    gui->setEnabled(false);
    gui->setVisible(false);
    moduleEnabled = false;
}

void Module::addSlider(string name, int & add, int min, int max, int val)
{
    params->addSlider(name, min, max, val)->bind(add);
}

void Module::addSlider(string name, float & add, float min, float max, float val)
{
    params->addSlider(name, min, max, val)->bind(add);
}

void Module::addInputConnection(ConnectionType t)
{
    WireConnection * input = new WireConnection;
    input->setWireConnectionType(t);
    inputs.push_back(input);
}

void Module::addOutputConnection(ConnectionType t)
{
    WireConnection * output = new WireConnection;
    output->setWireConnectionType(t);
    outputs.push_back(output);
}

