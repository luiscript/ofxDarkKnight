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


#define STRINGIFY(A) #A

#include "module.hpp"

class DKColorInverter : public Module
{
private:
	ofShader shader;
	ofFbo* fbo;
	ofFbo* newFbo;
	bool gotTexture;
	float mix;
public:
	void setup()
	{
		fbo = nullptr;

		newFbo = new ofFbo();
		newFbo->allocate(getModuleWidth(), getModuleHeight());
		newFbo->begin();
		ofClear(0, 0, 0, 0);
		newFbo->end();

		gotTexture = false;

		shader.load("Shaders/InverterShader");

		addInputConnection(ConnectionType::DK_FBO);
		addOutputConnection(ConnectionType::DK_FBO);
	}

	void update()
	{

	}

	void draw()
	{
		ofPushStyle();
		ofEnableAlphaBlending();
		if (gotTexture)
		{
			fbo->bind();
		}

		newFbo->begin();
		ofClear(0, 0, 0, 0);
		shader.begin();

		if (gotTexture)
		{
			shader.setUniform1f("mix", mix);
			shader.setUniformTexture("text1", fbo->getTextureReference(), 1);
			fbo->draw(0, 0);
		}

		shader.end();
		newFbo->end();
		ofDisableAlphaBlending();
		ofPopStyle();

		if (gotTexture)
		{
			fbo->unbind();
		}
	}

	void addModuleParameters()
	{
		gui->addSlider("mix", 0.0, 1.0, 1.0)->setPrecision(4)->bind(mix);
	}

	void setFbo(ofFbo* fboptr)
	{
		gotTexture = fboptr != nullptr;
		fbo = fboptr;
	}

	ofFbo* getFbo()
	{
		return  newFbo;
	}
};