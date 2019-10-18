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
#include "ofAppRunner.h"
#include "ofxDatGui.h"
#include "ofxParagraph/src/ofxParagraph.h"
#include "ofxModalEvent.h"
#include "ofxModalTheme.h"

class ofxModalAlert;

class ofxModalWindow {
    
    public:

        void show();
        void hide();
        void alert(string message);
    
        void setWidth(int w);
        void setHeight(int h);
        void setTitle(string text);
        void setMessage(string text);
        void setMessageAlignment(ofxParagraph::Alignment align);
        void setButtonLabel(string label, int bIndex = 0);
        void setAlert(shared_ptr<ofxModalAlert> alert);
        void setCancelable(bool cancelable);
        virtual void setTheme(std::shared_ptr<ofxModalTheme> theme);
    
        int getWidth();
        int getHeight();
        int getPadding();
        static bool visible();
    
        template<typename T, typename args, class ListenerClass>
        void addListener(T* owner, void (ListenerClass::*listenerMethod)(args))
        {
        //  cout << "ofxModalWindow :: adding listener" << endl;
            using namespace std::placeholders;
            g_callbacks.push_back(std::bind(listenerMethod, owner, _1));
        }
    
        template<typename T, typename args, class ListenerClass>
        void addListener(ofxModalEvent::EventType event, T* owner, void (ListenerClass::*listenerMethod)(args))
        {
        //  cout << "ofxModalWindow :: adding listener" << endl;
            using namespace std::placeholders;
            e_callbacks.push_back({event, std::bind(listenerMethod, owner, _1)});
        }
    
        void removeListener()
        {
        //  cout << "ofxModalWindow :: removing listener" << endl;
            g_callbacks.clear();
        }
    
        void removeListener(ofxModalEvent::EventType event)
        {
        //  cout << "ofxModalWindow :: removing listener" << endl;
            for(int i=0; i<e_callbacks.size(); i++){
                if (e_callbacks[i].eType == event){
                    e_callbacks.erase(e_callbacks.begin() + i);
                }
            }
        }
    
		struct {
			ofColor title;
			ofColor header;
			ofColor body;
			ofColor hrule;
			ofColor footer;
		} mColor;

    protected:
    
        ofxModalWindow();
    
    /*
        modal components
    */
    
        struct ModalComponent{
            int x;
            int y;
            ofxDatGuiComponent* component;
            ModalComponent(ofxDatGuiComponent* c, int x, int y){
                this->x = x;
                this->y = y;
                this->component = c;
            }
        };
    
        template<class component>
        component* addComponent(component* c, int x = 0, int y = 0, int w = 0)
        {
            if (y == 0) {
                y = mModal.padding;
                for(auto mc:mModalComponents) y+= mc.component->getHeight() + mModal.vMargin;
            }
            mModalComponents.push_back(ModalComponent(c, x, y));
            int maxW = mModal.width-(mModal.padding*2)-x;
            if (w > 0 && w < maxW) {
                c->setWidth(w, .3);
            }   else{
                c->setWidth(maxW, .3);
            }
            if (mModal.autoSize){
                int h = mModal.padding;
                for(auto mc:mModalComponents) h+= mc.component->getHeight() + mModal.vMargin;
                mModal.height.body = h + mModal.padding;
            }
            return c;
        }
    
        void autoSize()
        {
            for(auto mc:mModalComponents) mc.component->setWidth(mModal.width-(mModal.padding*2)-mc.x, .3);
        }
    
        void autoSize(ofxDatGuiComponent* c)
        {
            for(auto mc:mModalComponents) if (mc.component == c) c->setWidth(mModal.width-(mModal.padding*2)-mc.x, .3);
        }
    
    /*
        footer buttons
    */
    
        ofxDatGuiButton* addButton(string label)
        {
            ofxDatGuiButton* btn = new ofxDatGuiButton(ofToUpper(label));
            btn->setStripeVisible(false);
            btn->setLabelAlignment(ofxDatGuiAlignment::CENTER);
            btn->onButtonEvent(this, &ofxModalWindow::onButtonEvent);
            btn->setWidth(mTheme->layout.button.width);
            btn->setLabelColor(mTheme->color.button.wireframe.label);
            btn->setBackgroundColors(mTheme->color.button.wireframe.background, mTheme->color.button.wireframe.backgroundOnMouseOver, mTheme->color.button.wireframe.backgroundOnMouseDown);
            btn->setBorder(mTheme->color.button.wireframe.border, 1);
            mFooterButtons.push_back(btn);
            return btn;
        }
    
        ofxDatGuiButton* getButton(int index)
        {
            return mFooterButtons[index];
        }
    
        string mAlertMessage;
        shared_ptr<ofxModalAlert> mAlert;
        static std::shared_ptr<ofxModalTheme> mTheme;

    private:
    
        enum {
            HIDDEN,
            FADING_IN,
            VISIBLE,
            FADING_OUT
        } mState;
    
    
        struct{
            int x;
            int y;
            int width;
            int padding;
            int vMargin;
            struct{
                int header;
                int body;
                int footer;
            } height;
            bool autoSize;
        } mModal;
    
        struct{
            int nTicks;
            int tTicks;
            float percent;
            float nOpacity;
            float tOpacity;
        } mAnimation;
    
        struct{
            int x;
            int y;
            int height;
            string text;
            shared_ptr<ofxSmartFont> font;
        } mTitle;
    
        struct{
            ofPoint p1;
            ofPoint p2;
        } mBreak1;
    
        struct{
            ofPoint p1;
            ofPoint p2;
        } mBreak2;
    
        struct {
            bool mouseOver;
            int hitPadding;
            ofImage* normal;
            ofImage* active;
            ofRectangle rect;
            ofRectangle hitRect;
        } mCloseButton;
    
        bool mVisible;
        bool mCancelable;
        ofxParagraph* mMessage;
        vector<ModalComponent> mModalComponents;
        vector<ofxDatGuiButton*> mFooterButtons;
    
        void onDraw(ofEventArgs &e);
        void onUpdate(ofEventArgs &e);
        inline void layout();
        inline void animate();
        void centerModal();
        void setMessageTheme();
        void onMousePress(ofMouseEventArgs &e);
        void onMouseMove(ofMouseEventArgs &e);
        void onWindowResize(ofResizeEventArgs &e);
        void onButtonEvent(ofxDatGuiButtonEvent e);
        void dispatchCallbacks(ofxModalEvent::EventType eType);
    
    /*
        event subscribers
    */
    
        typedef std::function<void(ofxModalEvent)> onModalEventCallback;
        struct subscriber{
            ofxModalEvent::EventType eType;
            onModalEventCallback callback;
        };
        vector<subscriber> e_callbacks;
        vector<onModalEventCallback> g_callbacks;
    
    /*
        static properties & methods
    */
    
        static ofxModalWindow* activeModal;
        static vector<ofxModalWindow*> modals;
        static inline double easeInOutQuad( double t );
    
};


class ofxModalAlert : public ofxModalWindow {

    public:

        ofxModalAlert()
        {
            setTitle("alert");
            getButton(0)->setLabel("ok");
            setTheme(mTheme);
            setCancelable(false);
            setMessage("This is an alert message!");
        }
    
        void alert(string message)
        {
            setMessage(message);
            ofxModalWindow::show();
        }

};


