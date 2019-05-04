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

#include "NoiseSlider.hpp"


void NoiseSlider::setup()
{
    noiseValue = 0;
    amplitude = 1;
    offset = 0;
    random = ofRandom(0.0001,1.00001);
    mult = 1.0;
}

void NoiseSlider::update()
{
    noiseValue = ofNoise(ofGetElapsedTimef() * mult, random) * amplitude + offset;
}

void NoiseSlider::draw()
{
    
}

void NoiseSlider::addModuleParameters()
{
    gui->addSlider("amplitude", 0, 1, 1)->bind(amplitude);
    gui->addSlider("offset", 0,1,0)->bind(offset);
    gui->addSlider("velocity", 0, 4)->bind(mult);
    gui->addSlider("noise", 0,1,0)->setPrecision(4)->bind(noiseValue);
}


