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

#include "MediaPool.hpp"


void MediaPool::setup()
{

}

void MediaPool::init()
{
    font.load("fonts/Roboto-Light.ttf", 15, true, true);
    setModuleHasChild(true);
    alpha = 255;
    yOffsetGui = 120;
    showMediaPool = false;
    lightPositionX = lightPositionY = lightPositionZ = 0;
    diffuseR = diffuseG = diffuseB = 0;
    ambientR = ambientG = ambientB = 0;
    
    ofVec2f resolution = {getModuleWidth(), getModuleHeight()};
    
    for (auto collectionItem : collection) {
        collectionItem.thumbnail->load(collectionItem.fileName);
        collectionItem.canvas->setupModule(collectionItem.collectionName, this->getGuiTheme(), resolution);
        collectionItem.canvas->gui->setVisible(false);
    }
    
    translation = nullptr;
    
    nextIndex = index = 0;
    modules = nullptr;
    
    
    mainFbo.allocate(getModuleWidth(), getModuleHeight(), GL_RGBA, 4);
    mainFbo.begin();
    ofClear(0, 0, 0,0);
    mainFbo.end();
    
    
    currentCanvas = collection[index].canvas;
    currentCanvas->moduleIsChild = true;
    
    addCustomParameters();
    triggerPoolMedia(index);
    //ofAddListener(ofEvents().mousePressed, this, &MediaPool::mousePressed);
    cout << "media pool init "<< endl;
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
    
    if(!drawMode)
    {
       ofClear(0,0,0,255);
    }
    
    
    ofPushStyle();
    ofEnableLighting();
    light.enable();
    light.setPosition(lightPositionX, lightPositionY, lightPositionZ);
    
    light.setDiffuseColor(ofColor(diffuseR, diffuseG, diffuseB));
    light.setAmbientColor(ofColor(ambientR, ambientG, ambientB));
    ofEnableDepthTest();

    currentCanvas->draw();
    
    ofDisableDepthTest();
    light.disable();
    ofDisableLighting();
    ofPopStyle();
    

    
    if(drawMode)
    {
        ofPushStyle();
        ofSetColor(0, 0, 0, 255 - alpha);
        ofFill();
        ofDrawRectangle(0, 0, getModuleWidth(), getModuleHeight());
        ofPopStyle();
    }
    
    if (hasInput) {
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        inputFbo->draw(0,0);
    }
    
    
    mainFbo.end();

    drawMediaPool();
    currentCanvas->gui->draw();
}


void MediaPool::drawMediaPool()
{
    int width = gui->getWidth();
    float x = gui->getPosition().x;
    float y = 0.25 * width;
    float cellWidth = width/4;
    float cellHeight = cellWidth * 0.5615;

    ofPushMatrix();
    ofPushStyle();

    ofSetColor(255, 180);

    ofPoint pos = gui->getPosition();
    ofTranslate(pos.x, pos.y + yOffsetGui);
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
                collection[curIndex].thumbnail->draw(i*cellWidth+4,  j*cellHeight + 4, cellWidth-4, cellHeight-8);
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
    ofPopMatrix();

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
        
        int ind = 4 * yIndex + xIndex;
        triggerPoolMedia(ind);
    }
}

void MediaPool::triggerPoolMedia(int ind)
{
    if(ind < collection.size())
    {
        currentCanvas->disable();
        string moduleName = getName() + "/" + currentCanvas->getName();
        
        if(modules != nullptr) {
            unordered_map<string, Module*>::iterator it = modules->find(moduleName);
            if(it != modules->end()) modules->erase(moduleName);
        }
        
        currentCanvas = collection[ind].canvas;
        collection[ind].canvas->gui->setPosition(gui->getPosition().x - 400, gui->getPosition().y + 450 );
        currentCanvas = collection[ind].canvas;
        
        moduleName = getName() + "/" + currentCanvas->getName();
    
        currentCanvas->moduleIsChild = true;
        if (modules != nullptr) modules->insert({moduleName, currentCanvas});
        
        
        currentCanvas->enable();
        
        if(nextIndex != index) currentCanvas->triggerMidiEvent();
        nextIndex = index = ind;
        addCustomParameters();

    }
}
    

void MediaPool::addModuleParameters()
{
    gui->addSlider("Alpha", 0, 255)->bind(alpha);
    ofxDatGuiToggle * toggleDraw = gui->addToggle("draw");
    toggleDraw->setChecked(false);
    toggleDraw->onToggleEvent(this, &MediaPool::onToggleDraw);

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
        ofxDatGuiFolder* lightParams = currentCanvas->gui->addFolder("LIGHT POSITION", ofColor::white);
        lightParams->addSlider("x", -ofGetWidth(), 2*ofGetWidth(), ofGetWidth()/2)->bind(lightPositionX);
        lightParams->addSlider("y", -ofGetHeight(), 2*ofGetHeight()/2, ofGetHeight()/2)->bind(lightPositionY);
        lightParams->addSlider("z", -2000, 2000, 0)->bind(lightPositionZ);
        lightParams->expand();
        
        ofxDatGuiFolder* diffuseColorParams = currentCanvas->gui->addFolder("DIFFUSE COLOR", ofColor::white);
        diffuseColorParams->addSlider("r", 0,255,255)->bind(diffuseR);
        diffuseColorParams->addSlider("g", 0,255,255)->bind(diffuseG);
        diffuseColorParams->addSlider("b", 0,255,255)->bind(diffuseB);
        diffuseColorParams->expand();
        
        ofxDatGuiFolder* ambientColorParams = currentCanvas->gui->addFolder("AMBIENT COLOR", ofColor::white);
        
        ambientColorParams->addSlider("r", 0,255,255)->bind(ambientR);
        ambientColorParams->addSlider("g", 0,255,255)->bind(ambientG);
        ambientColorParams->addSlider("b", 0,255,255)->bind(ambientB);
        ambientColorParams->expand();
        
        currentCanvas->customParams = true;
    }
    
}

void MediaPool::gotMidiPitch(string mapping)
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

void MediaPool::drawMasterInput()
{
    drawInputConnection();
}

void MediaPool::drawMasterOutput()
{
    drawOutputConnection();
}

void MediaPool::setCollectionName(string name)
{
    collectionName = name;
}


ofFbo * MediaPool::getFbo()
{
    return &mainFbo;
}

void MediaPool::setFbo(ofFbo * fboptr)
{
    inputFbo = fboptr;
    hasInput = fboptr != nullptr;
}

void MediaPool::mousePressed(ofMouseEventArgs & mouse)
{
    translation != nullptr
        ?   updatePoolIndex(*mouse.getPtr() - translation->x, *(mouse.getPtr()+1) - translation->y)
        :   updatePoolIndex(*mouse.getPtr(), *(mouse.getPtr()+1))
    ;
}

void MediaPool::unMount()
{
    //currentCanvas->de
}

void MediaPool::enableLighting(bool enable)
{
    enableLight = enable;
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
