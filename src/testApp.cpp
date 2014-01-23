#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofEnableAlphaBlending();
    /* SETUP INITIAL VARAIBLES */
    currentLocation.set(160, ofGetHeight()/2);
    canvasLocation.set(0, 500);
    startLine.y = 0;
    pointSize = 40;
    creatingEnemies = false;
    patternNumber = 0;
    startingTest.set(ofGetWidth() - 50, ofGetHeight()/4);
    currentTest.set(startingTest.x, startingTest.y);
    endingTest.set(ofGetHeight(), ofGetWidth()/2);
    multiplier = 1;
    loadedFile = "pattern.xml";
    testingSpeed = false;
    buttonCount = 0;

    guiTabBar = new ofxUITabBar();
    gui = new ofxUICanvas(320, ofGetHeight());
    levelGui = new ofxUICanvas(320, ofGetHeight());

/* SETUP LEVEL GUI */
    levelGui->addLabel("Level Name");
    levelInput = levelGui->addTextInput("LEVEL", "Name of Level");
    levelGui->addLabel("Author");
    authorInput = levelGui->addTextInput("AUTHOR", "Your Name");
    levelGui->addSpacer(320, 10);
    levelGui->addLabel("Number of edges to collect");
    completeInput = levelGui->addTextInput("COMPLETE", "100");
    levelGui->addSpacer(320, 10);
    levelGui->addLabel("Speed");
    speedSlider = levelGui->addSlider("Starting", -.3, -9, -1, 100, 80);
    levelGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	multiSlider = levelGui->addSlider("Multiplier", 1, 3, 1, 100, 80);
	levelGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    levelGui->addSpacer(320, 20);
    patternList = levelGui->addDropDownList("Included Patterns", patterns, 150);
    levelGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    removeButton = (ofxUILabelButton *) levelGui->addLabelButton("REMOVE PATTERN", "REMOVE PATTERN", 150, 15);
    removeButton->setVisible(false);
    levelGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    levelGui->addSpacer(320, 200);
    levelGui->addLabelButton("SAVE LEVEL", "LEVEL", 310, 40);
    levelGui->setName("Level");

/* SETUP PATTERN GUI */
    
    vector<string> hnames;
    hnames.push_back("ENEMIES");
    hnames.push_back("COLLECTABLES");
    ofxUILabelButton *openButton = gui->addLabelButton("OPEN EXISTING PATTERN", "OPEN");
    patternInput = gui->addTextInput("PATTERN", "pattern0");
    gui->addSpacer();
	gui->addLabel("Placing");
	ofxUIRadio *radio = gui->addRadio("PLACING", hnames, OFX_UI_ORIENTATION_HORIZONTAL);
    radio->activateToggle("COLLECTABLES");
    gui->addSpacer(320,10);
    gui->addLabel("Object Size");
    ofxUISlider *sizeSlider = gui->addBiLabelSlider("SIZE", "TINY", "LARGE", SPACER, 160, pointSize, 310, 10);
    sizeSlider->setIncrement(SPACER);
    gui->addSpacer(320,10);
    usingCanvas = gui->addToggle("USING CANVAS", false);
    gui->addSpacer(320, 40, "SPACE4");
    gui->addTextArea("INSTRUCTIONS4", "ARROW KEYS move an object");
    gui->addTextArea("INSTRUCTIONS2", "SPACE places an object");
    gui->addTextArea("INSTRUCTIONS3", "BACKSPACE removes the object");
    gui->addTextArea("INSTRUCTIONS4", "[ and ] moves the canvas position");
    gui->addSpacer(320, 30, "SPACE1");
    gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    ofxUILabelButton *clearButton = gui->addLabelButton("clear pattern", "CLEAR", 100, 30);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    ofxUILabelButton *saveButton = gui->addLabelButton("save pattern", "SAVE", 100, 30);
    gui->addLabelButton("add to level", "ADD LEVEL", 100, 30);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->setName("Pattern");
    
/* SETUP TABS */
    guiTabBar->addCanvas(levelGui);
    guiTabBar->addCanvas(gui);
    guiTabBar->setPosition(250, 0);

/* SETUP EVENT LISTENERS */
    
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    ofAddListener(levelGui->newGUIEvent,this,&testApp::guiEvent);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
//DRAW GRID
    ofSetColor(200, 200, 200);
    for (int x = 0; x < 320; x += SPACER) {
        ofLine(x, 0, x, ofGetHeight());
    }

    for (int y = 0; y < ofGetHeight(); y += SPACER) {
        ofLine(0, y, 320, y);
    }

//DRAW PLACEMENTS
    ofSetColor(0, 0, 0);
	ofFill();
//FUTURE IMAGE SPRITES
    for (int i = 0; i < enemies.size(); i++) {
        if (spriteEnemy.isAllocated()) {
            ofSetColor(255, 255, 255);

            spriteEnemy.draw(enemies[i].x + canvasLocation.x, enemies[i].y + canvasLocation.y, spriteEnemy.getWidth(), spriteEnemy.getHeight());
        } else {
            ofCircle(enemies[i].x + canvasLocation.x, enemies[i].y + canvasLocation.y, enemies[i].z);
        }
    }
    
    for (int i = 0; i < collectables.size(); i++) {
        if (spriteCollectable.isAllocated()) {
            ofSetColor(255, 255, 255);
            
            spriteCollectable.draw(collectables[i].x + canvasLocation.x, collectables[i].y + canvasLocation.y, spriteCollectable.getWidth(), spriteCollectable.getHeight());
        } else {
            ofRect(collectables[i].x + canvasLocation.x, collectables[i].y + canvasLocation.y, collectables[i].z, collectables[i].z);

        }
    }
    

    ofNoFill();
    if (creatingEnemies) {
        if (spriteEnemy.isAllocated()) {
            ofSetColor(255, 255, 255);
            spriteEnemy.draw(currentLocation.x, currentLocation.y, spriteEnemy.getWidth(), spriteEnemy.getHeight());
        } else {
            ofCircle(currentLocation.x, currentLocation.y, pointSize);
        }

    }
    else {
        if (spriteCollectable.isAllocated()) {
            ofSetColor(255, 255, 255);
            spriteCollectable.draw(currentLocation.x, currentLocation.y, spriteCollectable.getWidth(), spriteCollectable.getHeight());
        } else {
            ofRect(currentLocation.x, currentLocation.y, pointSize, pointSize);

        }
        
    }

    /* SPEED TEST DISPLAY */
    if (currentTest.y > ofGetHeight() - ofGetHeight()/3) {
        currentSpeed = currentSpeed * multiplier;
        currentTest = startingTest;
    }
    if (currentSpeed <= -9) {
        currentSpeed = -9;
    }
    currentTest.y -= currentSpeed;
    ofFill();
    ofSetColor(255, 0, 0);
    ofRect(currentTest.x, currentTest.y, 5, 5);
    
    /* SHOW PROTAGONIST ON THE GRID ONLY */

    if (mouseX <= ofGetWidth()/2) {
        ofSetColor(0, 255, 0);
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofRect(mouseX, mouseY, 25, 25);
        ofSetRectMode(OF_RECTMODE_CORNER);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    if (usingCanvas->getValue()) {
        if (key == '+') {
            pointSize+= SPACER;
        }
        if (key == '-') {
            if (pointSize > SPACER) {
                pointSize-= SPACER;
            }
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
        if (key == OF_KEY_DOWN && currentLocation.y <= ofGetHeight() - pointSize - SPACER) {
            currentLocation.y += SPACER;
        }
        if (!creatingEnemies) {
            if (key == OF_KEY_UP && currentLocation.y >= SPACER) {
                currentLocation.y -= SPACER;
            }
            if (key == OF_KEY_LEFT && currentLocation.x >= SPACER) {
                currentLocation.x -= SPACER;
            }
            if (key == OF_KEY_RIGHT && currentLocation.x <= 320 - pointSize - SPACER) {
                currentLocation.x += SPACER;
            }

        }
        else {
            if (key == OF_KEY_UP && currentLocation.y >= pointSize + SPACER) {
                currentLocation.y -= SPACER;
            }
            if (key == OF_KEY_LEFT && currentLocation.x >= pointSize + SPACER) {
                currentLocation.x -= SPACER;
            }
            if (key == OF_KEY_RIGHT && currentLocation.x <= 320 - pointSize - SPACER) {
                currentLocation.x += SPACER;
            }

            
        }
        if (key == '[') {
            canvasLocation.y -= SPACER;
        }
        if (key == ']') {
            canvasLocation.y += SPACER;
        }
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

}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
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
    /* IF AN IMAGE IS DROPPED IN IT'LL SHOW THAT INSTEAD OF CIRCLE/SQUARES */
    else {
        if (creatingEnemies) {
            spriteEnemy.loadImage(dragInfo.files[0]);
        }
        else {
            spriteCollectable.loadImage(dragInfo.files[0]);
        }
    }
    
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    if(kind == OFX_UI_WIDGET_TOGGLE)
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        if (name == "ENEMIES") {
            creatingEnemies = true;
        }
        if (name == "COLLECTABLES") {
            creatingEnemies = false;
        }
    }
    if (name == "Included Patterns") {
        if (patternList->isOpen() == true) {
            cout << "Show Remove Button" << endl;
            removeButton->setVisible(true);
            removeButton->setLabelText("REMOVE");
        }
        else {
            cout << "Hide Remove Button" << endl;
            removeButton->setVisible(false);
        }
    }

    if (name == "Starting") {
        ofxUISlider *speedSlider = (ofxUISlider *) e.getSlider();
        speed = speedSlider->getScaledValue();
        currentSpeed = speed;
    }
    
    if (name == "Multiplier") {
        ofxUISlider *multiSlider = (ofxUISlider *) e.getSlider();
        multiplier = multiSlider->getScaledValue();

    }

    
    if(name == "SIZE")
	{
		ofxUISlider *slider = (ofxUISlider *) e.getSlider();
        pointSize = slider->getScaledValue();
	}
    
    if(name == "clear pattern") {
        buttonCount++;
        if(buttonCount ==2) {
            enemies.clear();
            collectables.clear();
            XML.clear();
        }
    }

    if (name == "save pattern") {
        buttonCount++;
        if (buttonCount == 2) {
            savePattern();
            buttonCount = 0;
        }
    }
    if (name == "add to level") {
        buttonCount++;
        if (buttonCount == 2) {
            patterns.push_back(patternInput->getTextString() + ".xml");
            patternList->addToggle(patternInput->getTextString());
            savePattern();
            buttonCount = 0;
        }
    }
    if (name == "REMOVE PATTERN") {
        buttonCount++;
        if (buttonCount ==2) {
            vector<ofxUIWidget *> &selected = patternList->getSelected();
            for(int i = 0; i < selected.size(); i++)
            {
                cout << "SELECTED: " << selected[i]->getName() << endl;
                patterns.erase(patterns.begin() + i);
                patternList->removeToggle(selected[i]->getName());
            }
            buttonCount = 0;
        }
    }
        
    if (name == "SAVE LEVEL") {
        buttonCount++;
        if (buttonCount == 2) {
            ofxXmlSettings levelXML;

            levelXML.setValue("name", levelInput->getTextString());
            levelXML.setValue("author", authorInput->getTextString());
            levelXML.setValue("complete", completeInput->getTextString());
            levelXML.setValue("speed", speedSlider->getValue());
            levelXML.setValue("multiplier", multiSlider->getValue());
            
            for (int i = 0; i < patterns.size(); i++) {
                levelXML.setValue("file", patterns[i], i);

            }
            
            
            levelXML.saveFile("new.level");
            levelXML.clear();
            buttonCount = 0;
        }
    }
    if(name == "OPEN EXISTING PATTERN") {
        buttonCount++;
        if (buttonCount == 2) {
		ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a pattern to open");
        
		//Check if the user opened a file
		if (openFileResult.bSuccess){

            cout <<             openFileResult.getName() << endl;
			ofLogVerbose("User selected a file");
			
			//We have a file, check it and process it
            ofFile file (openFileResult.getPath());
            
            if (file.exists()){
                
                string fileExtension = ofToUpper(file.getExtension());
                
                if (fileExtension == "XML") {
                    cout << "it's XML" << " " << file.getAbsolutePath() << endl;
                    patternInput->setTextString(file.getBaseName());
                    canvasLocation.set(0, 500);
                    if( XML.loadFile(file.getAbsolutePath()) ){
                        enemies.clear();
                        collectables.clear();
                        for (int i = 0; i < XML.getNumTags("enemy"); i++) {
                            ofPoint tmpPoint;
                            tmpPoint.set(XML.getValue("enemy:x", 0, i), XML.getValue("enemy:y", 0, i), XML.getValue("enemy:size", 0, i));
                            enemies.push_back(tmpPoint);
                        }
                        for (int i = 0; i < XML.getNumTags("collectables"); i++) {
                            ofPoint tmpPoint;
                            tmpPoint.set(XML.getValue("collectable:x", 0, i), XML.getValue("collectable:y", 0, i), XML.getValue("collectable:size", 0, i));
                            collectables.push_back(tmpPoint);
                        }
                        
                    }
                    else {
                        cout << "can't load file" << endl;
                    }
                }
                else {
                    cout << "it's not XML" << endl;

                }
                
                
            }
		
			
		}
            buttonCount = 0;
        }
        
    }
    
}

void testApp::savePattern() {
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
    
    
    XML.saveFile(patternInput->getTextString() + ".xml");
    XML.clear();

}

void testApp::processOpenFileSelection(ofFileDialogResult openFileResult){
	
	ofLogVerbose("getName(): "  + openFileResult.getName());
	ofLogVerbose("getPath(): "  + openFileResult.getPath());
	
	ofFile file (openFileResult.getPath());
	
	if (file.exists()){

		string fileExtension = ofToUpper(file.getExtension());
		
		if (fileExtension == "XML") {
		
        canvasLocation.set(0, 500);
        if( XML.loadFile(openFileResult.fileName) ){
            enemies.clear();
            collectables.clear();
            for (int i = 0; i < XML.getNumTags("enemy"); i++) {
                ofPoint tmpPoint;
                tmpPoint.set(XML.getValue("enemy:x", 0, i), XML.getValue("enemy:y", 0, i), XML.getValue("enemy:size", 0, i));
                enemies.push_back(tmpPoint);
            }
        }
        }
        
		
	}
	
}


void testApp::exit() {
    delete gui;
    delete levelGui;
    delete guiTabBar;
    
}