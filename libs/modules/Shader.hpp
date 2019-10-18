/*
 Copyright (C) 2019 Luis Fernando García [http://luiscript.com]
 
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

#ifndef Shader_hpp
#define Shader_hpp

#include "module.hpp"
#include "ofxAutoReloadedShader.h"

#define STRINGIFY(A) #A

class Shader : public Module{
private:
    bool drawFbo;
    bool gotTexture;
	bool loaded;
    ofFbo internalFbo;
    ofFbo * texture;
    ofShader shader;
    int numParameters;
    ofxAutoReloadedShader autoShader;
    ofFbo fbo;
    string parameterName;
    float min;
    float max;
    int precision;
    ofxDatGuiFolder * params;
    unordered_map<string, float*> floatParameters;
    unordered_map<string, int*> intParameters;
public:
    void setup();
    void update();
    void draw();
    void addModuleParameters();
    ofFbo * getFbo();
    void setFbo(ofFbo *);
    void addParameter(ofxDatGuiButtonEvent);
    void onParameterNameChange(ofxDatGuiTextInputEvent);
    void onParameterMinChange(ofxDatGuiTextInputEvent);
    void onParameterMaxChange(ofxDatGuiTextInputEvent);
    void onParameterPrecisionChange(ofxDatGuiTextInputEvent);
	void onShaderSettingsButtonPress(ofxDatGuiButtonEvent);
//    void onOuputControlChange(ofxDatGuiTextInputEvent);
//    void onOutputChannelChange(ofxDatGuiTextInputEvent);
};

#endif /* Shader_hpp */
