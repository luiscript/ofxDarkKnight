//
//  DKChain.cpp
//  BatMapp
//
//  Created by luiscript on 17/11/19.
//

#include "DKChain.h"

void DKChain::setup()
{
    ofFbo::Settings s;
    s.width = getModuleWidth();
    s.height = getModuleHeight();
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    //s.internalformat = GL_RGBA;
    //s.numSamples = 4;
    //s.numColorbuffers = 4;
    
    //raw.allocate(<#int width#>, <#int height#>)
    
    for (int i = 0; i < 2; ++i)
    {
        pingPong[i].allocate(s);
    }
    
//    s.useDepth = true;
//    s.depthStencilInternalFormat = GL_DEPTH_COMPONENT24;
//    s.depthStencilAsTexture = true;
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
    
    
//    raw.getTexture().draw(10, 10, 300, 300);
//    raw.getDepthTexture().draw(320, 10, 300, 300);
//    pingPong[currentReadFbo].draw(630, 10, 300, 300);
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
        fboIn->draw(0,0);
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
