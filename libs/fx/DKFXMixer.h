#include "DKModule.hpp"

class DKFXMixer : public DKModule
{
private:
    bool gotChain;
    ofShader shader;
    float alpha1;
    float alpha2;
public:
    void setup()
    {
        alpha1= alpha2 = 1.0;
       
        string fragShaderSrc =
        
        STRINGIFY
        (
        
        );
        
//        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
//        shader.linkProgram();
        
        addInputConnection(DKConnectionType::DK_CHAIN);
        addInputConnection(DKConnectionType::DK_FBO);
        addInputConnection(DKConnectionType::DK_EMPTY);
        addInputConnection(DKConnectionType::DK_FBO);
        addInputConnection(DKConnectionType::DK_EMPTY);
        addChainOutputConnection(DKConnectionType::DK_CHAIN);
    }
    void addModuleParameters()
    {
        gui->addLabel("Layer 1");
        addSlider("Alpha 1", alpha1, 0.0, 1.0, 1.0);
        gui->addLabel("Layer 2");
        addSlider("Alpha 2", alpha2, 0.0, 1.0, 1.0);
    }
    void render(ofFbo& readFbo, ofFbo& writeFbo)
    {
        writeFbo.begin();
        ofClear(0,0,0,0);
        shader.begin();
        shader.setUniform2f("resolution", 1.f, 1.f);
       
        readFbo.draw(0,0);
        shader.end();
        writeFbo.end();
    }
};


