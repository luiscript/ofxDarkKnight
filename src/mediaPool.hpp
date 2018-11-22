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

#ifndef canvasCollection_hpp
#define canvasCollection_hpp

#include "module.hpp"
#include <math.h>

//include this with ofxDarkKnightMidi
#include "ofxMidi.h"


class CollectionItem{
public:
    string collectionName;
    Module * canvas;
    string fileName;
    ofImage * thumbnail;
};


class MediaPool : public Module
{
private:
    string collectionName;
    int index;
    int nextIndex;
    bool showMediaPool;
    bool drawFbo = false;
    bool hasInput = false;
    bool drawMode = false;
    Module * currentCanvas;

    ofFbo mainFbo;
    ofFbo *inputFbo;
    ofLight light;
    
    int alpha;
    
    int diffuseR;
    int diffuseG;
    int diffuseB;
    
    int ambientR;
    int ambientG;
    int ambientB;
    
    float lightPositionX;
    float lightPositionY;
    float lightPositionZ;
    bool enableLight = true;
    
    ofVec2f * translation;
    
    ofEvent<string> deleteModule;
    ofEvent<Module*> addModule;
    unordered_map<string, Module*> * modules;
    int yOffsetGui;
    ofTrueTypeFont	font;
    
    float size;
    float time0;
    float time;
    
    
public:
    vector<CollectionItem> collection;
    unordered_map<string, int> midiMappings;
    
    void init();
    void setup();
    void update();
    void draw();
    void drawMasterOutput();
    void drawMasterInput();
    void addModuleParameters();
    void addCustomParameters();
    void onMidiInputListChange(ofxDatGuiDropdownEvent);
    void onToggleDraw(ofxDatGuiToggleEvent);
    
    void onMatrix1Change(ofxDatGuiMatrixEvent);
    void onKeyboardEvent(ofKeyEventArgs & e);
    
    Module * getChildModule();
    
    void drawMediaPool();
    void updatePoolIndex(int, int);
    //void onMouseMove(int, int);
    void triggerPoolMedia(int);
    void gotMidiMapping(string);
    void gotMidiMessage(ofxMidiMessage*);
    void sendMidiNote(ofxMidiMessage*);
    
    void unMount();
    
    ofFbo * getFbo();
    void setFbo(ofFbo *);
    
    void setCollectionName(string);
    void setModulesReference(unordered_map<string, Module*> *);
    void setTranslationReferences(ofVec2f *);
    
    void mousePressed(ofMouseEventArgs & mouse);
    
    void enableLighting(bool);
};

#endif /* canvasCollection_hpp */
