/*
 Copyright (C) 2018 Luis Fernando Garc’a PŽrez [http://luiscript.com]
 
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

#include "terrain.hpp"

void Terrain::setup()
{
    drawingMode = GL_TRIANGLE_STRIP;
    scl = 60;
    w = getModuleWidth();
    h = getModuleHeight();
    
    numCols = 60;
    numRows = 60;
    
    lineWidth = 4.0;
    flying = noiseX = direction = rotate = translateX = translateY = translateZ = 0;
    
    
    for(int y = 0; y < numRows - 1; y++)
    {
        for(int x = 0; x < numCols; x++)
        {
            noise[x][y] = 0;
        }
    }
    
}

void Terrain::update()
{
    flying -= ofMap(direction, -1, 1, -0.05, 0.05);
    
    float yoff = flying;
    
    float noiseNorm = ofMap(noiseX, 0, 1, 0, 0.1);
    for(int y = 0; y < numRows - 1; y++)
    {
        float xoff = 0;
        for(int x = 0; x < numCols; x++)
        {
            noise[x][y] = ofMap(ofNoise(xoff, yoff), 0, 1, -100, 100);
            if(y == (int)numRows/2)
            {
                noise[x][y] = noise[x][y];
            }
            xoff += noiseNorm;
        }
        yoff += noiseNorm;
    }
    
    //fbo.begin();
    
    //cam.end();
    //fbo.end();
}


void Terrain::draw()
{
    ofPushMatrix();
    ofPushStyle();
    ofClear(0,0,0,0);
    ofRotateX(rotate);
    ofTranslate(translateX, translateY, translateZ);
    
    ofSetColor(255);
    ofNoFill();

    glLineWidth(lineWidth);
    glPointSize(lineWidth);
    for(int y = 0; y < numRows - 1; y++)
    {
        glBegin(drawingMode);
        for(int x = 0; x < numCols; x++)
        {
            glVertex3f(x*scl, y*scl, noise[x][y]);
            glVertex3f(x*scl, (y+1)*scl, noise[x][y+1]);
        }
        glEnd();
    }
    
    ofPopStyle();
    ofPopMatrix();
    
}
void Terrain::addModuleParameters()
{
    
    ofxDatGuiMatrix * matrix = gui->addMatrix("mode", 8, true);
    matrix->setRadioMode(true);
    matrix->onMatrixEvent(this, &Terrain::onDrawingModeChange);
    
    addSlider("rotate", rotate, 0, 180, 0);
    addSlider("scale", scl, 0, 100, 100);
    addSlider("translate x", translateX, -1545, -45, 0);
    addSlider("translate y", translateY, -1500, 0, 0);
    addSlider("translate z", translateZ, -1500, 1500, 0);
    addSlider("noise", noiseX, 0, 1, 0);
    addSlider("direction", direction, -1, 1, 0);
    addSlider("particle size", lineWidth, 0, 10, 1);
     
}

void Terrain::onSliderEvent(ofxDatGuiSliderEvent e)
{
    onSliderEventParent(e);
}


void Terrain::onDrawingModeChange(ofxDatGuiMatrixEvent e)
{
    //drawingMode = e.child;
    if(e.child == 0) drawingMode = GL_POINTS;
    if(e.child == 1) drawingMode = GL_LINES;
    if(e.child == 2) drawingMode = GL_QUAD_STRIP;
    if(e.child == 3) drawingMode = GL_LINE_STRIP;
    if(e.child == 4) drawingMode = GL_TRIANGLE_STRIP;
    
}


