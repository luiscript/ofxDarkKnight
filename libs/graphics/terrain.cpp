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
    
    ofxDatGuiMatrix * matrix = gui->addMatrix("drawing modes", 8, true);
    matrix->setRadioMode(true);
    matrix->onMatrixEvent(this, &Terrain::onDrawingModeChange);
    
    
    ofxDatGuiFolder * terrainParams = gui->addFolder("GLOBAL");
    terrainParams->addSlider("rotate", 0, 180)->bind(rotate);
    terrainParams->addSlider("scale", 0, 100)->bind(scl);
    terrainParams->addSlider("translate x", -3000, 3000, 0)->bind(translateX);
    terrainParams->addSlider("translate y", -1500, 0, 0)->bind(translateY);
    terrainParams->addSlider("translate z", -500, 300, 0)->bind(translateZ);
    terrainParams->addSlider("noise", 0, 1, 0)->bind(noiseX);
    terrainParams->addSlider("direction",-1, 1, 0)->bind(direction);
    terrainParams->addSlider("particle size", 0, 100, 1)->bind(lineWidth);
    terrainParams->expand();
    
//    ofxDatGuiFolder * patternsParams = gui->addFolder("PATTERNS");
//    ofxDatGuiMatrix * matrix = patternsParams->addMatrix("drawing modes", 8, true);
//    matrix->onMatrixEvent(this, &Terrain::onDrawingModeChange);
//    matrix->setRadioMode(true);
//    patternsParams->expand();
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
    
    
    
    
    
    //if(e.child == 1) drawingMode = GL_TRIANGLES;
    //if(e.child == 4) drawingMode = GL_QUADS;
    //if(e.child == 6) drawingMode = GL_POLYGON;
    //if(e.child == 7) drawingMode = GL_LINE_STRIP;
    //if(e.child == 3) drawingMode = GL_TRIANGLE_FAN;
    //if(e.child == 4) drawingMode = GL_TRIANGLES_ADJACENCY;
    //if(e.child == 5) drawingMode = GL_LINE_LOOP;
    //if(e.child == 6) drawingMode = GL_LINES;
    //if(e.child == 7) drawingMode = GL_LINES_ADJACENCY;

    
}


