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

class DKFXAntiAliasing : public DKModule
{
private:
    bool gotChain;
    ofShader shader;
    float mn;
    float mx;
    float mt;
public:
    void setup()
    {
        mn = 1.0/128.0;
        mx = 8.0;
        mt = 1.0/8.0;

     string fragShaderSrc = STRINGIFY(
      uniform SAMPLER_TYPE tDiffuse;
      uniform vec2 resolution;
      varying vec2 vUv;
      
      float FXAA_REDUCE_MIN = 0.1471;
      float FXAA_REDUCE_MUL = 1.0/8.0;
      float FXAA_SPAN_MAX = 8.0;
      
      void main() {
          
          vec3 rgbNW = TEXTURE_FN( tDiffuse, ( gl_FragCoord.xy + vec2( -1.0, -1.0 ) ) * resolution ).xyz;
          vec3 rgbNE = TEXTURE_FN( tDiffuse, ( gl_FragCoord.xy + vec2( 1.0, -1.0 ) ) * resolution ).xyz;
          vec3 rgbSW = TEXTURE_FN( tDiffuse, ( gl_FragCoord.xy + vec2( -1.0, 1.0 ) ) * resolution ).xyz;
          vec3 rgbSE = TEXTURE_FN( tDiffuse, ( gl_FragCoord.xy + vec2( 1.0, 1.0 ) ) * resolution ).xyz;
          vec4 rgbaM  = TEXTURE_FN( tDiffuse,  gl_FragCoord.xy  * resolution );
          vec3 rgbM  = rgbaM.xyz;
          float opacity  = rgbaM.w;
          
          vec3 luma = vec3( 0.299, 0.587, 0.114 );
          
          float lumaNW = dot( rgbNW, luma );
          float lumaNE = dot( rgbNE, luma );
          float lumaSW = dot( rgbSW, luma );
          float lumaSE = dot( rgbSE, luma );
          float lumaM  = dot( rgbM,  luma );
          float lumaMin = min( lumaM, min( min( lumaNW, lumaNE ), min( lumaSW, lumaSE ) ) );
          float lumaMax = max( lumaM, max( max( lumaNW, lumaNE) , max( lumaSW, lumaSE ) ) );
          
          vec2 dir;
          dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
          dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
          
          float dirReduce = max( ( lumaNW + lumaNE + lumaSW + lumaSE ) * ( 0.25 * FXAA_REDUCE_MUL ), FXAA_REDUCE_MIN );
          
          float rcpDirMin = 1.0 / ( min( abs( dir.x ), abs( dir.y ) ) + dirReduce );
          dir = min( vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
                    max( vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
                        dir * rcpDirMin)) * resolution;
          
          vec3 rgbA = 0.5 * (
                             TEXTURE_FN( tDiffuse, gl_FragCoord.xy  * resolution + dir * ( 1.0 / 3.0 - 0.5 ) ).xyz +
                             TEXTURE_FN( tDiffuse, gl_FragCoord.xy  * resolution + dir * ( 2.0 / 3.0 - 0.5 ) ).xyz );
          
          vec3 rgbB = rgbA * 0.5 + 0.25 * (
                                           TEXTURE_FN( tDiffuse, gl_FragCoord.xy  * resolution + dir * -0.5 ).xyz +
                                           TEXTURE_FN( tDiffuse, gl_FragCoord.xy  * resolution + dir * 0.5 ).xyz );
          
          float lumaB = dot( rgbB, luma );
          
          if ( ( lumaB < lumaMin ) || ( lumaB > lumaMax ) ) {
              
              gl_FragColor = vec4( rgbA, opacity );
              
          } else {
              
              gl_FragColor = vec4( rgbB, opacity );
              
          }
          
      }
                                                                          
        );
        ostringstream oss;
        oss << "#version 120" << endl;
        if (true)
        {
            oss << "#define SAMPLER_TYPE sampler2DRect" << endl;
            oss << "#define TEXTURE_FN texture2DRect" << endl;
            oss << fragShaderSrc;
        }
        else
        {
            oss << "#define SAMPLER_TYPE sampler2D" << endl;
            oss << "#define TEXTURE_FN texture2D" << endl;
            oss << fragShaderSrc;
        }
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, oss.str());
        shader.linkProgram();
        addInputConnection(DKConnectionType::DK_CHAIN);
        addChainOutputConnection(DKConnectionType::DK_CHAIN);
    }
    void render(ofFbo& readFbo, ofFbo& writeFbo)
    {
        writeFbo.begin();
        ofClear(0,0,0,0);
        shader.begin();
        shader.setUniformTexture("tDiffuse", readFbo.getTexture(), 1);
        shader.setUniform2f("resolution", 1.f, 1.f);
        texturedQuad(0,0, getModuleWidth(), getModuleHeight());
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
