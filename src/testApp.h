#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxXmlSettings.h"

#define SPACER 10

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
        void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
        void guiEvent(ofxUIEventArgs &e);
        void processOpenFileSelection(ofFileDialogResult openFileResult);

        void savePattern();
		ofxXmlSettings XML;
        int pointSize;
        ofPoint currentLocation;
        ofPoint canvasLocation;
        ofPoint startLine;
        ofImage spriteEnemy;
        ofImage spriteCollectable;
        string loadedFile;
        bool creatingEnemies;
        bool testingSpeed;
        int patternNumber;
        int buttonCount;
        float speed;
        float multiplier;
        float currentSpeed;
        ofPoint startingTest;
        ofPoint endingTest;
        ofPoint currentTest;
    
        ofxUICanvas *gui;
        ofxUICanvas *levelGui;
        ofxUITabBar *guiTabBar;
        ofxUITextInput *levelInput;
        ofxUITextInput *patternInput;
        ofxUIDropDownList *patternList;
        ofxUILabelButton *removeButton;

        vector<ofPoint> enemies;
        vector<ofPoint> collectables;
        vector<std::string> patterns;
};

