/*
 Copyright (C) 2018 Luis Fernando García Pérez [http://luiscript.com]
 
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


#include "DKScreenOutput.hpp"

void DKScreenOutput::setup()
{
    addInputConnection(DKConnectionType::DK_FBO);
    addOutputConnection(DKConnectionType::DK_FBO);
    display = nullptr;
	drawFbo = false;
}

void DKScreenOutput::setFbo(ofFbo * fboPtr)
{
    fbo  = fboPtr;
    drawFbo = fboPtr != nullptr;
}

ofFbo * DKScreenOutput::getFbo()
{
    return fbo;
}


void DKScreenOutput::addModuleParameters()
{
    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
    
    monitorsName.push_back("NONE");
    
    GLFWvidmode *mode;
    mode = (GLFWvidmode*)glfwGetVideoMode(glfwGetPrimaryMonitor());
    
    for (int i = 0; i<monitorCount; i++) {
        
        mode = (GLFWvidmode*)glfwGetVideoMode(*monitors);
        string fullName = ofToString(glfwGetMonitorName(*monitors)) + " (" + ofToString(mode->width) + " x " + ofToString(mode->height) + ")";
        bool add = true;
        for(auto lmonitor : monitorsName)
        {
            if(lmonitor == fullName) add = false;
        }
        if(add) monitorsName.push_back( fullName );
        monitors++;
    }
    
    ofxDatGuiComponent * component = gui->addDropdown("Output", monitorsName);
    component->onDropdownEvent(this, &DKScreenOutput::onVideoOutputChange);
    monitorsName.clear();
}

void DKScreenOutput::onVideoOutputChange(ofxDatGuiDropdownEvent e)
{
    int index = e.child;
    
    if(index > 0){
        int monitorCount;
        GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
        monitors = monitors + (index - 1);
        int xpos, ypos;
        glfwGetMonitorPos(*monitors, &xpos, &ypos);
        
        ofGLFWWindowSettings settings;
        settings.setSize(getModuleWidth(), getModuleHeight());
        settings.setPosition(ofVec2f(xpos,ypos));
        settings.decorated = false;
        settings.resizable = false;
        settings.title = "BatMapp Output";
        settings.shareContextWith = mainWindow;
        
        glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
        glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
        
        display = ofCreateWindow(settings);
        
        ofAddListener(display->events().draw, this, &DKScreenOutput::drawDisplay);
    } else {
        if(display != nullptr)
        {
            display->setFullscreen(false);
            display->setWindowShouldClose();
            ofRemoveListener(display->events().draw, this, &DKScreenOutput::drawDisplay);
            display.reset();
            display = nullptr;
        }
       
    }
}

void DKScreenOutput::drawDisplay(ofEventArgs & args)
{
    if(drawFbo)
    {
        ofBackground(0);
        fbo->draw(0,0);
    }
}

void DKScreenOutput::unMount()
{
    
}




