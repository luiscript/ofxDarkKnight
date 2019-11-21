//
//  DKChainFX.hpp
//  BatMapp
//
//  Created by luiscript on 17/11/19.
//

#ifndef DKChain_h
#define DKChain_h

#include "DKModule.hpp"

class DKChain : public DKModule
{
public:
    void setup();
    void update();
    void draw();
    ofFbo* getFbo();
    void setFbo(ofFbo*);
private:

    void processChain(ofFbo&, ofFbo&, DKModule*);
    unsigned currentReadFbo;
    unsigned numFx = 0;
    bool gotTexture;
    
    ofFbo raw;
    ofFbo* fboIn;
    ofFbo pingPong[2];
};

#endif /* DKChain_h */
