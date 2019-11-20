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
#ifndef DKWire_hpp
#define DKWire_hpp

#include "ofMain.h"
#include "DKModule.hpp"
#include "DKWireConnection.hpp"

class DKWire{
public:
    DKWire();
    
    void update();

    void draw();
    void drawCurrentWire(ofPoint);
    void drawWire(ofPoint, ofPoint, ofPoint);
    
    float getDistance();
    ofPoint getWireControlPoint(ofPoint, ofPoint, float);
    DKWireConnection * getInput();
    DKWireConnection * getOutput();
    DKConnectionType getConnectionType();
    //vector<DKModule*>* getPasses();
    
    void setInputConnection(DKWireConnection *);
    void setOutputConnection(DKWireConnection *);
    void setInputModule(DKModule *);
    void setOutputModule(DKModule *);
    void setConnectionType(DKConnectionType);
    //void setPasses(vector<DKModule*>*);

    DKWireConnection * input;
    DKWireConnection * output;
    
    string type;
    
    void * data;
    ofFbo * fbo;
	ofLight* light;
    //DKFboChain* chain;
    vector<DKModule*>* passes;
    double * scale;
    ofxDatGuiSlider * slider;
    bool active;
    
    float connectionRadius;
    float wiresTension;
    
    DKModule * inputModule;
    DKModule * outputModule;
private:
    bool drawing;
    ofPoint inputPoint;
    ofPoint outputPoint;
    string name;
	int wireId;
    DKConnectionType connectionType;
};

#endif /* DKWire_hpp */
