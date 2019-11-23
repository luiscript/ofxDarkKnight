/*
 Copyright (C) 2018 Luis Fernando Garc’a PŽrez [http://luiscript.com]
 
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


#include "ofMain.h"
#include "DKModule.hpp"

class Constellation : public DKModule{
public:
    void setup();
    void update();
    void draw();
    void addModuleParameters();
    
    void onDrawingModeChange(ofxDatGuiMatrixEvent);
    
private:
    
    // parameters
    float connectionDistance;
    int numParticles;
    float particleSize;
    float time0;
    int drawingMode;
    
    float lineSize;
    float lineWidth;
    
    float velocityMultx;
    float velocityMulty;
    float velocityMultz;
    
    ofEasyCam cam;
    float camDistance;
    
    
//    int diffuseR;
//    int diffuseG;
//    int diffuseB;
//    
//    int ambientR;
//    int ambientG;
//    int ambientB;
    
    ofMesh mesh;
    ofMesh meshPoints;
    vector<ofVec3f> vel;

};
