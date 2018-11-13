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


void Module::setupModule(string name, ofxDatGuiTheme * themePtr, ofVec2f resolution)
{
    moduleName = name;
    theme = themePtr;

    moduleMidiMapMode = false;
    moduleEnabled = true;
    moduleWidth = resolution.x;
    moduleHeight = resolution.y;
    
    selectedComponent = nullptr;
    setupGui(theme);
    setup();
    
    float x = gui->getPosition().x + gui->getWidth() + 17;
    float y = gui->getPosition().y + 13;
    
    fboOutput = new WireConnection;
    fboOutput->setup(ofPoint(x,y), "fboOutput");
    
    x = gui->getPosition().x - 17;
    y = gui->getPosition().y + 13;
    
    fboInput = new WireConnection;
    fboInput->setup(ofPoint(x,y), "fboInput");
    
    moduleIsChild = false;  
    moduleInitialized = true;
}

void Module::setupGui(ofxDatGuiTheme * themePtr)
{
    theme = themePtr;
    gui = new ofxDatGui();
    
    gui->setTheme(theme);
    gui->addHeader(moduleName);
    
//    ofxDatGuiToggle * toggle = gui->addToggle("enable");
//    toggle->setChecked(moduleEnabled);
//    toggle->onToggleEvent(this, &Module::onEnableChange);
//    
    addModuleParameters();
    
    gui->setPosition(0, 0);
    gui->setWidth(450);
    
}

void Module::updateModule()
{
    gui->update();
    if (moduleEnabled) {
        update();
    }
    
    float x = gui->getPosition().x + gui->getWidth() + 17;
    float y = gui->getPosition().y + 13;

    fboOutput->setWireConnectionPos(ofPoint(x,y));
    x = gui->getPosition().x - 17;
    y = gui->getPosition().y + 13;
    fboInput->setWireConnectionPos(ofPoint(x,y));
    fboInput->setup(ofPoint(x,y), "fboInput");
}

void Module::updateModule(float tx, float ty)
{
    gui->setTranslation(tx, ty);
    updateModule();
}

void Module::drawModule()
{
    gui->draw();
    drawMasterInput();
    drawMasterOutput();
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

WireConnection * Module::getOutputConnection(int x , int y)
{
    return gui->testOutputConnection(x, y);
}

WireConnection * Module::getInputConnection(int x , int y)
{
    return gui->testInputConnection(x, y);
}

ofxDatGuiTheme * Module::getGuiTheme()
{
    return theme;
}


WireConnection * Module::getMainOutput(int x, int y)
{
    float xC = gui->getPosition().x + gui->getWidth() + 17;
    float yC = gui->getPosition().y + 13;
    fboOutput->setWireConnectionPos(ofPoint(xC, yC));
    
    return ofDist(x, y, xC, yC) <= 15.0 ? fboOutput : nullptr;
}

WireConnection * Module::getMainInput(int x, int y)
{
    float xC = gui->getPosition().x - 17;
    float yC = gui->getPosition().y + 13;
    
    fboInput->setWireConnectionPos(ofPoint(xC, yC));
    
    return ofDist(x, y, xC, yC) <= 15.0 ? fboInput : nullptr;
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


