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

#include "DKWireConnection.hpp"


void DKWireConnection::setup(ofPoint p, string n)
{
    wireConnectionPos = p;
    name = n;
}

void DKWireConnection::setup(ofPoint p, DKConnectionType type)
{
    wireConnectionPos = p;
    connectionType = type;
}

void DKWireConnection::updateWireConnectionPos(int x, int y)
{
    wireConnectionPos.x = x;
    wireConnectionPos.y = y;
}

void DKWireConnection::draw()
{
    if(connectionType != DKConnectionType::DK_EMPTY)
    {
        ofPushStyle();
        ofSetCircleResolution(60);
        ofNoFill();
        ofSetLineWidth(2);
        ofSetColor(getWireConnectionColor());
        ofDrawCircle(wireConnectionPos.x, wireConnectionPos.y, 8);
        ofPopStyle();
    }
}

void DKWireConnection::setWireConnectionPos(ofPoint p)
{
    wireConnectionPos = p;
}

ofPoint DKWireConnection::getWireConnectionPos()
{
    return wireConnectionPos;
}

string DKWireConnection::getName()
{
    return name;
}

DKConnectionType DKWireConnection::getConnectionType()
{
    return connectionType;
}

unsigned DKWireConnection::getIndex()
{
    return connectionIndex;
}

DKWireConnection * DKWireConnection::testWireConnection(float x, float y)
{
    ofPoint p = getWireConnectionPos();
    float dist = ofDist(p.x, p.y, x, y);
    return dist < 15.0 ? this : nullptr;
}

float DKWireConnection::getDist(DKWireConnection * wire)
{
    return ofDist(wireConnectionPos.x, wireConnectionPos.y, wire->wireConnectionPos.x, wire->wireConnectionPos.y);
}


void DKWireConnection::setScale(double * sc)
{
    scale = sc;
}

void DKWireConnection::setIndex(unsigned indexValue)
{
    connectionIndex = indexValue;
}

double * DKWireConnection::getScale()
{
    return scale;
}

void DKWireConnection::setFbo(ofFbo * fbo)
{
    fboPtr = fbo;
}

void DKWireConnection::setWireConnectionType(DKConnectionType type)
{
    connectionType = type;
}

ofFbo * DKWireConnection::getFbo()
{
    return fboPtr;
}

void DKWireConnection::setName(string n)
{
	name = n;
}

ofLight* DKWireConnection::getLight()
{
	return light;
}

void DKWireConnection::setLight(ofLight* l)
{
	light = l;
}

ofColor DKWireConnection::getWireConnectionColor()
{
	if (connectionType == DKConnectionType::DK_FBO
        || connectionType == DKConnectionType::DK_MULTI_FBO) return ofColor(180, 180, 0);
	if (connectionType == DKConnectionType::DK_SLIDER) return ofColor(255, 255, 255);
	if (connectionType == DKConnectionType::DK_LIGHT) return ofColor(0, 180, 180);
    if (connectionType == DKConnectionType::DK_CHAIN) return ofColor(226, 88, 33);
}
