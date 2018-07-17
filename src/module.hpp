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

#ifndef module_hpp
#define module_hpp

#include "ofMain.h"
#include "ofxDatGui.h"
#include "unordered_map"
#include "wireConnection.hpp"

class Module{
private:
    int     moduleIndex;
    ofPoint moduleOffset;
    string  moduleName;
    bool    moduleEnabled;
    bool    moduleMidiMapMode;
    bool    moduleDrawMasterOutput;
    bool    moduleDrawMasterInput;
    bool    moduleInitialized = false;
    bool    moduleHasChild = false;
    
    
    
    float   moduleAlpha;
    float   moduleWidth;
    float   moduleHeight;
    
    float   moduleGuiOpacity;
    
    ofxDatGuiComponent * selectedComponent;
    ofxDatGuiTheme * theme;
    
    unordered_map<string, ofxDatGuiComponent *> midiMappings;
    
public:
    
    bool customParams = false;
    bool moduleIsChild = false;
    ofxDatGui * gui;
    //ofxVectorGraphics canvasGraphics;
    ofImage connectionImage;
    
    WireConnection * fboOutput;
    WireConnection * fboInput;
    
    virtual void setup() { };
    virtual void update() { };
    virtual void draw() { };
    virtual void drawMasterInput(){ };
    virtual void drawMasterOutput(){ };
    virtual void addModuleParameters() { };
    virtual void unMount() { };
    virtual ofFbo * getFbo(){ };
    virtual void setFbo(ofFbo *){ };
    virtual void onMouseMove(int, int) { };
    virtual Module * getChildModule() { };
    virtual void triggerMidiEvent(){ };
    
    void init();
    void setupModule(string, ofxDatGuiTheme *, ofVec2f);
    void setupGui(ofxDatGuiTheme *);
    void updateModule();
    void updateModule(float, float);
    void drawModule();
    
    
    void enable();
    void disable();
    
    void drawOutputConnection();
    void drawInputConnection();
    
    int getModuleIndex();
    float getModuleWidth();
    float getModuleHeight();
    bool getModuleMidiMapMode();
    bool getModuleInitialized();
    bool getModuleEnabled();
    bool getModuleHasChild();
    
    
    WireConnection * getMainOutput(int, int);
    WireConnection * getMainInput(int, int);
    
    
    void setModuleWidth(float);
    void setModuleHeight(float);
    void setModuleEnabled(bool);
    void setModuleHasChild(bool);
    
    ofxDatGuiComponent * getOutputComponent(int, int);
    ofxDatGuiComponent * getInputComponent(int, int);
    ofxDatGuiTheme * getGuiTheme();
    
    WireConnection * getOutputConnection(int, int);
    WireConnection * getInputConnection(int, int);
    
    
    void setMidiMapping(string);
    void setMidiScale(string, double);
    void setModuleMidiMapMode(bool);
    void setResolution(int, int);
    void toggleMidiMap();
    
    void deleteMapping();
    
    void onSliderEventParent(ofxDatGuiSliderEvent);
    void onEnableChange(ofxDatGuiToggleEvent);

    
    string getName();
};

#endif /* module_hpp */
