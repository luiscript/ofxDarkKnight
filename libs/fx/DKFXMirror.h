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

#include "DKModule.hpp"

class DKFXMirror : public DKModule
{
private:
    float vertical;
    float horizontal;
    bool gotChain;
    ofShader shader;
    
public:
    void setup()
    {
        vertical = horizontal = 0.5;
        shader.load("Shaders/MirrorShader");
        addInputConnection(DKConnectionType::DK_CHAIN);
        addChainOutputConnection(DKConnectionType::DK_CHAIN);
    }
    void addModuleParameters()
    {
        addSlider("vertical", vertical, 0.0, 1.0, 0.5);
        addSlider("horizontal", horizontal, 0.0, 1.0, 0.5);
    }
    void render(ofFbo& readFbo, ofFbo& writeFbo)
    {
        writeFbo.begin();
        ofClear(0,0,0,0);
        shader.begin();
        shader.setUniform2f("resolution", glm::vec2(getModuleWidth(), getModuleHeight()));
        shader.setUniform1f("vertical", vertical);
        shader.setUniform1f("horizontal", horizontal);
        shader.setUniformTexture("tex0", readFbo.getTextureReference(), 1);
        readFbo.draw(0,0);
        shader.end();
        writeFbo.end();
    }
};
