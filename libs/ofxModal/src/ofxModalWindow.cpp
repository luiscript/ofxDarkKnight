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

#include "ofxModalWindow.h"

vector<ofxModalWindow*> ofxModalWindow::modals;
ofxModalWindow* ofxModalWindow::activeModal = nullptr;
std::shared_ptr<ofxModalTheme> ofxModalWindow::mTheme = nullptr;

/*
    pubic methods
*/

void ofxModalWindow::show()
{
// ensure we never show two at the same time //
    if (activeModal == nullptr){
        centerModal();
        mVisible = true;
        activeModal = this;
        mState = FADING_IN;
        mAnimation.nTicks = 0;
        mAnimation.percent = 0;
        ofAddListener(ofEvents().draw, this, &ofxModalWindow::onDraw, OF_EVENT_ORDER_AFTER_APP + 9999);
        ofAddListener(ofEvents().update, this, &ofxModalWindow::onUpdate, OF_EVENT_ORDER_AFTER_APP + 9999);
        ofAddListener(ofEvents().mouseMoved, this, &ofxModalWindow::onMouseMove, OF_EVENT_ORDER_AFTER_APP + 9999);
        ofAddListener(ofEvents().mousePressed, this, &ofxModalWindow::onMousePress, OF_EVENT_ORDER_AFTER_APP + 9999);
        ofAddListener(ofEvents().windowResized, this, &ofxModalWindow::onWindowResize, OF_EVENT_ORDER_AFTER_APP + 9999);
    }
}

void ofxModalWindow::hide()
{
    if (mVisible){
        mVisible = false;
        mState = FADING_OUT;
        mAnimation.nTicks = 0;
        mAnimation.percent = 0;
    }
}

void ofxModalWindow::alert(string message)
{
    mAlertMessage = message;
}

void ofxModalWindow::setWidth(int w)
{
    mModal.width = w;
    mModal.x = ofGetWidth() / 2 - mModal.width / 2;
    if (mMessage != nullptr) mMessage->setWidth(mModal.width - (mModal.padding * 2));
}

void ofxModalWindow::setHeight(int h)
{
    mModal.autoSize = false;
    mModal.height.body = h - mModal.height.header - mModal.height.footer;
// establish a minimun body height //
    if (mModal.height.body < 200) mModal.height.body = 200;
}

void ofxModalWindow::setTitle(string text)
{
	mTitle.text = text;

    if (mTitle.font != nullptr) mTitle.height = mTitle.font->height(mTitle.text);
}

void ofxModalWindow::setMessage(string text)
{
// disabled if this modal already has components //
    if (mModalComponents.size() == 0){
        if (mMessage == nullptr) mMessage = new ofxParagraph();
        setMessageTheme();
        mMessage->setText(text);
        if (mModal.autoSize) mModal.height.body = mMessage->getHeight() + mModal.padding * 2;
    }
}

void ofxModalWindow::setMessageAlignment(ofxParagraph::Alignment align)
{
    if (mMessage != nullptr) mMessage->setAlignment(align);
}

void ofxModalWindow::setButtonLabel(string label, int bIndex)
{
    if (bIndex < mFooterButtons.size()){
        mFooterButtons[bIndex]->setLabel(label);
    }   else{
        cout << "ofxModalWindow::setButtonLabel index " << bIndex << " is out of range" << endl;
    }
}

void ofxModalWindow::setAlert(shared_ptr<ofxModalAlert> alert)
{
    mAlert = alert;
}

void ofxModalWindow::setTheme(std::shared_ptr<ofxModalTheme> theme)
{
    mTheme = theme;
    mColor.title = theme->color.text.title;
    mColor.header = theme->color.modal.header;
    mColor.body = theme->color.modal.body;
    mColor.footer = theme->color.modal.footer;
    mColor.hrule = theme->color.modal.hrule;
    mCloseButton.normal = &theme->buttonX.normal;
    mCloseButton.active = &theme->buttonX.active;
    mCloseButton.rect.width = theme->buttonX.width;
    mCloseButton.rect.height = theme->buttonX.height;
    mCloseButton.hitPadding = theme->buttonX.hitPadding;
    mAnimation.tTicks = theme->animation.speed * ofGetFrameRate();
    mAnimation.tOpacity = theme->alpha.window.background;
    mTitle.font = theme->fonts.title;
    mTitle.height = theme->fonts.title->height(mTitle.text);
    mModal.height.header = mModal.padding * 2 + mCloseButton.rect.height;
    mModal.height.footer = mModal.padding * 3;
    mModal.height.body = theme->layout.modal.height - mModal.height.header - mModal.height.footer;
    mModal.padding = theme->layout.modal.padding;
    mModal.vMargin = theme->layout.modal.vMargin;
    mModal.autoSize = theme->layout.modal.autoSize;
    setWidth(theme->layout.modal.width);
    if (mMessage != nullptr) setMessageTheme();
    for (int i=0; i<mFooterButtons.size(); i++){
        mFooterButtons[i]->setWidth(theme->layout.button.width);
        mFooterButtons[i]->setLabelColor(theme->color.button.wireframe.label);
        mFooterButtons[i]->setBackgroundColors(theme->color.button.wireframe.background,
            theme->color.button.wireframe.backgroundOnMouseOver,
            theme->color.button.wireframe.backgroundOnMouseDown);
        if (theme->layout.button.borders) {
            mFooterButtons[i]->setBorder(theme->color.button.wireframe.border, 1);
        }
    }
}

void ofxModalWindow::setCancelable(bool cancelable)
{
    mCancelable = cancelable;
    for (auto btn : mFooterButtons){
        if (ofToLower(btn->getLabel()) == "cancel") btn->setVisible(cancelable);
    }
}

int ofxModalWindow::getWidth()
{
    return mModal.width;
}

int ofxModalWindow::getHeight()
{
    return mModal.height.header + mModal.height.body + mModal.height.footer;
}

int ofxModalWindow::getPadding()
{
    return mModal.padding;
}

bool ofxModalWindow::visible()
{
    return activeModal != nullptr;
}

/*
    protected methods
*/

ofxModalWindow::ofxModalWindow()
{
    mAlert = nullptr;
    mAlertMessage = "";
    mMessage =  nullptr;
    mCancelable = true;
    mCloseButton.mouseOver = false;
    if (mTheme == nullptr) mTheme = std::make_shared<ofxModalTheme>();
    setTheme(mTheme);
    setTitle("Title");
    addButton("Close");
    setTheme(mTheme);
    modals.push_back(this);
}

void ofxModalWindow::dispatchCallbacks(ofxModalEvent::EventType eType)
{
    for(auto e: e_callbacks){
        if (e.eType == eType){
            e.callback(ofxModalEvent(eType, this));
        }
    }
    for(auto g: g_callbacks) g(ofxModalEvent(eType, this));
    if (eType == ofxModalEvent::HIDDEN){
        if (mAlertMessage != ""){
            if (mAlert != nullptr) mAlert->alert(mAlertMessage);
            mAlertMessage = "";
        }
    }
}

/*
    private methods
*/

void ofxModalWindow::onDraw(ofEventArgs &e)
{
    ofPushStyle();
        ofFill();
    // draw background blackout //
        ofSetColor(0, 0, 0, mAnimation.nOpacity);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    // draw modal header //
        ofSetColor(mColor.header);
        ofDrawRectangle(mModal.x, mModal.y, mModal.width, mModal.height.header);
    // draw modal body //
        ofSetColor(mColor.body);
        ofDrawRectangle(mModal.x, mModal.y + mModal.height.header, mModal.width, mModal.height.body);
    // draw modal header //
        ofSetColor(mColor.footer);
        ofDrawRectangle(mModal.x, mModal.y + mModal.height.header + mModal.height.body, mModal.width, mModal.height.footer);
    // draw title //
        ofSetColor(mColor.title);
        mTitle.font->draw(mTitle.text, mTitle.x, mTitle.y);
        ofDrawLine(mBreak1.p1, mBreak1.p2);
        ofDrawLine(mBreak2.p1, mBreak2.p2);
    // draw message //
        if (mMessage != nullptr) mMessage->draw();
    // draw close button //
        if (mCancelable){
            ofSetColor(ofColor::white);
            if (mCloseButton.mouseOver == false){
                mCloseButton.normal->draw(mCloseButton.rect);
            }   else{
                mCloseButton.active->draw(mCloseButton.rect);
            }        
        }
    ofPopStyle();
// draw body components //
    for(auto mc:mModalComponents) mc.component->draw();
// draw footer buttons //
    for(auto button:mFooterButtons) button->draw();
}

void ofxModalWindow::onUpdate(ofEventArgs &e)
{
    if (mState == FADING_IN || mState == FADING_OUT){
        animate();
    }   else{
    // update modal components //
        for(auto bn:mFooterButtons) bn->update();
        for(auto mc:mModalComponents) mc.component->update();
    }
}

void ofxModalWindow::onButtonEvent(ofxDatGuiButtonEvent e)
{
    hide();
    if (e.target == mFooterButtons[0]){
        dispatchCallbacks(ofxModalEvent::CONFIRM);
    }   else if (mFooterButtons.size() > 1 && e.target == mFooterButtons[1]){
        dispatchCallbacks(ofxModalEvent::CANCEL);
    }
}

void ofxModalWindow::layout()
{
    mTitle.x = mModal.x + mModal.padding;
    mTitle.y = mModal.y + mModal.padding + mCloseButton.rect.height/2 + mTitle.height/2;
    mBreak1.p1.x = mModal.x;
    mBreak1.p1.y = mModal.y + mModal.height.header;
    mBreak1.p2.x = mModal.x + mModal.width;
    mBreak1.p2.y = mBreak1.p1.y;
    mBreak2.p1.x = mBreak1.p1.x;
    mBreak2.p1.y = mModal.y + mModal.height.header + mModal.height.body;
    mBreak2.p2.x = mBreak1.p2.x;
    mBreak2.p2.y = mBreak2.p1.y;
    if (mMessage != nullptr) {
        mMessage->setPosition(mBreak1.p1.x + mModal.padding, mBreak1.p1.y + mModal.padding + mMessage->getStringHeight());
    }
    mCloseButton.rect.x = mModal.x + mModal.width - mModal.padding - mCloseButton.rect.width;
    mCloseButton.rect.y = mModal.y + mModal.padding;
    mCloseButton.hitRect.x = mCloseButton.rect.x - mCloseButton.hitPadding;
    mCloseButton.hitRect.y = mCloseButton.rect.y - mCloseButton.hitPadding;
    mCloseButton.hitRect.width = mCloseButton.rect.width + (mCloseButton.hitPadding * 2);
    mCloseButton.hitRect.height = mCloseButton.rect.height + (mCloseButton.hitPadding * 2);
    for(auto mc:mModalComponents) {
        mc.component->setPosition(mModal.x + mModal.padding + mc.x, mModal.y + mModal.height.header + mc.y);
    }
    for(int i=0; i<mFooterButtons.size(); i++){
        int buttonSpacing = 8;
        int w = mFooterButtons[0]->getWidth();
        int x = mModal.x + mModal.width - mModal.padding - w;
        int y = mBreak2.p1.y + mModal.height.footer/2 - mFooterButtons[i]->getHeight()/2;
        x -= (w+buttonSpacing) * i;
        mFooterButtons[i]->setPosition(x, y);
    }
}

void ofxModalWindow::animate()
{
    mAnimation.nTicks++;
    if (mState == FADING_IN){
        mAnimation.percent = easeInOutQuad(float(mAnimation.nTicks)/mAnimation.tTicks);
    }   else if (mState == FADING_OUT) {
        mAnimation.percent = 1.0f - easeInOutQuad(float(mAnimation.nTicks)/mAnimation.tTicks);
    }
    int height = getHeight();
    mAnimation.nOpacity = mAnimation.percent * (mAnimation.tOpacity * 255);
    mModal.y = -height + mAnimation.percent * (ofGetHeight()/2 - height/2 + height);
    if (mAnimation.nTicks == mAnimation.tTicks){
        if (mState == FADING_IN){
            mState = VISIBLE;
            dispatchCallbacks(ofxModalEvent::SHOWN);
        }   else if (mState == FADING_OUT){
            mState = HIDDEN;
        // modal is closed, ok to show another one now //
            activeModal = nullptr;
            dispatchCallbacks(ofxModalEvent::HIDDEN);
            ofRemoveListener(ofEvents().draw, this, &ofxModalWindow::onDraw);
            ofRemoveListener(ofEvents().update, this, &ofxModalWindow::onUpdate);
            ofRemoveListener(ofEvents().mouseMoved, this, &ofxModalWindow::onMouseMove);
            ofRemoveListener(ofEvents().mousePressed, this, &ofxModalWindow::onMousePress);
        }
    }
// sync modal components as window moves //
    layout();
}

void ofxModalWindow::centerModal()
{
    int height = getHeight();
    mModal.x = ofGetWidth() / 2 - mModal.width / 2;
    mModal.y = -height + mAnimation.percent * (ofGetHeight()/2 - height/2 + height);
    layout();
}

void ofxModalWindow::setMessageTheme()
{
    mMessage->setFont(mTheme->fonts.message);
    mMessage->setColor(mTheme->color.text.body);
    mMessage->setSpacing(mTheme->layout.text.wordSpacing);
    mMessage->setWidth(mModal.width - (mModal.padding * 2));
}

void ofxModalWindow::onMousePress(ofMouseEventArgs &e)
{
    ofPoint mouse = ofPoint(e.x, e.y);
    if (mVisible && mCancelable == true){
        if (ofRectangle(mModal.x, mModal.y, mModal.width, getHeight()).inside(mouse) == false) {
            hide();
        }   else if (mCloseButton.hitRect.inside(mouse)){
            hide();
        }
    }
}

void ofxModalWindow::onMouseMove(ofMouseEventArgs &e)
{
    ofPoint mouse = ofPoint(e.x, e.y);
    mCloseButton.mouseOver = mCloseButton.hitRect.inside(mouse);
}

void ofxModalWindow::onWindowResize(ofResizeEventArgs &e)
{
    centerModal();
}

double ofxModalWindow::easeInOutQuad( double t )
{
    return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
}




