//
//  DKMixer.cpp
//  BatMapp
//
//  Created by luiscript on 17/11/19.
//

#include "DKMixer.hpp"

void DKMixer::setup()
{
    ofFbo::Settings s;
    s.width = getModuleWidth();
    s.height = getModuleHeight();
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    
    for (int i = 0; i < 2; ++i)
    {
        pingPong[i].allocate(s);
    }
    
    raw.allocate(s);
    
    numFx = currentReadFbo = 0;
    
    addOutputConnection(DKConnectionType::DK_FBO);
    addInputConnection(DKConnectionType::DK_EMPTY);
    addInputConnection(DKConnectionType::DK_EMPTY);
    addInputConnection(DKConnectionType::DK_MULTI_FBO, 0);
    addInputConnection(DKConnectionType::DK_EMPTY);
    addInputConnection(DKConnectionType::DK_MULTI_FBO, 1);
    addInputConnection(DKConnectionType::DK_EMPTY);
    addChainOutputConnection(DKConnectionType::DK_CHAIN);
    
    blendMode = 0;
    chainModule = nullptr;
    alphaMaster = alpha1 = alpha2 = 1.0;
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, psBlendFragShaderGL2);
    shader.linkProgram();

    fboInputs[0] = nullptr;
    fboInputs[1] = nullptr;
}

void DKMixer::update()
{
    numFx = 0;
    processChain(pingPong[currentReadFbo], pingPong[1-currentReadFbo], chainModule);
}

void DKMixer::draw()
{
    
    raw.begin();
    ofClear(0,0,0,0);
    shader.begin();
    
    int read1 = 0, read2 = 0;
    
    if(fboInputs[0] != nullptr)
    {
        shader.setUniformTexture("base", fboInputs[0]->getTextureReference(), 1);
        read1 = 1;
    }
    
    if(fboInputs[1] != nullptr)
    {
        shader.setUniformTexture("blendTgt", fboInputs[1]->getTextureReference(), 2);
        read2 = 1;
    }
    
    
    shader.setUniform1f("alpha1", alpha1);
    shader.setUniform1f("alpha2", alpha2);
    shader.setUniform1f("master", alphaMaster);
    shader.setUniform1i("mode", blendMode);
    shader.setUniform1i("read1", read1);
    shader.setUniform1i("read2", read2);
    
    drawPlane();
    
    shader.end();
    raw.end();
    
    numFx = 0;
    
    processChain(raw, pingPong[1-currentReadFbo], chainModule);
    
    if(numFx > 0)
    {
        raw.begin();
        ofClear(0,0,0,0);
        pingPong[currentReadFbo].draw(0,0);
        raw.end();
    }
    
}

void DKMixer::addModuleParameters()
{
    addSlider("Master", alphaMaster, 0.0, 1.0, 1.0);
    gui->addLabel("1");
    addSlider("1 Alpha", alpha1, 0.0, 1.0, 1.0);
    gui->addLabel("2");
    addSlider("2 Alpha", alpha2, 0.0, 1.0, 1.0);
    
    guiLabel = gui->addLabel(getBlendName(blendMode));
    guiLabel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
   
    auto matrix = gui->addMatrix("", 24, true);
    matrix->setRadioMode(true);
    matrix->onMatrixEvent(this, &DKMixer::onBlendModeChange);
    matrix->setSelected({0});

}

void DKMixer::processChain(ofFbo& read, ofFbo& write, DKModule* cModule)
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
    }
}

ofFbo* DKMixer::getFbo()
{
    return &raw;
}

void DKMixer::setFbo(ofFbo * fboPtr, int fboIndex)
{
    
    if(fboIndex == 0 || fboIndex == 1)
    {
        fboInputs[fboIndex] = fboPtr;
    }
    if(fboPtr == nullptr)
    {
        cout << "clear raw: " << fboIndex << endl;
        raw.begin();
        ofClear(0,0,0,0);
        raw.end();
    }
}

string DKMixer::getBlendName(int blendMode)
{
    string blendName = "";
    if (blendMode == 0)
    {
        blendName = "Normal";
    }
    else if (blendMode == 1)
    {
        blendName = "Multiply";
    }
    else if (blendMode == 2)
    {
        blendName = "Average";
    }
    else if (blendMode == 3)
    {
        blendName = "Add";
    }
    else if (blendMode == 4)
    {
        blendName = "Substract";
    }
    else if (blendMode == 5)
    {
        blendName = "Difference";
    }
    else if (blendMode == 6)
    {
        blendName = "Negation";
    }
    else if (blendMode == 7)
    {
        blendName = "Exclusion";
    }
    else if (blendMode == 8)
    {
        blendName = "Screen";
    }
    else if (blendMode == 9)
    {
        blendName = "Overlay";
    }
    else if (blendMode == 10)
    {
        blendName = "Soft Light";
    }
    else if (blendMode == 11)
    {
        blendName = "Hard Light";
    }
    else if (blendMode == 12)
    {
        blendName = "Color Dodge";
    }
    else if (blendMode == 13)
    {
        blendName = "Color Burn";
    }
    else if (blendMode == 14)
    {
        blendName = "Linear Light";
    }
    else if (blendMode == 15)
    {
        blendName = "Vivid Light";
    }
    else if (blendMode == 16)
    {
        blendName = "Pin Light";
    }
    else if (blendMode == 17)
    {
        blendName = "Hard Mix";
    }
    else if (blendMode == 18)
    {
        blendName = "Reflect";
    }
    else if (blendMode == 19)
    {
        blendName = "Glow";
    }
    else if (blendMode == 20)
    {
        blendName = "Phoenix";
    }
    else if (blendMode == 21)
    {
        blendName = "Hue";
    }
    else if (blendMode == 22)
    {
        blendName = "Saturation";
    }
    else if (blendMode == 23)
    {
        blendName = "Color";
    }
    else if (blendMode == 24)
    {
        blendName = "Luminosity";
    }
    return blendName;
}

void DKMixer::onBlendModeChange(ofxDatGuiMatrixEvent e)
{
    blendMode = e.child;
    guiLabel->setLabel(getBlendName(blendMode));
}
