
/*
    Copyright (C) 2014 Stephen Braitsch [http://braitsch.io]

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

#include "ofxParagraph.h"

ofxParagraph::ofxParagraph(std::string text, int width, Alignment align)
: mFont(nullptr)
, mColor(ofColor::black)
, mIndent(40)
, mSpacing(6)
, mLeading(16)
, bDrawBorder(false)
, mBorderColor(ofColor::black)
, mBorderPadding(15)
, bDrawWordBoundaries(false)
, mWordBoundaryPadding(2)
, mWordBoundaryColor(ofColor::red)
{
    setText(text);
    setAlignment(align);
    setWidth(width);
};

void ofxParagraph::draw()
{
    ofPushStyle();
        for (int i=0; i<mWords.size(); i++) {
            ofSetColor(mColor);
            mFont->draw(mWords[i].text, this->x + mWords[i].rect.x, this->y + mWords[i].rect.y);
            if (bDrawWordBoundaries == true){
                ofPushStyle();
                ofNoFill();
                ofSetColor(mWordBoundaryColor);
                ofDrawRectangle(this->x + mWords[i].rect.x - mWordBoundaryPadding,
                       this->y + mWords[i].rect.y-mLineHeight - mWordBoundaryPadding,
                       mWords[i].rect.width + (mWordBoundaryPadding * 2),
                       mLineHeight + (mWordBoundaryPadding * 2));
                ofPopStyle();
            }
        }
        if (bDrawBorder == true){
            ofNoFill();
            ofSetColor(mBorderColor);
            ofDrawRectangle(this->x - mBorderPadding,
                   this->y - mFont->getLineHeight() - mBorderPadding,
                   mWidth + (mBorderPadding * 2),
                   mHeight + (mBorderPadding * 2));
        }
    ofPopStyle();
}

void ofxParagraph::draw(int x, int y)
{
    this->x = x;
    this->y = y;
    draw();
}

void ofxParagraph::setColor(ofColor color)
{
    mColor = color;
}

void ofxParagraph::setColor(int color)
{
    mColor = ofColor::fromHex(color);
}

int ofxParagraph::getWidth()
{
    return mWidth;
}

int ofxParagraph::getHeight()
{
    return mHeight;
}

int ofxParagraph::getStringHeight(std::string s)
{
    if (s == "") s = "ABCDEFGHIJKLMNOPQWXYZ1234567890";
    return mFont->height(s);
}

void ofxParagraph::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

void ofxParagraph::drawBorder(bool draw)
{
    bDrawBorder = draw;
}

void ofxParagraph::drawBorder(ofColor color)
{
    bDrawBorder = true;
    mBorderColor = color;
}

void ofxParagraph::setBorderPadding(int padding)
{
    mBorderPadding = padding;
}

void ofxParagraph::drawWordBoundaries(bool draw)
{
    bDrawWordBoundaries = draw;
}

/*
    re-rendering methods
*/

void ofxParagraph::setText(std::string text)
{
    mText = trim(text);
    render();
}

void ofxParagraph::setWidth(int width)
{
    mWidth = width;
    render();
}

void ofxParagraph::setSpacing(int spacing)
{
    mSpacing = spacing;
    render();
}

void ofxParagraph::setLeading(int leading)
{
    mLeading = leading;
    render();
}

void ofxParagraph::setIndent(int indent)
{
    mIndent = indent;
    render();
}

void ofxParagraph::setAlignment(Alignment align)
{
    mAlignment = align;
    render();
}

void ofxParagraph::setFont(std::shared_ptr<ofxSmartFont> ttf)
{
    mFont = ttf;
    render();
}

void ofxParagraph::setFont(std::string file, int size, std::string name)
{
    mFont = ofxSmartFont::add(file, size, name);
    render();
}

void ofxParagraph::render()
{
    if (mFont == nullptr) return;
    mWords.clear();
    mLineHeight = 0;
    std::string str = mText;
    
// break paragraph into words //
    int position = str.find(" ");
    while ( position != std::string::npos )
    {
        std::string s = str.substr(0, position);
        word w = {s, mFont->rect(s)};
        mWords.push_back(w);
        str.erase(0, position + 1);
        position = str.find(" ");
        if (w.rect.height > mLineHeight) mLineHeight = w.rect.height;
    }
// append the last word //
    word w = {str, mFont->rect(str)};
    mWords.push_back(w);
    if (w.rect.height > mLineHeight) mLineHeight = w.rect.height;
    
// assign words to lines //
    int y = 0;
    int x = mAlignment == ALIGN_LEFT ? mIndent : 0;
    mLines.clear();
    std::vector<word*> line;
    for (int i=0; i<mWords.size(); i++) {
        if (x + mWords[i].rect.width < mWidth){
            mWords[i].rect.x = x;
            mWords[i].rect.y = y;
            x += mWords[i].rect.width + mSpacing;
            line.push_back(&mWords[i]);
        }   else{
            if (line.size() > 0 ) y+= mLineHeight + mLeading;
            mWords[i].rect.x = 0;
            mWords[i].rect.y = y;
            x = mWords[i].rect.width + mSpacing;
            if (line.size() > 0) mLines.push_back(line);
            line.clear();
            line.push_back(&mWords[i]);
        }
    }
// append the last line //
    mLines.push_back(line);
    mHeight = mLines.size() * (mLineHeight + mLeading);
    
// reposition words for right & center aligned paragraphs //
    if (mAlignment == ALIGN_CENTER){
        for(int i=0; i<mLines.size(); i++) {
            int lineWidth = 0;
            for(int j=0; j<mLines[i].size(); j++) {
                lineWidth+= mLines[i][j]->rect.width;
            }
            lineWidth+= mSpacing * (mLines[i].size()-1);
        // calculate the amount each word should move over //
            int offset = (mWidth - lineWidth) / 2;
            for(int j=0; j<mLines[i].size(); j++) mLines[i][j]->rect.x += offset;
        }
    }   else if (mAlignment == ALIGN_RIGHT){
        for(int i=0; i<mLines.size(); i++) {
            word* lword = mLines[i].back();
    // calculate the distance the last word in each line is from the right boundary //
            int offset = mWidth - (lword->rect.x + lword->rect.width);
            for(int j=0; j<mLines[i].size(); j++) mLines[i][j]->rect.x += offset;
        }
    }
}

