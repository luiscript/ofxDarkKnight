//
//  mixer.hpp
//  batmapp
//
//  Created by luiscript on 20/12/18.
//

#ifndef mixer_h
#define mixer_h

#include "module.hpp"
#include "wireConnection.hpp"
#include "unordered_map"

class Mixer : public Module
{
private:
    int blendMode;
    ofFbo compositionFbo;
    ofFbo * fboA;
    ofFbo * fboB;
    WireConnection * extraInput;
    unordered_map<string, int> blendModes;
    bool drawOutput;
public:
    void setup()
    {
        int x = gui->getPosition().x - 17;
        int y = gui->getPosition().y + 43;
        
        extraInput = new WireConnection;
        extraInput->setup(ofPoint(x,y), "fboInputB");
        
        blendModes.insert({"DISABLED" , OF_BLENDMODE_DISABLED});
        blendModes.insert({"ALPHA" , OF_BLENDMODE_ALPHA});
        blendModes.insert({"ADD" , OF_BLENDMODE_ADD});
        blendModes.insert({"SUBTRACT" , OF_BLENDMODE_SUBTRACT});
        blendModes.insert({"MULTIPLY" , OF_BLENDMODE_MULTIPLY});
        blendModes.insert({"SCREEN" , OF_BLENDMODE_SCREEN});
        
        drawOutput = false;
        
        compositionFbo.allocate(getModuleWidth(), getModuleHeight(), GL_RGBA, 4);
        compositionFbo.begin();
        ofClear(0,0,0,0);
        compositionFbo.end();
    };
    void update()
    {
        int x = gui->getPosition().x - 17;
        int y = gui->getPosition().y + 43;
        extraInput->setWireConnectionPos(ofPoint(x,y));
    };
    void draw()
    {
        if (drawOutput) {
            compositionFbo.begin();
            fboA.draw();
            ofEnableBlendMode(blendMode);
            fboB.draw();
            ofDisableBlendMode();
            compositionFbo.end();
        }
    };
    void addModuleParameters()
    {
        
    };
    void drawMasterOutput()
    {
        
    };
}

#endif /* mixer_h */
