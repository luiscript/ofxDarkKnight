#include "ofApp.h"

void ofApp::setup()
{
    ofSetVerticalSync(true);
    manager.mainWindow = mainWindow;
    ofBackground(17,17,17);
    
    unordered_map<string, Module*> modulesPool;
    modulesPool = {
        { "ABLETON LINK", new ofxDarkKnightAbletonLink },
        { "MAPPING TOOLS", new ofxDarkKnightMapping },
        { "MIDI CONTROLLER", new ofxDarkKnightMidi },
        { "OSC CONTROLLER", new ofxDarkKnightOsc },
        { "SYPHON SERVER", new ofxDarkKnightSyphon },
        { "SYPHON CLIENT", new DarkKnightSyphonClient },
        { "POST PROCESSING FX", new DarkKnightPostProcessing },
        { "PREVIEW", new Preview },
        { "SCREEN OUTPUT", new ScreenOutput },
        { "SKETCH POOL 1", new Basic },
        { "SKETCH POOL 2", new Basic }
    };
    manager.setup(&modulesPool);
}

void ofApp::update()
{
    manager.update();
}

void ofApp::draw()
{
    manager.draw();
}


void ofApp::exit()
{
    manager.close();
}
