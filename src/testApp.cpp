#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofEnableAlphaBlending();
	//we load our settings file
	//if it doesn't exist we can still make one
	//by hitting the 's' key
    loadedFile = "pattern.xml";
    
	if( XML.loadFile(loadedFile) ){
    }
    else {
        int tagNum = XML.addTag("level");
        XML.setValue("level:name", "New Level", tagNum);
        XML.setValue("level:speed", 1, tagNum);
    }


    for (int i = 0; i < XML.getNumTags("enemy"); i++) {
        ofPoint tmpPoint;
        tmpPoint.set(XML.getValue("enemy:x", 0, i), XML.getValue("enemy:y", 0, i), XML.getValue("enemy:size", 0, i));
        enemies.push_back(tmpPoint);
    }

    for (int i = 0; i < XML.getNumTags("collectable"); i++) {
        ofPoint tmpPoint;
        tmpPoint.set(XML.getValue("collectable:x", 0, i), XML.getValue("collectable:y", 0, i), XML.getValue("collectable:size", 0, i));
        collectables.push_back(tmpPoint);
    }

    

	//load a monospaced font
	//which we will use to show part of the xml structure
	TTF.loadFont("mono.ttf", 10);
    currentLocation.set(ofGetWidth()/2, ofGetHeight()/2);
    canvasLocation.set(0, 500);
    
//    startLine.x = currentLocation.x - canvasLocation.x;
    startLine.y = 0;
    pointSize = 40;
    creatingEnemies = false;
    patternNumber = 0;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetRectMode(OF_RECTMODE_CENTER);
//DRAW GRID
    ofSetColor(200, 200, 200);
    for (int x = 0; x < ofGetWidth(); x += SPACER) {
        ofLine(x, 0, x, ofGetHeight());
    }

    for (int y = 0; y < ofGetHeight(); y += SPACER) {
        ofLine(0, y, ofGetWidth(), y);
    }

//DRAW PLACEMENTS
    ofSetColor(0, 0, 0);
	ofFill();
    for (int i = 0; i < enemies.size(); i++) {
        if (spriteEnemy.isAllocated()) {
            ofSetColor(255, 255, 255);

            spriteEnemy.draw(enemies[i].x + canvasLocation.x, enemies[i].y + canvasLocation.y, spriteEnemy.getWidth(), spriteEnemy.getHeight());
        } else {
            ofCircle(enemies[i].x + canvasLocation.x, enemies[i].y + canvasLocation.y, enemies[i].z);
//            ofRect(enemies[i].x + canvasLocation.x, enemies[i].y + canvasLocation.y, enemies[i].z, enemies[i].z);
        }
    }
    
    for (int i = 0; i < collectables.size(); i++) {
        if (spriteCollectable.isAllocated()) {
            ofSetColor(255, 255, 255);
            
            spriteCollectable.draw(collectables[i].x + canvasLocation.x, collectables[i].y + canvasLocation.y, spriteCollectable.getWidth(), spriteCollectable.getHeight());
        } else {
            ofRect(collectables[i].x + canvasLocation.x, collectables[i].y + canvasLocation.y, collectables[i].z, collectables[i].z);

            //ofCircle(collectables[i].x + canvasLocation.x, collectables[i].y + canvasLocation.y, collectables[i].z);
        }
    }
    

    ofNoFill();
    if (creatingEnemies) {
        if (spriteEnemy.isAllocated()) {
            ofSetColor(255, 255, 255);
            spriteEnemy.draw(currentLocation.x, currentLocation.y, spriteEnemy.getWidth(), spriteEnemy.getHeight());
        } else {
            ofCircle(currentLocation.x, currentLocation.y, pointSize);
//            ofRect(currentLocation.x, currentLocation.y, pointSize, pointSize);
        }

    }
    else {
        if (spriteCollectable.isAllocated()) {
            ofSetColor(255, 255, 255);
            spriteCollectable.draw(currentLocation.x, currentLocation.y, spriteCollectable.getWidth(), spriteCollectable.getHeight());
        } else {
//            ofCircle(currentLocation.x, currentLocation.y, pointSize);
            ofRect(currentLocation.x, currentLocation.y, pointSize, pointSize);

        }
        
    }
    ofSetColor(0, 0, 0);
    ofFill();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofRect(0, ofGetHeight() - 80, ofGetWidth(), ofGetHeight());
    

    if (creatingEnemies) {
        //control box 1
        ofSetColor(255, 255, 255);
        ofRect(0, ofGetHeight() - 79, 160, 80);
        ofSetColor(0,0,0);
        TTF.drawString("enemy", 10, ofGetHeight() - 40);
        
        //control box 2
        ofSetColor(127, 127, 127);
        ofRect(161, ofGetHeight() - 79, 160, 80);
        ofSetColor(0,0,0);
        TTF.drawString("collectable", 170, ofGetHeight() - 40);
        
    }
    else {
        //control box 1
        ofSetColor(127, 127, 127);
        ofRect(0, ofGetHeight() - 79, 160, 80);
        ofSetColor(0,0,0);
        TTF.drawString("enemy", 10, ofGetHeight() - 40);
        
        //control box 2
        ofSetColor(255,255,255);
        ofRect(161, ofGetHeight() - 79, 160, 80);
        ofSetColor(0,0,0);
        TTF.drawString("collectable", 170, ofGetHeight() - 40);
        
    }
    ofSetColor(255, 255, 255);
    ofRect(0, 0, ofGetWidth(), 80);
    ofSetColor(0);
    for (int i = 0; i < ofGetWidth(); i+=5) {
        ofLine(i, startLine.y + canvasLocation.y, i - 2, startLine.y + canvasLocation.y);
    }
    TTF.drawString("change size: + / -\nscroll: [ / ]\nadd object: space\nload xml file: drag and drop", 20, 20);
                   
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    //no data gets saved unless you hit the s key
    if(key == 's'){
//        XML.saveFile("pattern" + ofToString(patternNumber) + ".xml");
        for (int i = 0; i < enemies.size(); i++) {
            int tagNum = XML.addTag("enemy");

            XML.setValue("enemy:x", enemies[i].x, tagNum);
            XML.setValue("enemy:y", enemies[i].y, tagNum);
            XML.setValue("enemy:type", 1);
            XML.setValue("enemy:size", enemies[i].z, tagNum);
            
            
        }
        for (int i = 0; i < collectables.size(); i++) {
            int tagNum = XML.addTag("collectable");
            XML.setValue("collectable:x", collectables[i].x, tagNum);
            XML.setValue("collectable:y", collectables[i].y, tagNum);
            XML.setValue("collectable:type", 1);
            XML.setValue("collectable:size", collectables[i].z, tagNum);
        }
        
        
        XML.saveFile(loadedFile);
        patternNumber++;
    }
    if (key == 'c') {
        enemies.clear();
        collectables.clear();
        XML.clear();
    }
    if (key == '+') {
        pointSize++;
    }
    if (key == '-') {
        pointSize--;
    }
    if (key == OF_KEY_BACKSPACE) {
        if (creatingEnemies) {
            if (enemies.size() > 0) {
                enemies.pop_back();
            }
        }
        else {
            if (collectables.size() > 0) {
                collectables.pop_back();
            }
        }
    }
    if (key == ' ') {
        if (creatingEnemies) {
            enemies.push_back(ofPoint(currentLocation.x - canvasLocation.x,currentLocation.y - canvasLocation.y,pointSize));
        }
        else {
            collectables.push_back(ofPoint(currentLocation.x - canvasLocation.x,currentLocation.y - canvasLocation.y,pointSize));

        }
    }
    if (key == OF_KEY_DOWN) {
        currentLocation.y += SPACER;
    }
    if (key == OF_KEY_UP) {
        currentLocation.y -= SPACER;
    }
    if (key == OF_KEY_LEFT) {
        currentLocation.x -= SPACER;
    }
    if (key == OF_KEY_RIGHT) {
        currentLocation.x += SPACER;
    }
    if (key == '[') {
        canvasLocation.y -= SPACER;
    }
    if (key == ']') {
        canvasLocation.y += SPACER;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){




}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if (y > ofGetHeight() - 80 && x < 160) {
        creatingEnemies = true;
    }
    if (y > ofGetHeight() - 80 && x > 160 && x < 320) {
        creatingEnemies = false;
    }
}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
    //    dragInfo.files[0].
    if (ofIsStringInString(dragInfo.files[0], "xml")) {
        loadedFile = dragInfo.files[0];
        canvasLocation.set(0, 500);
        if( XML.loadFile(dragInfo.files[0]) ){
            cout << "Clearing Stuff Up" <<endl;
            enemies.clear();
            collectables.clear();
            for (int i = 0; i < XML.getNumTags("enemy"); i++) {
                ofPoint tmpPoint;
                tmpPoint.set(XML.getValue("enemy:x", 0, i), XML.getValue("enemy:y", 0, i), XML.getValue("enemy:size", 0, i));
                enemies.push_back(tmpPoint);
            }
        }
        
    }
    else {
        if (creatingEnemies) {
            spriteEnemy.loadImage(dragInfo.files[0]);
        }
        else {
            spriteCollectable.loadImage(dragInfo.files[0]);
        }
    }
    
}

