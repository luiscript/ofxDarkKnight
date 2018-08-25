#include "ofApp.h"

void ofApp::setup()
{
    ofSetVerticalSync(true);
    manager.mainWindow = mainWindow;
    
    ofBackground(17,17,17);
    
    unordered_map<string, Module*> modulesPool;
    
    modulesPool = {
        { "PREVIEW", new Preview },
        { "SCREEN OUTPUT", new ScreenOutput },
        { "SKETCH POOL", new Basic }
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
