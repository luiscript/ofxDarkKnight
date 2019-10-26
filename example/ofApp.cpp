#include "ofApp.h"

void ofApp::setup()
{
    ofSetVerticalSync(true);
    manager.mainWindow = mainWindow;
    ofBackground(17,17,17);
    
    unordered_map<string, Module*> modulesPool;
    modulesPool = {
       	{ "ABLETON LINK", new ofxDarkKnightAbletonLink },
		{ "COLOR INVERTER", new DKColorInverter},
		{ "COLOR SHADER", new ColorShader},
		{ "INVERTER", new ParamInverter},
		{ "LIGHT", new DarkKnightLight},
		{ "LIVE SHADER", new Shader},
		{ "LFO", new LfoSlider },
		{ "MAPPING GRID", new ofxDarkKnightMapping },
		{ "MIDI CLOCK IN", new DarkKnightMidiClockIn },
		{ "MIDI CONTROL IN", new DarkKnightMidiControlIn },
		{ "MIDI CONTROL OUT", new DarkKnightMidiControlOut },
		{ "MIXER", new Mixer },
		{ "OSC CLIENT", new DarkKnightOscClient },
		{ "OSC SERVER", new DarkKnightOscServer },
		{ "PERLIN NOISE", new NoiseSlider},
		{ "POST PROCESSING FX", new DarkKnightPostProcessing },
		{ "PREVIEW", new Preview },
		{ "SCREEN OUTPUT", new ScreenOutput },
		{ "SKETCH POOL", new MediaPool },
		{ "SKETCH POOL 1", new Basic },
		{ "SKETCH POOL 2", new Basic },
		{ "SPOUT SERVER", new DarkKnightTextureServer },
		{ "SPOUT CLIENT", new DarkKnightTextureClient },
		{ "VIDEO RECORDER", new DarkKnightVideoRecorder},
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
