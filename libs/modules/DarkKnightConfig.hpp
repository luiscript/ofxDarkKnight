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

class DarkKnightConfig : public Module
{
private:
    vector<string> options;
    vector<ofVec2f> resolutions;
    float wiresTension;
    
public:
    ofEvent<ofVec2f> onResolutionChangeEvent;
    
    void setup()
    {
        resolutions = {
            {3840, 2160},
            {1920, 1200},
            {1920, 1080},
            {1280, 768},
            {1280, 720},
            {1024, 768},
            {800, 600},
            {640, 480}
        };
        
        wiresTension = 0.8;
    }
    
    void update()
    {
        
    }
    
    void draw()
    {
        
    }
    
    void addModuleParameters()
    {
        options =
        {
            "UHD-1 (3840 x 2160)",
            "WUXGA (1920 x 1200)",
            "FULL HD (1920 x 1080)",
            "WXGA (1280 x 768)",
            "HD (1280 x 720)",
            "XGA (1024 x 768)",
            "SVGA (800 x 600)",
            "VGA (640 x 480)"
        };
        
        gui->addFRM();
        //gui->addSlider("tension", 0.0, 1.0)->bind(wiresTension);
        //gui->addDropdown("Select Theme", options)->onDropdownEvent(this, &Config::onResolutionChange);
        auto resolutionDropDown = gui->addDropdown("Select Resolution", options);
        resolutionDropDown->onDropdownEvent(this, &DarkKnightConfig::onResolutionChange);
        resolutionDropDown->setIndex(2);
    }
    
    void onResolutionChange(ofxDatGuiDropdownEvent e)
    {
        ofNotifyEvent(onResolutionChangeEvent, resolutions[e.child], this);
    }
    
    float * getWiresTension()
    {
        
    }  
};
