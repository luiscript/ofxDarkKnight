/*
 Copyright (C) 2018 Luis Fernando García [http://luiscript.com]
 
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

#ifndef screenOutput_hpp
#define screenOutput_hpp

#include "ofMain.h"
#include "ofxDatGui.h"
#include "DKModule.hpp"
#include "GLFW/glfw3.h"


class DKScreenOutput : public DKModule{
private:
    ofFbo * fbo;
    bool drawFbo = false;
    string serverName;
    vector<string> monitorsName;
    shared_ptr<ofAppBaseWindow> display;
    
public:
    void setup();
    void drawDisplay(ofEventArgs & args);
    void setFbo(ofFbo *);
    void addModuleParameters();
    ofFbo * getFbo();
    void onVideoOutputChange(ofxDatGuiDropdownEvent);    
    shared_ptr<ofAppBaseWindow> mainWindow;
};



#endif /* screenOutput_hpp */
