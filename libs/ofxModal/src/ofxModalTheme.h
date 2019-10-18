/*
    Copyright (C) 2016 Stephen Braitsch [http://braitsch.io]

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

#pragma once
#include "ofImage.h"
#include "ofColor.h"
#include "ofGraphics.h"
#include "ofxSmartFont/ofxSmartFont.h"

class ofxModalTheme {

    public:
    
        ofxModalTheme(){
            buttonX.normal.load("ofxbraitsch/ofxmodal/modal-buttonX.png");
            buttonX.active.load("ofxbraitsch/ofxmodal/modal-buttonX-hover.png");
            fonts.title = ofxSmartFont::add("ofxbraitsch/fonts/HelveticaNeueLTStd-Md.otf", 24, "modal-title");
            fonts.message = ofxSmartFont::add("ofxbraitsch/fonts/verdana.ttf", 20, "modal-message");
        }
    
    /*
        default color palette
    */
    
        struct{
            struct{            
                struct{
                    ofColor label = ofxDatGuiTheme::hex(0x333333);
                    ofColor labelOnMouseOver = ofxDatGuiTheme::hex(0x333333);
                    ofColor labelOnMouseDown = ofxDatGuiTheme::hex(0x333333);
                    ofColor background = ofxDatGuiTheme::hex(0xffffff);
                    ofColor backgroundOnMouseOver = ofxDatGuiTheme::hex(0xE6E6E6);
                    ofColor backgroundOnMouseDown = ofxDatGuiTheme::hex(0xD4D4D4);
                    ofColor border = ofxDatGuiTheme::hex(0xCCCCCC);
                } wireframe;
                struct{
                    ofColor label = ofxDatGuiTheme::hex(0xffffff);
                    ofColor labelOnMouseOver = ofxDatGuiTheme::hex(0xffffff);
                    ofColor labelOnMouseDown = ofxDatGuiTheme::hex(0xffffff);
                    ofColor background = ofxDatGuiTheme::hex(0x337ab7);
                    ofColor backgroundOnMouseOver = ofxDatGuiTheme::hex(0x286090);
                    ofColor backgroundOnMouseDown = ofxDatGuiTheme::hex(0x1f4c73);
                    ofColor border = ofxDatGuiTheme::hex(0x1f4c73);
                } darkblue;
            } button;
            struct{
                ofColor title = ofxDatGuiTheme::hex(0x111111);
                ofColor body = ofxDatGuiTheme::hex(0x777777);
            } text;
            struct{
                ofColor header = ofxDatGuiTheme::hex(0xFFFFFF);
                ofColor body = ofxDatGuiTheme::hex(0xFFFFFF);
                ofColor footer = ofxDatGuiTheme::hex(0xF7F7F9);
                ofColor hrule = ofxDatGuiTheme::hex(0x333333);
            } modal;
            struct{
                ofColor background = ofxDatGuiTheme::hex(0x000000);
            } window;
        } color;
    
        struct{
            struct{
                float header = 1.0f;
                float body = 1.0f;
                float footer = 1.0f;
                float hrule = 1.0f;
            } modal;
            struct{
                float background = 0.7f;
            } window;
        } alpha;
    
    
    /* postioning, width & height */
    
        struct{
            struct{
                int width = 160;
                int height = 80;
                bool borders = true;
            } button;
            struct {
                int width = 800;
                int height = 600;
                int padding = 30;
                int vMargin = 2;
                int autoSize = true;
            } modal;
            struct{
                float wordSpacing = 14.0f;
            } text;
        } layout;
    
        struct {
            shared_ptr<ofxSmartFont> title;
            shared_ptr<ofxSmartFont> message;
        } fonts;

        struct {
            float speed = 0.4f;
        } animation;
    
        struct {
            int width = 26;
            int height = 26;
            int hitPadding = 20;
            ofImage normal;
            ofImage active;
        } buttonX;


};

