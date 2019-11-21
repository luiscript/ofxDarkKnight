//
//  DKFXAntiAliasing.h
//  batmapp
//
//  Created by luiscript on 20/11/19.
//

#include "DKModule.hpp"

class DKFXTiltShiftH : public DKModule
{
private:
    bool gotChain;
    ofShader shader;
    float h;
    float r;
public:
    void setup()
    {
        h = 2.0/512.0;
        r = 0.5;
        
        string fragShaderSrc = STRINGIFY(
                                         uniform sampler2DRect tDiffuse;
                                         uniform float h;
                                         uniform float r;
                                         
                                         void main() {
                                             vec2 vUv = gl_TexCoord[0].st;
                                             vec4 sum = vec4( 0.0 );
                                             
                                             float hh = h * abs( r - vUv.y );
                                             
                                             sum += texture2DRect( tDiffuse, vec2( vUv.x - 4.0 * hh, vUv.y ) ) * 0.051;
                                             sum += texture2DRect( tDiffuse, vec2( vUv.x - 3.0 * hh, vUv.y ) ) * 0.0918;
                                             sum += texture2DRect( tDiffuse, vec2( vUv.x - 2.0 * hh, vUv.y ) ) * 0.12245;
                                             sum += texture2DRect( tDiffuse, vec2( vUv.x - 1.0 * hh, vUv.y ) ) * 0.1531;
                                             sum += texture2DRect( tDiffuse, vec2( vUv.x, vUv.y ) ) * 0.1633;
                                             sum += texture2DRect( tDiffuse, vec2( vUv.x + 1.0 * hh, vUv.y ) ) * 0.1531;
                                             sum += texture2DRect( tDiffuse, vec2( vUv.x + 2.0 * hh, vUv.y ) ) * 0.12245;
                                             sum += texture2DRect( tDiffuse, vec2( vUv.x + 3.0 * hh, vUv.y ) ) * 0.0918;
                                             sum += texture2DRect( tDiffuse, vec2( vUv.x + 4.0 * hh, vUv.y ) ) * 0.051;
                                             
                                             gl_FragColor = sum;
                                         }
                                         );
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
        
        addInputConnection(DKConnectionType::DK_CHAIN);
        addChainOutputConnection(DKConnectionType::DK_CHAIN);
    }
    void addModuleParameters()
    {
        addSlider("h", h, 0.0, 1.0, 2.0/512.0, 6);
        addSlider("r", r, 0.0, 1000.0, 0.5, 6);
    }
    void render(ofFbo& readFbo, ofFbo& writeFbo)
    {
        writeFbo.begin();
        ofClear(0,0,0,0);
        shader.begin();
        shader.setUniformTexture("tDiffuse", readFbo.getTexture(), 1);
        shader.setUniform2f("resolution", 1.f, 1.f);
        shader.setUniform1f("h", h);
        shader.setUniform1f("r", r);
        //texturedQuad(0,0, 1920, 1080);
        readFbo.draw(0,0);
        shader.end();
        writeFbo.end();
    }
    void texturedQuad(float x, float y, float width, float height, float s = 1, float t = 1)
    {
        // TODO: change to triangle fan/strip
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(x, y, 0);
        
        glTexCoord2f(s, 0);
        glVertex3f(x + width, y, 0);
        
        glTexCoord2f(s, t);
        glVertex3f(x + width, y + height, 0);
        
        glTexCoord2f(0, t);
        glVertex3f(x, y + height, 0);
        glEnd();
    }
};

