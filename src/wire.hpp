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
#ifndef wires_hpp
#define wires_hpp

#include "ofMain.h"
#include "module.hpp"
#include "wireConnection.hpp"

class Wire{
public:
    Wire();
    
    void update();

    float getDistance();
    
    void setInput(WireConnection *);
    void setOutput(WireConnection *);
    void draw();
    void drawCurrentWire(ofPoint);
    void drawWire(ofPoint, ofPoint, ofPoint);
    ofPoint getWireControlPoint(ofPoint, ofPoint, float);
    
    WireConnection * getInput();
    WireConnection * getOutput();
    
    WireConnection * input;
    WireConnection * output;
    
    string type;
    void * data;
    ofFbo * fbo;
    double * scale;
    ofxDatGuiSlider * slider;
    bool active;
    
    float connectionRadius;
    float wiresTension;
    
    Module * inputModule;
    Module * outputModule;
private:
    bool drawing;
    ofPoint inputPoint;
    ofPoint outputPoint;
    string name;
};

#endif /* wires_hpp */
