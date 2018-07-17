#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    CGRect thisMonitorArea = CGDisplayBounds(CGMainDisplayID());
    int thisWidth = CGRectGetWidth(thisMonitorArea);
    int thisHeight = CGRectGetHeight(thisMonitorArea);
    
    ofGLFWWindowSettings settings;
    settings.resizable = true;
    settings.width = 1920;
    settings.height = 1080;
    settings.windowMode = OF_WINDOW;
    
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
    shared_ptr<ofApp> mainApp(new ofApp);
    
    mainApp->mainWindow = mainWindow;
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();

}
