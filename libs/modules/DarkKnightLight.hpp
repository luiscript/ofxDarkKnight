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


#include "module.hpp"

class DarkKnightLight : public Module
{
private:
    int ambientColorRed;
    int ambientColorGreen;
    int ambientColorBlue;
    
    int diffuseColorRed;
    int diffuseColorGreen;
    int diffuseColorBlue;
    
    int specularColorRed;
    int specularColorGreen;
    int specularColorBlue;
    
    float attenuationX;
    float attenuationY;
    float attenuationZ;
    
    int positionX;
    int positionY;
    int positionZ;
    
    int rotationDegX;
    int rotationDegY;
    int rotationDegZ;
    ofLight light;
    
public:
    void setup()
    {
        ambientColorRed = 255;
        ambientColorGreen = 0;
        ambientColorBlue = 0;
        
        diffuseColorRed = 0;
        diffuseColorGreen = 255;
        diffuseColorBlue = 0;
        
        specularColorRed = 0;
        specularColorGreen = 0;
        specularColorBlue = 255;
        
        attenuationX = 1.0;
        attenuationY = 0.00001;
        attenuationZ = 0.00001;
        
        positionX = 0;
        positionY = 0;
        positionZ = 400;
        
        rotationDegX = 0;
        rotationDegY = 0;
        rotationDegZ = 0;
       
		light.setup();
        light.setPosition(positionX, positionY, positionZ);
        light.setDiffuseColor(ofColor(
                                      diffuseColorRed,
                                      diffuseColorGreen,
                                      diffuseColorBlue));
        light.setAmbientColor(ofColor(
                                      ambientColorRed,
                                      ambientColorGreen,
                                      ambientColorBlue));
        
        addOutputConnection(ConnectionType::DK_LIGHT);
    }
    
    void update()
    {
		light.setPosition(positionX, positionY, positionZ);
		light.setDiffuseColor(ofColor(
			diffuseColorRed,
			diffuseColorGreen,
			diffuseColorBlue));
		light.setAmbientColor(ofColor(
			ambientColorRed,
			ambientColorGreen,
			ambientColorBlue));  
    }
    
    void draw()
    {
       
    }
    
    void addModuleParameters()
    {
        gui->addSlider("position x", -getModuleWidth(), getModuleWidth(), getModuleWidth()/2)->bind(positionX);
        gui->addSlider("position Y", -getModuleHeight(), getModuleHeight(), getModuleWidth()/2)->bind(positionY);
        gui->addSlider("position Z", 0, getModuleWidth(), getModuleWidth()/2)->bind(positionZ);
        
        gui->addSlider("ambient red", 0, 255, 0)->bind(ambientColorRed);
        gui->addSlider("ambient green", 0, 255, 0)->bind(ambientColorGreen);
        gui->addSlider("ambient blue", 0, 255, 0)->bind(ambientColorBlue);
        
        gui->addSlider("diffuse red", 0, 255, 0)->bind(diffuseColorRed);
        gui->addSlider("diffuse green", 0, 255, 0)->bind(diffuseColorGreen);
        gui->addSlider("diffuse blue", 0, 255, 0)->bind(diffuseColorBlue);
   
    }   

	ofLight* getLight()
	{
		return &light;
	}
};
