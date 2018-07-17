//
//  screenOutput.cpp
//  luiscript
//
//  Created by luiscript on 10/01/18.
//
//

#include "screenOutput.hpp"

void ScreenOutput::setFbo(ofFbo * fboPtr)
{
    fbo  = fboPtr;
    drawFbo = fboPtr != nullptr;
}

ofFbo * ScreenOutput::getFbo()
{
    return fbo;
}

void ScreenOutput::drawMasterInput()
{
    drawInputConnection();
}

void ScreenOutput::drawMasterOutput()
{
    
}


void ScreenOutput::addModuleParameters()
{
    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
    
    monitorsName.push_back("NONE");
    
    GLFWvidmode *mode;
    mode = (GLFWvidmode*)glfwGetVideoMode(glfwGetPrimaryMonitor());
    
    for (int i = 0; i<monitorCount; i++) {
        
        mode = (GLFWvidmode*)glfwGetVideoMode(*monitors);
        
        string fullName = ofToString(glfwGetMonitorName(*monitors)) + " (" + ofToString(mode->width) + " x " + ofToString(mode->height) + ")";
        monitorsName.push_back( fullName );
        monitors++;
    }
    
    ofxDatGuiComponent * component = gui->addDropdown("Output", monitorsName);
    component->onDropdownEvent(this, &ScreenOutput::onVideoOutputChange);
}

void ScreenOutput::onVideoOutputChange(ofxDatGuiDropdownEvent e)
{
    int index = e.child;
    if(index > 0){
        
        int monitorCount;
        GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
        
        vector<string> monitorsName;
        
        GLFWvidmode *mode;
        monitors = monitors + (index - 1);
        mode = (GLFWvidmode*)glfwGetVideoMode(*monitors);

        int xpos, ypos;
        glfwGetMonitorPos(*monitors, &xpos, &ypos);
        
        ofGLFWWindowSettings settings;
        
        settings.width = getModuleWidth();
        settings.height = getModuleHeight();
        settings.setPosition(ofVec2f(xpos,ypos));
        settings.decorated = true;
        settings.resizable = true;
        settings.windowMode = OF_WINDOW;
        settings.multiMonitorFullScreen = false;
        settings.shareContextWith = mainWindow;
        display = ofCreateWindow(settings);
        ofAddListener(display->events().draw, this, &ScreenOutput::drawDisplay);
        //display->setFullscreen(true);
    } else {
        display->setFullscreen(false);
        display->setWindowShouldClose();
    }
}

void ScreenOutput::drawDisplay(ofEventArgs & args)
{
    if(drawFbo)
    {
        ofBackground(0);
        fbo->draw(0,0);
    }
}
