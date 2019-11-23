/*
 Copyright (C) 2019 Luis Fernando García Pérez [http://luiscript.com]
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef DKLua_h
#define DKLua_h

#include "DKModule.hpp"
#include "ofxLua.h"
#include "tinyfiledialogs.h"
#include "ofxThreadedFileDialog.h"

class DKLua : public DKModule, ofxLuaListener{
private:
    bool drawFbo;
    bool gotTexture;
    bool loaded;
    ofFbo internalFbo;
    ofFbo * texture;
    ofShader shader;
    int numParameters;
    ofFbo fbo;
    string parameterName;
    float min;
    float max;
    int precision;
    float noise;
    ofxDatGuiFolder * params;
    unordered_map<string, float*> floatParameters;
    unordered_map<string, int*> intParameters;
    
    //
    bool bWatchingFiles;
    bool filesChanged();
    bool loadShaderNextFrame;
    std::time_t getLastModified( ofFile& _file );
    int lastTimeCheckMillis;
    int millisBetweenFileCheck;
    ofFile scriptFile;
    vector< time_t > fileChangedTimes;
    ofxThreadedFileDialog fileDialog;
public:
    
    void setup();
    void update();
    void draw();
    void addModuleParameters();
    ofFbo * getFbo();
    void addParameter(ofxDatGuiButtonEvent);
    void onParameterNameChange(ofxDatGuiTextInputEvent);
    void onParameterMinChange(ofxDatGuiTextInputEvent);
    void onParameterMaxChange(ofxDatGuiTextInputEvent);
    void onParameterPrecisionChange(ofxDatGuiTextInputEvent);
    void onSettingsButtonPress(ofxDatGuiButtonEvent);
    void onFileDialogResponse(ofxThreadedFileDialogResponse&);
    
    void exit();
    void unMount();
    
    // override the initialisation functions
    bool load(string shaderName );
    bool load(string vertName, string fragName, string geomName);
    
    void setMillisBetweenFileCheck( int _millis );
    
    void errorReceived(string& msg);
    
    // script control
    void reloadScript();
  
    ofxLua lua;
    string script;
    size_t currentScript;
    
    
    void keyPressed(int key){};
    void mouseMoved(int x, int y){};
    void mouseDragged(int x, int y, int button){};
    void mousePressed(int x, int y, int button){};
    void mouseReleased(int x, int y, int button){};
};

#endif /* DKLua_h */
