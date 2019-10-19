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

#include "LfoSlider.hpp"

void LfoSlider::setup()
{
    icons.load("icons/lfoWaves.png");
    icons.resize(178, 13);
	
    wave = 0;
    time = 0;
    amplitude = 1;  
    offset = 0;
    result = 0;
}

void LfoSlider::update()
{
    
    switch(wave){
        case 0: {
            //sine wave
            result = (1 - ((1+cos (PI* (time*2)))*0.5 ));
            break;
        }
        case 1: {
            //linear wave
            int parity = ((int)floor(time*2))%2;
            if(parity==0){
                result = fmod(time, 1) * 2;
            }else{
                result = 2 - (fmod(time,1) * 2);
            }
            break;
        }
        case 2: {
            //square wave
            result = round (fmod (time,1));
            break;
        }
        case 3: {
            // in
            result = fmod (time*2,1);
            break;
        }
        case 4: {
            // out
            result = 1 - fmod (time*2,1);
            break;
        }
        case 5: {
            // random
            if((int) fmod(ceil((time-0.5)*2),2) == 1)
            {
                result = ofRandom(0,1);
            }
        }
    }
    result *= amplitude;
    result += offset;
}

void LfoSlider::draw()
{
    float size = moduleGuiWidth * 0.35 + 3;
    icons.draw(gui->getPosition().x + size, gui->getPosition().y + 32);
}

void LfoSlider::addModuleParameters()
{
    ofxDatGuiMatrix * matrix = gui->addMatrix("wave", 6, false);
    matrix->onMatrixEvent(this, &LfoSlider::onWaveSelected);
    matrix->setRadioMode(true);
    matrix->getChildAt(0)->setSelected(true);
    
    gui->addSlider("time", 0, 1, 0)->setPrecision(4)->bind(time);
    gui->addSlider("amplitude",0, 1,0)->setPrecision(3)->bind(amplitude);
    gui->addSlider("offset", 0, 1, 0)->setPrecision(3)->bind(offset);
    gui->addSlider("result", 0, 1, 0)->setPrecision(4)->bind(result);
}

void LfoSlider::onWaveSelected(ofxDatGuiMatrixEvent e)
{
    wave = e.child;
}
