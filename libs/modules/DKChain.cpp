/*
 Copyright (C) 2020 Luis Fernando García Pérez [http://luiscript.com]

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

#include "DKChain.h"

void DKChain::setup()
{
    ofFbo::Settings s;
    s.width = getModuleWidth();
    s.height = getModuleHeight();
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;

    for (int i = 0; i < 2; i++)
    {
        pingPong[i].allocate(s);
    }
    
    raw.allocate(s);
    numFx = currentReadFbo = 0;

    addInputConnection(DKConnectionType::DK_FBO);
    addOutputConnection(DKConnectionType::DK_FBO);
    addChainOutputConnection(DKConnectionType::DK_CHAIN);
    
    chainModule = nullptr;
    fboIn = nullptr;
}

void DKChain::update()
{
    if(gotTexture)
    {
        numFx = 0;
        processChain(*fboIn, pingPong[1-currentReadFbo], chainModule);
    }
}

void DKChain::draw()
{
    raw.begin();
    ofClear(0,0,0,0);
    if(gotTexture)
    {
        pingPong[currentReadFbo].draw(0,0);
    }
    raw.end();
}

void DKChain::processChain(ofFbo& read, ofFbo& write, DKModule* cModule)
{
    if(cModule != nullptr)
    {
        cModule->render(read, write);
        currentReadFbo = 1 - currentReadFbo;
        if(cModule->getChainModule() != nullptr)
        {
            processChain(pingPong[currentReadFbo], pingPong[1-currentReadFbo], cModule->getChainModule());
        }
        numFx++;
    } else if(numFx == 0)
    {
        pingPong[currentReadFbo].begin();
        ofClear(0,0,0,0);
		if(fboIn != nullptr) fboIn->draw(0,0);
        pingPong[currentReadFbo].end();
    }
}

void DKChain::setFbo(ofFbo* fboptr)
{
    gotTexture = fboptr != nullptr;
    fboIn = fboptr;
}

ofFbo* DKChain::getFbo()
{
    return &raw;
}
