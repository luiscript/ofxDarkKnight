#include "ofApp.h"

void ofApp::setup()
{
    ofSetVerticalSync(true);
    app.mainWindow = mainWindow;
    ofBackground(17,17,17);

    app.moduleList["ABLETON LINK"] = &moduleType<DKAbletonLink>;
    app.moduleList["CHAIN FX"] = &moduleType<DKChain>;
    app.moduleList["FX AA"] = &moduleType<DKFXAntiAliasing>;
    app.moduleList["FX INVERT"] = &moduleType<DKFXColorInv>;
    app.moduleList["FX MIRROR"] = &moduleType<DKFXMirror>;
    app.moduleList["FX RGB SUB"] = &moduleType<DKFXColorSub>;
    app.moduleList["FX ROTATE"] = &moduleType<DKFXRotate>;
    app.moduleList["FX TILT SHIFT H"] = &moduleType<DKFXTiltShiftH>;
    app.moduleList["INVERTER"] = &moduleType<DKSliderInverter>;
    app.moduleList["LIGHT"] = &moduleType<DKLight>;
    app.moduleList["LIVE SCRIPT"] = &moduleType<DKLua>;
    app.moduleList["LIVE SHADER"] = &moduleType<DKLiveShader>;
    app.moduleList["LFO"] = &moduleType<DKLfo>;
    app.moduleList["MAPPING GRID"] = &moduleType<DKMapping>;
    app.moduleList["MIDI CLOCK IN"] = &moduleType<DKMidiClockIn>;
    app.moduleList["MIDI CONTROL IN"] = &moduleType<DKMidiControlIn>;
    app.moduleList["MIDI CONTROL OUT"] = &moduleType<DKMidiControlOut>;
    app.moduleList["MIXER"] = &moduleType<DKMixer>;
    app.moduleList["OSC RECEIVER"] = &moduleType<DKOscClient>;
    app.moduleList["OSC SENDER"] = &moduleType<DKOscServer>;
    app.moduleList["PERLIN NOISE"] = &moduleType<DKPerlin>;
    app.moduleList["PREVIEW"] = &moduleType<DKPreview>;
    app.moduleList["PROJECT"] = &moduleType<DKConfig>;
    app.moduleList["SCREEN OUTPUT"] = &moduleType<DKScreenOutput>;
    app.moduleList["SKETCH POOL"] = &moduleType<DKMediaPool>;
    app.moduleList["SKETCH POOL: DEMO"] = &moduleType<Basic>;
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
