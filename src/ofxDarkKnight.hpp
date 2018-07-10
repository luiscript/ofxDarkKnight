/*
 Copyright (C) 2018 Luis Fernando Garc�a [http://luiscript.com]
 
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

#ifndef modulesManager_hpp
#define modulesManager_hpp

#include "unordered_map"

#include "ofMain.h"

#include "module.hpp"
#include "ofxDatGui.h"
#include "wireConnection.hpp"
#include "wire.hpp"
#include "ofxVectorGraphics.h"
#include "preview.hpp"
#include "mediaPool.hpp"
#include "basic.hpp"
#include "screenOutput.hpp"


class ofxDarkKnight {
private:
    ofxDatGuiTheme * theme;
    ofVec2f resolution;
    ofVec2f translation;
    ofPoint pointer;
    bool showExplorer;
    bool drawing;
    bool midiMapMode;
    bool cmdKey;
    bool shiftKey;
    
    Wire * currentWire;
    vector<Wire> wires;
    
    ofxDatGui * gui;
    ofxDatGuiScrollView * componentsList;
    
    unordered_map<string, Module*> modulesPool;
    unordered_map<string, Module*> modules;
    
    list<string> poolNames;
public:
    
    ofxDarkKnight();
    ~ofxDarkKnight();
    
    shared_ptr<ofAppBaseWindow> mainWindow;
    
    void setup();
    void update();
    
    void draw();
    
    void close();
    
    void toggleList();
    void toggleMappingMode();
    
    void addModule(string, Module *);
    void deleteModule(string);
    void deleteComponentWires(ofxDatGuiComponent *);
    void deleteFocusedModule();
    
    void handleMousePressed(ofMouseEventArgs&);
    void handleMouseDragged(ofMouseEventArgs&);
    void handleMouseReleased(ofMouseEventArgs&);
    void handleMouseScrolled(ofMouseEventArgs&);
    void handleKeyPressed(ofKeyEventArgs & keyboard);
    void handleKeyReleased(ofKeyEventArgs & keyboard);
    
    void onResolutionChange(ofVec2f &);
    void onComponentListChange(ofxDatGuiScrollViewEvent e);
};

#endif /* modulesManager_hpp */