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

class DKFXRotate : public DKModule
{
private:
    float x;
    float y;
    float z;
    float rotation;
    ofShader shader;
public:
    void setup()
    {
        
        
        string shaderSource = STRINGIFY
       (
        uniform float rotation;
        uniform float x;
        uniform float y;
        uniform float z;
        uniform vec2 u_resolution;
        uniform sampler2DRect tex1;
         
        mat2 rotate(float angle){
            return mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
        }

        void main(){
            
            vec2 coord = gl_FragCoord.xy / u_resolution;
            vec3 color = vec3(0.0);
            
            coord -= vec2(0.5);
            coord = rotate(rotation) * coord;
            coord += vec2(0.5);
            
            vec4 texturePixels = texture2DRect(tex1, coord);
            
            color += texturePixels.rgb;
            
            gl_FragColor = vec4(color, 1.0);
        });
        rotation = 0;
        x = y = z = 0;
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderSource);
        shader.linkProgram();
        
        addInputConnection(DKConnectionType::DK_CHAIN);
        addChainOutputConnection(DKConnectionType::DK_CHAIN);
    }
    void addModuleParameters()
    {
        addSlider("rotation", rotation, 0, 360, 0);
        addSlider("x", x, 0.0, 1.0, 0.0);
        addSlider("y", y, 0.0, 1.0, 0.0);
        addSlider("z", z, 0.0, 1.0, 0.0);
    }
    void render(ofFbo& readFbo, ofFbo& writeFbo)
    {
        writeFbo.begin();
        shader.begin();
        shader.setUniform2f("u_resolution", getModuleWidth(), getModuleHeight());
        shader.setUniform1f("x", x);
        shader.setUniform1f("y", y);
        shader.setUniform1f("z", z);
        shader.setUniform1f("rotation", rotation);
        
        shader.setUniformTexture("tex1", readFbo.getTextureReference(), 1);
        readFbo.draw(0, 0);
        shader.end();
        writeFbo.end();
    }
};

