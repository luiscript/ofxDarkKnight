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

#include "wireConnection.hpp"


void WireConnection::setup(ofPoint p, string n)
{
    wireConnectionPos = p;
    name = n;
}

void WireConnection::setup(ofPoint p, ConnectionType type)
{
    wireConnectionPos = p;
    connectionType = type;
}

void WireConnection::updateWireConnectionPos(int x, int y)
{
    wireConnectionPos.x = x;
    wireConnectionPos.y = y;
}

void WireConnection::draw()
{
    ofPushStyle();
    ofSetColor(getWireConnectionColor());
    ofDrawCircle(wireConnectionPos.x, wireConnectionPos.y, 10);
    ofSetColor(0);
    ofDrawCircle(wireConnectionPos.x, wireConnectionPos.y, 6);
    ofPopStyle();
}

void WireConnection::setWireConnectionPos(ofPoint p)
{
    wireConnectionPos = p;
}

ofPoint WireConnection::getWireConnectionPos()
{
    return wireConnectionPos;
}

string WireConnection::getName()
{
    return name;
}

ConnectionType WireConnection::getConnectionType()
{
    return connectionType;
}

WireConnection * WireConnection::testWireConnection(float x, float y)
{
    ofPoint p = getWireConnectionPos();
    float dist = ofDist(p.x, p.y, x, y);
    return dist < 15.0 ? this : nullptr;
}

float WireConnection::getDist(WireConnection * wire)
{
    return ofDist(wireConnectionPos.x, wireConnectionPos.y, wire->wireConnectionPos.x, wire->wireConnectionPos.y);
}


void WireConnection::setScale(double * sc)
{
    scale = sc;
}

double * WireConnection::getScale()
{
    return scale;
}

void WireConnection::setFbo(ofFbo * fbo)
{
    fboPtr = fbo;
}

void WireConnection::setWireConnectionType(ConnectionType type)
{
    connectionType = type;
}

ofFbo * WireConnection::getFbo()
{
    return fboPtr;
}

void WireConnection::setName(string n)
{
	name = n;
}

ofLight* WireConnection::getLight()
{
	return light;
}

void WireConnection::setLight(ofLight* l)
{
	light = l;
}

void WireConnection::checkConnectionOnDrag(ConnectionType type)
{
    if(connectionType == type)
    {
        //change color of connection to indicate that you can connect
    }
}

ofColor WireConnection::getWireConnectionColor()
{
	if (connectionType == ConnectionType::DK_FBO) return ofColor(180, 180, 0);
	if (connectionType == ConnectionType::DK_SLIDER) return ofColor(255, 255, 255);
	if (connectionType == ConnectionType::DK_LIGHT) return ofColor(0, 180, 180);
}
