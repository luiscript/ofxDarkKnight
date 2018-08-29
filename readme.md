## ofxDarkKnight ##

![ofxDarkKnight](ofxaddons_thumbnail.png)

**Node based programming environment for openframeworks**

![ofxDarkKnight](screenshot.png)
# Getting started

## Download openFrameworks
ofxDarkKnight it's built on top of [openframeworks](https://openframeworks.cc/) v0.9.8 and it's only tested on MacOS for now. 

Download [openFrameworks v0.9.8](https://openframeworks.cc/download/older/) for your development platform. Open an example and run it to make sure that everything is working correctly.

If your having problems running the examples please [read this guide](https://openframeworks.cc/ofBook/chapters/setup_and_project_structure.html#runningexamples)


Double check that you download the (0.9.8) version


## Install ofxDarkKnight
Download and install [ofxDarkKnight](https://github.com/luiscript/ofxDarkKnight/) addon into your `openframeworks_v0.9.8/addons/` directory or install it using git:
  
  `git clone https://github.com/luiscript/ofxDarkKnight/`

# Running the examples

1. Open the [basicExample](https://github.com/luiscript/ofxDarkKnight/tree/master/examples/basicExample) project under your `openframeworks/addons/ofxDarkKnight/example` directory.

2. Run the project.

3. Press `CMD+Intro` to show/hide the modules list.

4. Click on SKETCH POOL module to add it to the stage.

5. Repeat the step (3) and click the PREVIEW module.

6. Press `CMD+M` to enter mapping mode.

7. Connect the output of the SKETCH POOL module into the input of PREVIEW module by dragging the mouse.

8. Select the second item in the sketch pool.

9. Play with the module parameters.

# Create a new project

If you want to start a new project from scratch, follow the next steps:

- Open the OF projectGenerator

- Add ofxDarkKnight addon

- Generate the project

- Copy the `ofxDarkKnight/data` content into `yourNewProject/bin/data` directory

## Including the addon 

- Add ofxDarkKnight to your code
- Create an instance of ofxDarkKnight class and call it `manager`.

> ofApp.h file:

```c++
#include "ofxDarkNight.hpp"
#include "ofMain.h"

class ofApp : public ofBaseApp
{
    public:
    
    void setup();
    void update();
    void draw();

    ofxDarkKnight manager;
}
```

## Adding existing modules

This are the modules that you want to use in your project, here you need to include the base DK modules, your custom modules and the third party DK modules.

> ofApp.h:

```c++
#include "ofxDarkNight.hpp"
#include "ofMain.h"

// this module draws a preview on the gui
#include "preview.hpp"

//this is the basic sketch pool collection
#include "basic.hpp"
```


## Config the manager

The `setup` function of ofxDarkKnight class is expecting an `unordered_map<string, Module*>` pointer with a collection of pairs `moduleName-module` objects of type `string-Module*`.

Create the data structure and send it to the manager in the `setup` function of `ofApp.cpp`.

Then just call `manager.update()` in the main `update()`function and `manager.draw()` in the main `draw()` function.

<aside class="notice">
You may need to add <code>#include "unordered_map"</code> in your <code>ofApp.h</code> file.
</aside>

> ofApp.cpp:

```c++
void ofApp::setup()
{
  ofBackground(0);

  unordered_map<string, Module*> modulesPool;
  modulesPool = {
      { "PREVIEW", new Preview },
      { "SKETCH POOL", new Basic }
  };
  manager.setup(&modulesPool);
}

void ofApp::update()
{
    manager.update();
}

void ofApp::draw()
{
    manager.draw();
}
```

## Setup the SCREEN OUTPUT module


The SCREEN OUTPUT module requires to share the OpenGL context with the main window. To do that, create a `shared_ptr<ofAppBaseWindow>` property of the ofApp class.

Add the window to the manager by modifying the `mainWindow` property.

Modify the `main.cpp` file to add some custom configuration to the window.

> ofApp.h:

```c++
    ...
    ofDarkKnight manager;
    shared_ptr<ofAppBaseWindow> mainWindow;
}
```
> ofApp.cpp:

```c++
void ofApp::setup()
{
    //...
    manager.mainWindow = mainWindow;
    //...
}
```

> main.cpp:

```c++
#include "ofMain.h"
#include "ofApp.h"

int main( ){
    CGRect thisMonitorArea = CGDisplayBounds(CGMainDisplayID());
    int thisWidth = CGRectGetWidth(thisMonitorArea);
    int thisHeight = CGRectGetHeight(thisMonitorArea);
    
    ofGLFWWindowSettings settings;
    settings.resizable = true;
    settings.width = 1920;
    settings.height = 1080;
    settings.windowMode = OF_WINDOW;
    
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    shared_ptr<ofApp> mainApp(new ofApp);
    
    mainApp->mainWindow = mainWindow;
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
````


# Create new modules

The fun part of ofxDarkKnight begins a soon as you start creating your own modules. The addon provides c++ clases that will help you create new modules with ease. For now you can create only 2 types of modules: Stand alone & Drawer modules. 

The **stand alone** modules are designed to interact with your sketches in different ways. For instance you may want to add a MIDI controller to modify parameters or a Syphon server to send your graphics to other app in real time. 

The **drawer modules** are designed to draw something on the screen, this could be a video, generative animation, shader or anything that can draw on the screen.


## The Module Class


To create a new module follow the steps:

1. Add a new c++ file to your project.
2. Include `module.hpp` file (provided with the addon).
3. Create a new c++ class that inheriths from `Module` class.
4. Implement the virtual functions of `Module` class


> Stand alone module header template

```c++
#include "module.hpp"

class ofxDarkNightTemplate : public Module
{
    public:
    void setup();
    void update();
    void draw();
    void addModuleParameters();
    void drawMasterInput();
    void drawMasterOutput();
    void setFbo(ofFbo *);
    ofFbo * getFbo();
    void unMount();
}
```

This are the list of virtual functions that you should implement in your module:

functions | description
---------- | -------
void setup() | Runs once at the begining of the sketch.
void update() | Runs every frame to update data.
void draw() | Runs every frame to draw something on the screen.
void addModuleParameters() | Runs once after setup, here you will add all the parameters that your module needs. ofxDarkKnight uses ofxDatGui, to learn how to add paramemeters please read the [ofxDatGui docs](https://braitsch.github.io/ofxDatGui/). Right now only sliders are supported.
void drawMasterInput() | You should implement this function when your module needs an input in order to work, for example a video FX.
void drawMasterOutput() | If your module returns something, then you should implement this function.
void setFbo(ofFbo *) | This function will be called when an external module conects it's main output with the current module's input and it will recieve a pointer to an ofFbo that contains the graphics.
ofFbo getFbo() | This function will be called when you try to connect the current module's output to an external module's input. It should return an ofFbo pointer that contains the drawing.
void unMount() | Runs once when the app closes.



You don't have to implement all the functions, just use the ones that you need. None function is required, it all depends on your goals.


## Simple drawer module

Let's create a simple ellipse module that has 2 main parameters: radius and fillColor. For the radius we will use an integer value and for the fillColor we will use 3 integer values for the RGB components of the color.

We will need to implement the `drawMasterOutput()` because this module will draw something and will return the drawing in the module's output.

In this particular case, we will need to allocate an fbo because the drawing is generated from scratch and it do not depends on the module's input.

Implement the `ofFbo * getFbo()` to pass it's drawing to the output.


> Simple Ellipse module ofxDarkKnightEllipse.hpp

```c++
#include "module.hpp"

class ofxDarkKnightEllipse : public Module
{
    public:
    void setup();
    void draw();
    void addModuleParameters();
    void drawMasterOutput();
    ofFbo * getFbo();
    
    private:
    ofFbo * fbo;
    int radius;
    int fillColorR;
    int fillColorG;
    int fillColorB;
}
```

> Simple Ellipse module ofxDarkKnightEllipse.cpp

```c++
#include "ofxDarkKnightEllipse.hpp"

void ofxDarkKnightEllipse::setup()
{
  //allocate the fbo
  fbo->allocate(1920, 1080, GL_RGBA);
  fbo->begin();
  ofClear();
  fbo->end();

  //initialize all your parameters
  radius = 30;
  fillColorR = 255;
  fillColorG = fillColorB = 0;
}

void ofxDarkKnightEllipse::draw()
{
  ofFill();
  ofSetColor(fillColorR,fillColorG, fillColorB);
  ofDrawEllipse(ofGetWidth()/2, ofGetHeight()/2, radius, raduis);
}

void ofxDarkKnightEllipse::addModuleParameters()
{
  gui->addSlider("radius", 10, 400)->bind(radius);
  gui->addSlider("R", 10, 400)->bind(fillColorR);
  gui->addSlider("G", 10, 400)->bind(fillColorG);
  gui->addSlider("B", 10, 400)->bind(fillColorB);
}

ofFbo * ofxDarkKnightEllipse::getFbo()
{
  return fbo;
}
```

## Add new module to manager

Now that we created a new module, we need to add it to the manager, please refer to the [Adding existing modules](#adding-existing-modules) and follow the instructions. The `modulesPool` structure should look like the code example.

Try to run the project and if everything is good you should be able to add your brand new Ellipse module and control it's size and fillColor with the module's parameters.


Remember to include the module in the main header file <code>#include "ofxDarkKnightEllipse.hpp"</code>


> Simple Ellipse module ofxDarkKnightEllipse.cpp

```c++
  ...
  modulesPool = {
      { "ELLIPSE", new ofxDarkKnightEllipse},
      { "PREVIEW", new Preview },
      { "SKETCH POOL", new Basic }
  };
  ...
```

## Collections


Collections are specials modules that holds up to 16 drawer modules with the hability to change between each other in real time. Look at the `ofxDarkKnight/libs/collections/basic.hpp`to see an example of a basic collection.

To create a new collection you will need to follow next steps: 

1. create a new class that inheriths from the `MediaPool` class.

2. In the setup function set the collection name with the `setCollectionName(string)` function.

3. Add a thumbnail image for the module.

4. Provide a set of collectionItems that includes: name, module instance, thumbnail path, ofImage instance.

5. Call the `init()` function.

> Basic collection example

```c++
class Basic : public MediaPool{

public:
    void setup(){
        setCollectionName("Basic");
        
        collection = {
            {
                "TERRAIN",
                new Terrain,
                "thumbnails/terrain.jpg",
                new ofImage
            },
            {
                "CONSTELLATION",
                new Constellation,
                "thumbnails/constellation.jpg",
                new ofImage
            },
            {
              "ELLIPSE",
              new ofxDarkKnightEllipse,
              "thumbnails/ellipse.jpg",
              new ofImage
            }
        };
        init();
    }
};
```


## Adding Ellipse example to the collection


Since we're not using the Ellipse module as a stand alone module we can skip some tasks from the previus module definition:

1. You don't need to declare and allocate the fbo, the media pool does it for you in an efficient way that only 1 fbo will be needed to draw any of the 16 drawer modules.

2. You don't need the `drawMainOutput()` function because we will not return nothing from the drawers, only the collection will have a single output and the `MediaPool` class will handle it for you.

3. Since we're not returning anything you should skip the `ofFbo * getFbo()` function from your module.

Do this modifications before runing your project that includes the Ellipse module in the basic collection.

> Modified version of Ellipse module header

```c++
#include "module.hpp"

class ofxDarkKnightEllipse : public Module
{
    public:
    void setup();
    void draw();
    void addModuleParameters();

    private:
    int radius;
    int fillColorR;
    int fillColorG;
    int fillColorB;
}
```

> Modified version of Ellipse module

```c++
#include "ofxDarkKnightEllipse.hpp"

void ofxDarkKnightEllipse::setup()
{
  //initialize all your parameters
  radius = 30;
  fillColorR = 255;
  fillColorG = fillColorB = 0;
}

void ofxDarkKnightEllipse::draw()
{
  ofFill();
  ofSetColor(fillColorR,fillColorG, fillColorB);
  ofDrawEllipse(ofGetWidth()/2, ofGetHeight()/2, radius, raduis);
}

void ofxDarkKnightEllipse::addModuleParameters()
{
  gui->addSlider("radius", 10, 400)->bind(radius);
  gui->addSlider("R", 10, 400)->bind(fillColorR);
  gui->addSlider("G", 10, 400)->bind(fillColorG);
  gui->addSlider("B", 10, 400)->bind(fillColorB);
}
```

# Featured modules

MIDI controller [ofxDarkKnightMidi](https://github.com/luiscript/ofxDarkKnightMidi)

OSC controller [ofxDarkKnightOsc](https://github.com/luiscript/ofxDarkKnightOsc)

Ableton Link [ofxDarkKnightAbletonLink](https://github.com/luiscript/ofxDarkKnightAbletonLink)

Syphon server [ofxDarkKnightSyphon](https://github.com/luiscript/ofxDarkKnightSyphon)


# License

 **Copyright** (C) 2018 [Luis Fernando García](http://luiscript.com)
 
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

 # Credits

ofxDarkKnight was created by [Luis Fernando García](https://github.com/luiscript)

ofxDatGui was created by [Stephen Braitsch](https://github.com/braitsch)