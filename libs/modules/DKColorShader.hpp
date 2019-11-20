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

#include "DKModule.hpp"

class DKColorShader : public DKModule
{
private:
	float red;
	float green;
	float blue;
    ofShader shader;
public:
	void init()
	{
		red = green = blue = 1.0;
		shader.load("Shaders/ColorShader");
	}

    void drawChain()
    {
        if (false)
        {
            ofPushStyle();
            ofEnableAlphaBlending();
//            chain->readFbo->bind();
//            chain->writeFbo->begin();
            ofClear(0, 0, 0, 0);
            shader.begin();
            shader.setUniform1f("r", red);
            shader.setUniform1f("g", green);
            shader.setUniform1f("b", blue);
            
            //shader.setUniformTexture("text1", chain->readFbo->getTexture(), 1);
            //chain->readFbo->draw(0, 0);
            
            shader.end();
//            chain->writeFbo->end();
//
//            chain->readFbo->unbind();
            
            ofDisableAlphaBlending();
            ofPopStyle();
            
            //chain->readFbo = chain->writeFbo;
        }
        
    }

	void addModuleParameters()
	{
		gui->addSlider("red", 0.0, 1.0, 1.0)->setPrecision(4)->bind(red);
		gui->addSlider("green", 0.0, 1.0, 1.0)->setPrecision(4)->bind(green);
		gui->addSlider("blue", 0.0, 1.0, 1.0)->setPrecision(4)->bind(blue);
	}
	
};
