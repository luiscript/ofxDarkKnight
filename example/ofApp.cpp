#include "ofApp.h"

void ofApp::setup()
{
    ofSetVerticalSync(true);
    app.mainWindow = mainWindow;
    ofBackground(17,17,17);

    app.factory["ABLETON LINK"] = &createInstance<ofxDarkKnightAbletonLink>;
	app.factory["COLOR INVERTER"] = &createInstance<DKColorInverter>;
	app.factory["COLOR SHADER"] = &createInstance<ColorShader>;
	app.factory["INVERTER"] = &createInstance<ParamInverter>;
	app.factory["LIGHT"] = &createInstance<DarkKnightLight>;
	app.factory["LFO"] = &createInstance<LfoSlider>;
	app.factory["MAPPING GRID"] = &createInstance<ofxDarkKnightMapping>;
	app.factory["MIDI CLOCK IN"] = &createInstance<DarkKnightMidiClockIn>;
	app.factory["MIDI CONTROL IN"] = &createInstance<DarkKnightMidiControlIn>;
	app.factory["MIDI CONTROL OUT"] = &createInstance<DarkKnightMidiControlOut>;
	app.factory["MIXER"] = &createInstance<Mixer>;
	app.factory["OSC RECEIVER"] = &createInstance<DarkKnightOscClient>;
	app.factory["OSC SENDER"] = &createInstance<DarkKnightOscServer>;
	app.factory["PERLIN NOISE"] = &createInstance<NoiseSlider>;
	app.factory["POST PROCESSING FX"] = &createInstance<DarkKnightPostProcessing>;
	app.factory["PREVIEW"] = &createInstance<Preview>;
	app.factory["SCREEN OUTPUT"] = &createInstance<ScreenOutput>;
	app.factory["SKETCH POOL"] = &createInstance<MediaPool>;
	app.factory["SKETCH POOL: FREE"] = &createInstance<Basic>;
	app.factory["SPOUT SERVER"] = &createInstance<DarkKnightTextureServer>;
	app.factory["SPOUT CLIENT"] = &createInstance<DarkKnightTextureClient>;
	app.factory["VIDEO RECORDER"] = &createInstance<DarkKnightVideoRecorder>;
}

void ofApp::update()
{
    app.update();
}

void ofApp::draw()
{
    app.draw();
}


void ofApp::exit()
{
    app.close();
}
