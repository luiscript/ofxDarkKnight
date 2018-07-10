#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    CGRect thisMonitorArea = CGDisplayBounds(CGMainDisplayID());
    int thisWidth = CGRectGetWidth(thisMonitorArea);
    int thisHeight = CGRectGetHeight(thisMonitorArea);
    
    ofGLFWWindowSettings settings;
    settings.resizable = true;
    settings.width = 2*thisWidth;
    settings.height = 2*thisHeight;
    settings.windowMode = OF_WINDOW;
    
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
    shared_ptr<ofApp> mainApp(new ofApp);
    
    mainApp->mainWindow = mainWindow;
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();

}
