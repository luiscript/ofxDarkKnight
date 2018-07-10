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

#include "wire.hpp"


Wire::Wire()
{
    input = nullptr;
    output = nullptr;
    active = true;
    
    wiresTension = 0.95;
    connectionRadius = 7;
}

void Wire::update()
{
    if(input != nullptr && output != nullptr)
    {
        if(type == "scale")
        {
            slider->setComponentScale(*output->getScale());
        }
        else if(type == "fbo")
        {
            
        }
    }
}

void Wire::draw()
{
    if (this->inputModule->getModuleEnabled() && this->outputModule->getModuleEnabled()) {
        float dist = this->getDistance();
        
        ofPoint p1 = this->getOutput()->getWireConnectionPos();
        ofPoint p2 = this->getInput()->getWireConnectionPos();
        ofPoint p3 = getWireControlPoint(p1, p2, this->getOutput()->getDist(this->getInput()));
        drawWire(p1, p2, p3);
    }
}

ofPoint Wire::getWireControlPoint(ofPoint p1, ofPoint p2, float dist)
{
    ofPoint slump;
    slump.y = (1.0 - wiresTension) * (150.0 + 1.0*dist);
    ofPoint p3 = (p1 + p2)/2 + slump;
    return p3;

}

void Wire::drawWire(ofPoint p1, ofPoint p2, ofPoint p3)
{
    ofPushStyle();
    ofPushMatrix();
    
    wireGraphics.setColor(232, 181, 54);
    wireGraphics.setLineWidth(5);
    wireGraphics.noFill();
    wireGraphics.bezier(p1.x, p1.y, p3.x, p3.y, p3.x, p3.y, p2.x, p2.y);
    wireGraphics.fill();
    wireGraphics.circle(p1.x, p1.y, connectionRadius);
    wireGraphics.circle(p2.x, p2.y, connectionRadius);
    
    ofPopMatrix();
    ofPopStyle();
}

void Wire::drawCurrentWire(ofPoint p)
{
    ofPoint op = this->getOutput()->getWireConnectionPos();
    float dist = ofDist(op.x, op.y, p.x, p.y);
    ofPoint out = this->getOutput()->getWireConnectionPos();
    ofPoint controlPoint = getWireControlPoint(out, p, dist);
    drawWire(out, p, controlPoint);
}

WireConnection * Wire::getInput()
{
    return input;
}

WireConnection * Wire::getOutput()
{
    return output;
}

void Wire::setInput(WireConnection * in)
{
    input = in;
}

void Wire::setOutput(WireConnection * out)
{
    output = out;
}


float Wire::getDistance()
{
    return ofDist(input->getWireConnectionPos().x, input->getWireConnectionPos().y, output->getWireConnectionPos().x, output->getWireConnectionPos().y);
}

