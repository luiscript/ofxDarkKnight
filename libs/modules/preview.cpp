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

#include "preview.hpp"

void Preview::setup()
{
    drawFbo = false;
    fbo = nullptr;
    scaleX = scaleY = 0.5;
}

void Preview::update()
{
    
}

void Preview::draw()
{
    if(drawFbo)
    {
        ofPushMatrix();
        ofTranslate(gui->getPosition().x, gui->getPosition().y + 30);
        ofScale(scaleX, scaleY);
        fbo->draw(0,0);
        ofPopMatrix();
    }
    
}

void Preview::setFbo(ofFbo * fboPtr)
{
    fbo  = fboPtr;
    drawFbo = fboPtr != nullptr;
    
    if(drawFbo)
    {
        //setModuleWidth(fbo->getWidth()/4);
        gui->setWidth(getModuleWidth()/2);
    }
    
    scaleX = 0.5;
    scaleY = 0.5;
}

ofFbo * Preview::getFbo()
{
    return fbo;
}


void Preview::drawMasterOutput()
{
    drawOutputConnection();
}

void Preview::drawMasterInput()
{
    drawInputConnection();
}

