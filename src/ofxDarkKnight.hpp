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

#ifndef ofxDarkKnight_hpp
#define ofxDarkKnight_hpp

#include "unordered_map"
#include "ofMain.h"
#include "ofxDatGui.h"

// Core modules
#include "DKModule.hpp"
#include "DKMediaPool.hpp"
#include "DKWireConnection.hpp"
#include "DKWire.hpp"

// Internal modules
#include "DKColorInverter.h"
#include "DKColorShader.hpp"
#include "DKConfig.hpp"
#include "DKLight.hpp"
#include "DKLiveShader.hpp"
#include "DKLfo.hpp"
#include "DKPreview.hpp"
#include "DKMixer.hpp"
#include "DKPerlin.hpp"
#include "DKScreenOutput.hpp"
#include "DKSliderInverter.hpp"

// External modules
#include "DKMidi.hpp"
#include "DKAbletonLink.hpp"
#include "DKVideoRecorder.hpp"
#include "DKMapping.hpp"
#include "DKOsc.hpp"
#include "DKPostProcessingFx.h"
#include "DKSyphonSpout.hpp"
#include "DKHap.hpp"

// Collections
#include "basic.hpp"

// I dare you to add your own here!

template<typename T> DKModule* createInstance() { return new T; }
typedef map<string, DKModule* (*)()> map_type;

class ofxDarkKnight : public ofxMidiListener{
private:
    unordered_map<string, DKModule*> modulesPool;
    unordered_map<string, DKModule*> modules;
    list<string> poolNames;
    
    ofVec2f resolution;
    ofVec2f translation;
    ofPoint pointer;
    
    bool showExplorer;
    bool drawing;
    bool cmdKey;
    bool shiftKey;
    bool altKey;
    bool loadWires;
    DKConnectionType currentWireConnectionType;
    
    DKWire* currentWire;
    vector<DKWire> wires;
    
    ofxDatGui* gui;
    ofxDatGuiScrollView* componentsList;

    ofxMidiOut darkKnightMidiOut;

	int startX;
	int startY;
	int moduleId;
    float zoom;
public:
    ofxDarkKnight();
    ~ofxDarkKnight();
    
	bool midiMapMode;
    shared_ptr<ofAppBaseWindow> mainWindow;
	map_type factory;
    
    void setup();
    void update();
    void draw();
    void close();
    
    void toggleList();
    void toggleMappingMode();
    
    void addModule(string, DKModule *);
    DKModule * addModule(string);
    void deleteModule(string);
    void deleteComponentWires(ofxDatGuiComponent *, int);
    void deleteFocusedModule();
	void deleteAllModules();
    
    //mouse event handlers
    void handleMousePressed(ofMouseEventArgs&);
    void handleMouseDragged(ofMouseEventArgs&);
    void handleMouseReleased(ofMouseEventArgs&);

    void handleMouseScrolled(ofMouseEventArgs&);
    void handleKeyPressed(ofKeyEventArgs & keyboard);
    void handleKeyReleased(ofKeyEventArgs & keyboard);
    
    void handleDragEvent(ofDragInfo&);
    
    void onResolutionChange(ofVec2f &);
    void onComponentListChange(ofxDatGuiScrollViewEvent e);
    void newMidiMessage(ofxMidiMessage &);
    
    void savePreset();
    
    void checkInputConnection(float, float, string);
    void checkOutputConnection(float, float, string);

    void sendMidiMessage(ofxMidiMessage &);

	unordered_map<string, DKModule*>* getModulesReference();
	vector<DKWire>* getWiresReference();
	ofVec2f* getTranslationReference();
	float* getZoomReference();

	int getNextModuleId();
	void loadProjectFromXml(ofXml);
	void setTranslation(ofVec2f);
	void setZoom(float);
    void resizeWindow(int, int);
};

#endif /* ofxDarkKnight_hpp */
