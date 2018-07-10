#include "ofApp.h"

void ofApp::setup()
{
    ofSetVerticalSync(true);
    manager.mainWindow = mainWindow;
    manager.setup();
    ofBackground(17,17,17);
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
