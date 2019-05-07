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

#include "Shader.hpp"

void Shader::setup()
{
    autoShader.load("Shaders/EmptyShader");
    fbo.allocate(getModuleWidth(), getModuleHeight());
    fbo.begin();
    ofClear(0, 0, 0, 0);
    fbo.end();
    
    addOutputConnection(ConnectionType::DK_FBO);
    //addInputConnection(ConnectionType::DK_FBO);
    
}

void Shader::update()
{
    
}

void Shader::draw()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    ofClear(0, 0, 0, 0);
    autoShader.begin();
    autoShader.setUniform1f("u_time", ofGetElapsedTimef() );
    ofDrawRectangle(0, 0, getModuleWidth(), getModuleHeight());
    autoShader.end();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
}


void Shader::addModuleParameters()
{
//    ofxDatGuiFolder * shaderSettings = gui->addFolder("SHADER SETTINGS");
//    ofxDatGuiFolder * addParameter = gui->addFolder("ADD PARAMETER");
//    ofxDatGuiTextInput * parameterName = addParameter->addTextInput("Name", "parameter");
//    ofxDatGuiTextInput * parameterMin = addParameter->addTextInput("Min value", "0");
//    ofxDatGuiTextInput * parameterMax = addParameter->addTextInput("Max value", "1");
//    ofxDatGuiTextInput * parameterPrecision = addParameter->addTextInput("Precision", "2");
//
//    ofxDatGuiButton * addParameterButton = addParameter->addButton("ADD");
//    addParameterButton->setLabelAlignment(ofxDatGuiAlignment::CENTER);
//    addParameterButton->onButtonEvent(this, &Shader::addParameter);
    
//    ofxDatGuiTextInput * channelComponent = config->addTextInput("Channel", "1");
//    channelComponent->onTextInputEvent(this, &Shader::onOutputChannelChange);
//    
//    ofxDatGuiTextInput * controlComponent = config->addTextInput("Control", "0");
//    controlComponent->onTextInputEvent(this, &Shader::onOuputControlChange);
}


void Shader::addParameter(ofxDatGuiButtonEvent e)
{
    
}

ofFbo * Shader::getFbo()
{
    return &fbo;
}
