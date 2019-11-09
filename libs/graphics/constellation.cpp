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


#include "constellation.h"

void Constellation::setup()
{

    drawingMode = 0;
    connectionDistance = 100;
    numParticles = 2000;
    particleSize = 8.0;
    lineWidth = 1.0;
    velocityMultx = velocityMulty = velocityMultz = 1;
    
    
    mesh.setMode(OF_PRIMITIVE_LINES);
    mesh.clear();
    meshPoints.setMode(OF_PRIMITIVE_POINTS);
    meshPoints.clear();
    meshPoints.enableIndices();
    
    for (int i=0; i<numParticles; i++)
    {
        ofVec3f pos(ofRandom(getModuleWidth()), ofRandom(getModuleHeight()), ofRandom(-512, 512));
        mesh.addVertex(pos);
        meshPoints.addVertex(pos);
        ofVec3f randomVelocity = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
        vel.push_back(randomVelocity);
    }
    
    numParticles = 1000;
    ofClear(0,0,0,0);
}


void Constellation::update()
{
    
    if(numParticles > 0)
    {
        mesh.clearIndices();
        meshPoints.clearIndices();
        for (int i=0; i<numParticles; i++) {
            ofVec3f point = mesh.getVertex(i);
            point.x += vel[i].x * velocityMultx;
            point.y += vel[i].y * velocityMulty;
            point.z += vel[i].z * velocityMultz;

            if(point.x > getModuleWidth() || point.x < 0)
                vel[i].x = vel[i].x*-1;
            if(point.y > getModuleHeight() || point.y < 0)
                vel[i].y = vel[i].y*-1;
            if(point.z > 700 || point.z < -1024)
                vel[i].z = vel[i].z*-1;

            mesh.setVertex(i, point);
            meshPoints.setVertex(i, point);
            meshPoints.addIndex(i);
            
            if(connectionDistance > 0)
            {
                ofVec3f verta = mesh.getVertex(i);
                for (int b=i+1; b<numParticles-1; b++) {
                    ofVec3f vertb = mesh.getVertex(b);
                    float distance = verta.distance(vertb);
                    if (distance <=  connectionDistance) {
                        mesh.addIndex(i);
                        mesh.addIndex(b);
                    }
                    
                }
                
            }
            
        }
        
    }
}

void Constellation::draw()
{
    //draw the FBO
    //fbo.begin();
    ofPushMatrix();
    ofClear(0,0,0,0);
    
    
    ofSetColor(255);
    
    if(particleSize > 0)
    {
        if (drawingMode == 0) {
            mesh.setMode(OF_PRIMITIVE_LINES);
            if(lineWidth > 0)
            {
                glEnable(GL_LINE_SMOOTH);
                glLineWidth(lineWidth);
                mesh.draw();
            }
            meshPoints.setMode(OF_PRIMITIVE_POINTS);
            glEnable(GL_POINT_SMOOTH);
            glPointSize(particleSize);
            meshPoints.draw();
        } else if(drawingMode == 1)
        {
            mesh.setMode(OF_PRIMITIVE_LINES);
            if(lineWidth > 0)
            {
                glEnable(GL_LINE_SMOOTH);
                glLineWidth(lineWidth);
                mesh.draw();
            }
            for(int c = 0; c < numParticles; c++)
            {
                ofVec3f p = meshPoints.getVertex(c);
                ofDrawSphere(p.x, p.y, p.z, particleSize);
            }
        }else if(drawingMode == 2)
        {            
            mesh.setMode(OF_PRIMITIVE_LINES);
            if(lineWidth > 0)
            {
                glEnable(GL_LINE_SMOOTH);
                glLineWidth(lineWidth);
                mesh.draw();
            }
            for(int c = 0; c < numParticles; c++)
            {
                ofVec3f p = meshPoints.getVertex(c);
                ofDrawBox(p.x, p.y, p.z, particleSize, particleSize, particleSize);
            }
        }else if(drawingMode == 3)
        {
            mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            if(lineWidth > 0)
            {
                glEnable(GL_LINE_SMOOTH);
                glLineWidth(lineWidth);
                mesh.draw();
            }
        }else if(drawingMode == 4)
        {
            mesh.setMode(OF_PRIMITIVE_TRIANGLES);
            if(lineWidth > 0)
            {
                glEnable(GL_LINE_SMOOTH);
                glLineWidth(lineWidth);
                mesh.draw();
            }
        }
        else if(drawingMode == 5)
        {
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP_ADJACENCY);
            ofPushStyle();
            ofNoFill();
            if(lineWidth > 0)
            {
                glEnable(GL_LINE_SMOOTH);
                glLineWidth(lineWidth);
                mesh.draw();
            }
            ofPopStyle();
        }
        
        else if(drawingMode == 6)
        {
            mesh.setMode(OF_PRIMITIVE_LINES);
            ofPushStyle();
            ofNoFill();
            if(lineWidth > 0)
            {
                glEnable(GL_LINE_SMOOTH);
                glLineWidth(lineWidth);
                mesh.draw();
            }
            meshPoints.clearVertices();
            meshPoints.clearIndices();
            meshPoints.setMode(OF_PRIMITIVE_TRIANGLES);
            for(int i = 0; i<mesh.getNumIndices(); i++)
            {
                int ind = mesh.getIndex(i);
                ofVec3f p = mesh.getVertex(ind);
                meshPoints.addVertex(p);
            }
            meshPoints.draw();
            
            ofPopStyle();
        }
    }
    
    //ofDisableDepthTest();

    ofPopMatrix();
    //cam.end();
    //fbo.end();
}

void Constellation::addModuleParameters()
{
    ofxDatGuiMatrix * matrix = gui->addMatrix("drawing modes", 8, true);
    matrix->onMatrixEvent(this, &Constellation::onDrawingModeChange);
    matrix->setRadioMode(true);
    matrix->getChildAt(0)->setSelected(false);
    
    addSlider("count", numParticles, 0, 2000, 500);
    addSlider("size", particleSize, 0, 50, 5);
    addSlider("vel x", velocityMultx, 0, 50, 1);
    addSlider("vel y", velocityMulty, 0, 50, 1);
    addSlider("vel z", velocityMultz, 0, 50, 1);
    addSlider("distance", connectionDistance, 50, 500, 100);
    addSlider("line width", lineWidth, 0, 10, 1);
}

void Constellation::onDrawingModeChange(ofxDatGuiMatrixEvent e)
{
    drawingMode = e.child;
}


