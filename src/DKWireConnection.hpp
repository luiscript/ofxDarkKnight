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

#ifndef DKWireConnection_hpp
#define DKWireConnection_hpp

#include "ofMain.h"

enum class DKConnectionType {
    DK_TEXTURE = 0,
    DK_FBO = 1,
    DK_SLIDER = 2,
    DK_INT = 3,
	DK_LIGHT = 4,
    DK_CHAIN = 5,
    DK_EMPTY = 6
};

struct DKFboChain
{
    ofFbo* readFbo;
    ofFbo* writeFbo;
};


class DKWireConnection{
private:
    ofPoint wireConnectionPos;
    string name;
    DKConnectionType connectionType;
    double * scale;
    ofFbo * fboPtr;
	ofLight* light;
public:
    void setup(ofPoint, string);
    void setup(ofPoint, DKConnectionType);
    
    void updateWireConnectionPos(int, int);
    
    void draw();
    
    float getDist(DKWireConnection *);
    string getName();
    ofPoint getWireConnectionPos();
    double * getScale();
    DKConnectionType getConnectionType();
    ofColor getWireConnectionColor();
    
    void setScale(double *);
    void setWireConnectionType(DKConnectionType);
    void setWireConnectionPos(ofPoint);
	void setName(string);
    
    DKWireConnection * testWireConnection(float, float);
    
    ofFbo * getFbo();
    void setFbo(ofFbo *);

	ofLight* getLight();
	void setLight(ofLight*);
};


#endif /* DKWireConnection_hpp */
