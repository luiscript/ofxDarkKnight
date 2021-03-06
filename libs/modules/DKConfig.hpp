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

class DKConfig : public DKModule
{
private:
    vector<ofVec2f> resolutions;
    float wiresTension;
    int indexResolution;
    int indexSplit;
    
public:
    ofEvent<ofVec2f> onResolutionChangeEvent;
    
    void setup()
    {
        indexResolution = 2;
        indexSplit = 0;
        
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

		//auto themeA = new ofxDatGuiThemeAqua;
		//gui->setTheme(themeA);
    }
    
    void update()
    {
        
    }
    
    void draw()
    {
        
    }
    
    void addModuleParameters()
    {
		vector<string> fileOptions =
		{
			"Save",
			"Open"
		};
        
        vector<string> splitOptions = {
            "SINGLE",
            "DOUBLE",
            "TRIPLE"
        };
        
        vector<string> options =
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
        
       
		auto fileDropDown = gui->addFolder("FILE");
		auto saveButton = fileDropDown->addButton("Save");
		auto openButton = fileDropDown->addButton("Open");
		saveButton->onButtonEvent(this, &DKConfig::onFileButtonPress);
		openButton->onButtonEvent(this, &DKConfig::onFileButtonPress);
        gui->addBreak();
        
        auto resolutionDropDown = gui->addDropdown("Resolution", options);
        resolutionDropDown->onDropdownEvent(this, &DKConfig::onResolutionChange);
        resolutionDropDown->select(2);
        gui->addBreak();
        
        auto numScreens = gui->addDropdown("Split", splitOptions);
        numScreens->onDropdownEvent(this, &DKConfig::onSplitChange);
        numScreens->select(0);
        gui->addBreak();
		
        gui->addFRM();
    }

	void onFileButtonPress(ofxDatGuiButtonEvent e)
	{
		if (e.target->getName() == "Save")
		{
			
		}
		else if (e.target->getName() == "Open")
		{

		}
	}

    void onResolutionChange(ofxDatGuiDropdownEvent e)
    {	
        indexResolution = e.child;
        ofVec2f resolution = resolutions[indexResolution];
        resolution.x = resolution.x * (indexSplit + 1);
        ofNotifyEvent(onResolutionChangeEvent, resolution, this);
    }
    
    void onSplitChange(ofxDatGuiDropdownEvent e)
    {
        indexSplit = e.child;
        ofVec2f resolution = resolutions[indexResolution];
        resolution.x = resolution.x * (indexSplit + 1);
        ofNotifyEvent(onResolutionChangeEvent, resolution, this);
    }
    
    float * getWiresTension()
    {
        
    }  
};
