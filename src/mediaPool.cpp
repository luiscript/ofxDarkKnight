/*
 Copyright (C) 2018 Luis Fernando Garc’a [http://luiscript.com]
 
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

#include "mediaPool.hpp"


void MediaPool::setup()
{

}

void MediaPool::init()
{
    font.load("fonts/Roboto-Light.ttf", 15, true, true);
    setModuleHasChild(true);
    alpha = 255;
    yOffsetGui = 20;
    gui->setWidth(ofGetWidth()/5);
    numItems = 0;
    showMediaPool = false;
    
    ofVec2f resolution = { getModuleWidth(), getModuleHeight() };
    
    translation = nullptr;
    nextIndex = index = 0;
    modules = nullptr;
    
    mainFbo.allocate(getModuleWidth(), getModuleHeight(), GL_RGBA, 4);
    mainFbo.begin();
    ofClear(0, 0, 0,0);
    mainFbo.end();
    
    mediaPoolFbo.allocate(gui->getWidth(), gui->getWidth() * 0.5615);
    mediaPoolFbo.begin();
    ofClear(0, 0, 0, 0);
    mediaPoolFbo.end();
    
    drawMediaPool();
    
    currentCanvas = collection[index].canvas;
    currentCanvas->moduleIsChild = true;
    
    addCustomParameters();
    triggerPoolMedia(index);
    

    //MIDI mappings
    //to trigger the index 0 (first media) with midi cannel 16 pitch 0
    //use this:
    //midiMappings.insert({"16/0", 0});

    addInputConnection(ConnectionType::DK_FBO);
    addOutputConnection(ConnectionType::DK_FBO);
}


void MediaPool::onKeyboardEvent(ofKeyEventArgs & e)
{
    
}

void MediaPool::update()
{
    if(nextIndex != index) triggerPoolMedia(nextIndex);
    currentCanvas->setModuleMidiMapMode(getModuleMidiMapMode());

    currentCanvas->gui->setPosition(gui->getPosition().x, gui->getPosition().y + gui->getWidth() * 0.5625 + yOffsetGui);
    currentCanvas->gui->setTranslation(translation->x, translation->y);
}

void MediaPool::draw()
{
    mainFbo.begin();
    
    ofPushStyle();
    currentCanvas->draw();
    ofPopStyle();
    
    if(hasInput)
    {
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        inputFbo->draw(0, 0);
        ofDisableBlendMode();   
    }
    
    mainFbo.end();

    //drawMediaPool();
    ofPoint pos = gui->getPosition();
    mediaPoolFbo.draw(pos.x, pos.y + yOffsetGui);
    currentCanvas->gui->draw();
}


void MediaPool::drawMediaPool()
{
    mediaPoolFbo.begin();
    ofClear(0,0,0,0);
    int width = gui->getWidth();
    float x = gui->getPosition().x;
    float y = 0.25 * width;
    float cellWidth = width/4;
    float cellHeight = cellWidth * 0.5615;

    ofPushStyle();
    ofSetColor(255, 180);

    for(int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int curIndex = i + 4 * j;
            ofSetColor(66,66,74);
            ofDrawRectangle(i*cellWidth+2, j*cellHeight, cellWidth - 2, cellHeight - 2);
            if(curIndex < collection.size())
            {
                ofSetColor(255);
                collection[curIndex].thumbnail->draw(i*cellWidth+3,  j*cellHeight+1, cellWidth-4, cellHeight-4);
            }
            if(index == curIndex)
            {
                ofSetColor(232, 181, 54, 128);
                ofDrawRectangle(i*cellWidth+2, j*cellHeight, cellWidth - 2, cellHeight - 2);
            }
            if(getModuleMidiMapMode())
            {
                ofSetColor(0, 200);
                ofDrawRectangle(i*cellWidth+2, j*cellHeight, cellWidth - 2, cellHeight - 2);
                
                for (pair<string, int> element : midiMappings) {
                    if(element.second == curIndex)
                    {
                        ofSetColor(255);
                        font.drawString(element.first, i*cellWidth+2 + cellWidth/2 - 15, j*cellHeight + cellHeight/2 + 5);
                    }
                }
                
                if(index == curIndex)
                {
                    ofSetColor(255, 128);
                    ofDrawRectangle(i*cellWidth+2, j*cellHeight, cellWidth - 2, cellHeight - 2);
                }
            }
        }
    }
    ofPopStyle();
    mediaPoolFbo.end();
}


void MediaPool::updatePoolIndex(int mouseX, int mouseY)
{
    ofPoint pos = gui->getPosition();
    float offset = yOffsetGui + gui->getWidth() * 0.5625;
    
    if (mouseX >= pos.x &&
        mouseX <= pos.x + gui->getWidth() &&
        mouseY >= pos.y + yOffsetGui &&
        mouseY <= pos.y + offset) {
        
        int xIndex = (int)  ofMap(mouseX, pos.x, pos.x + gui->getWidth(), 0,4);
        int yIndex = (int)  ofMap(mouseY, pos.y + yOffsetGui, pos.y + offset, 0 ,4);
        
        int newIndex = 4 * yIndex + xIndex;
        
        if(newIndex <= collection.size())
        {
            index = newIndex;
            drawMediaPool();
            triggerPoolMedia(index);
        }
        
    }
}

void MediaPool::triggerPoolMedia(int ind)
{
    if(ind < collection.size())
    {
        currentCanvas->disable();
        collection[ind].canvas->gui->setPosition(gui->getPosition().x - 400, gui->getPosition().y + 450 );
        currentCanvas = collection[ind].canvas;
        string moduleName = getName() + "/" + currentCanvas->getName();
        currentCanvas->moduleIsChild = true;
        currentCanvas->enable();
        currentCanvas->reset();
        
        if(nextIndex != index) currentCanvas->triggerMidiEvent();
        nextIndex = index = ind;
        addCustomParameters();
        
        mainFbo.begin();
        ofClear(0,0,0,0);
        mainFbo.end();
    }
}
    

void MediaPool::addModuleParameters()
{

}

void MediaPool::onMatrix1Change(ofxDatGuiMatrixEvent e)
{

}

void MediaPool::onToggleDraw(ofxDatGuiToggleEvent e)
{
    drawMode = e.target->getChecked();
}

void MediaPool::addCustomParameters()
{
    if(!currentCanvas->customParams)
    {
        currentCanvas->customParams = true;
    }
    
}

void MediaPool::addItem(Module * module, string fileName, string name)
{
    vector<Preset> modulePresets;
    
    CollectionItem item = { name, module, fileName, new ofImage, modulePresets };
    item.thumbnail->load(item.fileName);
    item.canvas->setupModule(name, {getModuleWidth(), getModuleHeight()}, true);
    item.canvas->gui->setVisible(false);
    item.canvas->moduleIsChild = true;
    item.canvas->setModuleIndex(numItems);
    collection.push_back(item);
    numItems++;
}


void MediaPool::gotMidiMapping(string mapping)
{
    
    unordered_map<string, int>::iterator it;
    it = midiMappings.find(mapping);
    
    //midi mapping not found -> insert it
    if(it == midiMappings.end())
    {
        if(this->getModuleMidiMapMode())
        {
            if(index >= 0)
            {
                midiMappings.insert({mapping, index});
            }
        }
    }
    //midi mapping found -> do something
    else {
        if(!this->getModuleMidiMapMode())
        {
            int ind = midiMappings[mapping];
    
            if(ind != index)
            {
                nextIndex = ind;
            } else {
                currentCanvas->triggerMidiEvent();
            }
        }
    }
}

void MediaPool::gotMidiMessage(ofxMidiMessage * msg)
{
    currentCanvas->triggerMidiMessage(msg);
}

void MediaPool::setCollectionName(string name)
{
    collectionName = name;
}


ofFbo * MediaPool::getFbo()
{
    return &mainFbo;
}

int MediaPool::getCurrentIndex()
{
    return index;
}
void MediaPool::setFbo(ofFbo * fboptr)
{
    inputFbo = fboptr;
    hasInput = fboptr != nullptr;
}

void MediaPool::mousePressed(ofMouseEventArgs & mouse)
{
    translation != nullptr
        ?   updatePoolIndex(mouse.x - translation->x, mouse.y - translation->y)
        :   updatePoolIndex(mouse.x, mouse.y)
    ;
}

void MediaPool::unMount()
{
    currentCanvas->unMount();
}

Module * MediaPool::getChildModule()
{
    return currentCanvas;
}

void MediaPool::setModulesReference(unordered_map<string, Module *> * m)
{
    modules = m;
}

void MediaPool::setTranslationReferences(ofVec2f * t)
{
    translation = t;
}

void MediaPool::savePreset()
{
    unordered_map<string, float> preset;
    for(ofxDatGuiComponent * param : currentCanvas->params->children)
    {
        preset.insert({param->getName(), param->getComponentScale()});
    }
    string name = "preset-"+ofToString(collection[index].presets.size());
    collection[index].presets.push_back({name, preset});
}
