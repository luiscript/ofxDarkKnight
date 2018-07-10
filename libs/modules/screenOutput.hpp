//
//  screenOutput.hpp
//  luiscript
//
//  Created by luiscript on 10/01/18.
//
//

#ifndef screenOutput_hpp
#define screenOutput_hpp

#include "ofMain.h"
#include "ofxDatGui.h"
#include "module.hpp"

class ScreenOutput : public Module{
private:
    ofFbo * fbo;
    bool drawFbo = false;
    string serverName;
    vector<string> monitorsName;
    shared_ptr<ofAppBaseWindow> display;
    

public:
    void drawDisplay(ofEventArgs & args);
    void drawMasterOutput();
    void drawMasterInput();
    void setFbo(ofFbo *);
    void addModuleParameters();
    ofFbo * getFbo();
    void onVideoOutputChange(ofxDatGuiDropdownEvent);
    
    shared_ptr<ofAppBaseWindow> mainWindow;
};



#endif /* screenOutput_hpp */
