#pragma once

#include "ofMain.h"

#include "ofxDarkKnight.hpp"
#include "ofxDarkKnightMidi.hpp"
#include "ofxDarkKnightOsc.hpp"
#include "ofxDarkKnightAbletonLink.hpp"

#include "preview.hpp"
#include "basic.hpp"
#include "screenOutput.hpp"


#include "unordered_map"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    ofxDarkKnight manager;
    shared_ptr<ofAppBaseWindow> mainWindow;
    
};
