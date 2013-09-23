#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#define SPACER 10

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

		ofxXmlSettings XML;
		ofTrueTypeFont TTF;
        int pointSize;
        ofPoint currentLocation;
        ofPoint canvasLocation;
        ofPoint startLine;
        ofImage spriteEnemy;
        ofImage spriteCollectable;
        string loadedFile;
        bool creatingEnemies;
        int patternNumber;

        vector<ofPoint> enemies;
        vector<ofPoint> collectables;

};

