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

#ifndef module_hpp
#define module_hpp

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxMidi.h"
#include "unordered_map"
#include "DKWireConnection.hpp"
#include "ofxPostProcessing.h"


#define STRINGIFY(A) #A

struct ModuleSoundParticle
{
	float lifetime;
	float size;
	float noise;
};

class DKModule{

private:
    int     moduleId;
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
    ofPoint translation;
	float	zoom;
    
    ofxDatGuiComponent * selectedComponent;
    unordered_map<string, ofxDatGuiComponent *> midiMappings;
public:
	vector<ofxMidiMessage*> outMidiMessages;
    
	float  moduleGuiWidth;
    bool customParams = false;
    bool moduleIsChild = false;
    ofxDatGui * gui;

    vector<DKWireConnection*> inputs;
    vector<DKWireConnection*> outputs;
    vector<DKWireConnection*> chainOutputs;
    DKModule* chainModule;
    
    virtual void setup() { };
    virtual void update() { };
    virtual void draw() { };
    virtual void render(ofFbo&, ofFbo&) { };
    virtual void addModuleParameters() { };
    virtual void unMount() { };
    virtual void setFbo(ofFbo *){ };
    virtual void setFbo(ofFbo *, int) { };
	virtual void setLight(ofLight*) { };
    virtual void onMouseMove(int, int) { };
    virtual void triggerMidiEvent(){ };
    virtual void triggerMidiMessage(ofxMidiMessage *) { };
    virtual void reset() { };
    
    virtual ofFbo * getFbo(){ return nullptr; };
	virtual ofLight* getLight() { return nullptr; };
    virtual ofxPostProcessing* getChain() { return nullptr; };
    

    void setupModule(string, ofVec2f, bool);
    void setupModule(string, ofVec2f);
    void setupCommon(string, ofVec2f);
    void setChainModule(DKModule*);
    void setupGui();
    void updateModule();
    void updateModule(float, float);
	void updateModule(float, float, float);
    void drawModule();
    
    void enable();
    void disable();
    
    int getModuleId();
    float getModuleWidth();
    float getModuleHeight();
    bool getModuleMidiMapMode();
    bool getModuleInitialized();
    bool getModuleEnabled();
    bool getModuleHasChild();
    ofPoint getTranslation();
    DKModule * getChainModule();
    
    void setModuleWidth(float);
    void setModuleHeight(float);
    void setModuleEnabled(bool);
    void setModuleHasChild(bool);
    void setModuleId(int);
    
    ofxDatGuiComponent * getOutputComponent(int, int);
    ofxDatGuiComponent * getInputComponent(int, int);
    ofxDatGuiTheme * getGuiTheme();
    
    DKWireConnection * getOutputConnection(float, float);
    DKWireConnection * getInputConnection(float, float);
    
    void setMidiMapping(string);
    void setMidiScale(string, double);
    void setModuleMidiMapMode(bool);
    void setResolution(int, int);
    void toggleMidiMap();
    
    void deleteMapping();
    
    void onSliderEventParent(ofxDatGuiSliderEvent);
    void onEnableChange(ofxDatGuiToggleEvent);
    
    void addSlider(string, int &, int, int, int);
    void addSlider(string, float &, float, float, float);
    
    void addSlider(string, int &, int, int, int, int);
    void addSlider(string, float &, float, float, float, int);
    
    void addInputConnection(DKConnectionType);
    void addOutputConnection(DKConnectionType);
    void addChainOutputConnection(DKConnectionType);

	void addInputConnection(DKConnectionType, string);
	void addOutputConnection(DKConnectionType, string);
    
    void sendMidiMessage(ofxMidiMessage *);

    string getName();
};

#endif /* module_hpp */
