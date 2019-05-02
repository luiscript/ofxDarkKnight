#include "ofApp.h"

void ofApp::setup()
{
    ofSetVerticalSync(true);
    manager.mainWindow = mainWindow;
    ofBackground(17,17,17);
    
    unordered_map<string, Module*> modulesPool;
    modulesPool = {
        { "ABLETON LINK", new ofxDarkKnightAbletonLink },
        { "MAPPING GRID", new ofxDarkKnightMapping },
        { "MIDI CONTROL IN", new DarkKnightMidiControlIn },
        { "MIDI CONTROL OUT", new DarkKnightMidiControlOut },
        { "MIXER", new Mixer },
        { "OSC CLIENT", new ofxDarkKnightOsc },
        { "PERLIN NOISE", new NoiseSlider},
        { "POST PROCESSING FX", new DarkKnightPostProcessing },
        { "PREVIEW", new Preview },
        { "SCREEN OUTPUT", new ScreenOutput },
        { "SKETCH POOL 1", new Basic },
        { "SKETCH POOL 2", new Basic },
        { "SYPHON SERVER", new ofxDarkKnightSyphon },
        { "SYPHON CLIENT", new DarkKnightSyphonClient },
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
