#pragma once

#include "ofMain.h"
#include "ofxDarkKnight.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
    
        ofxDarkKnight app;
    	shared_ptr<ofAppBaseWindow> mainWindow;
};
