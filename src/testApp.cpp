#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofEnableAlphaBlending();
    guiTabBar = new ofxUITabBar();

    loadedFile = "pattern.xml";
    testingSpeed = false;
	if( XML.loadFile(loadedFile) ){
    }
    /*
    else {

        int tagNum = XML.addTag("level");
        XML.setValue("level:name", "New Level", tagNum);
        XML.setValue("level:speed", 1, tagNum);
    }
*/

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
    buttonCount = 0;
    gui = new ofxUICanvas(320, ofGetHeight());
    levelGui = new ofxUICanvas(320, ofGetHeight());

    vector<string> hnames;
    hnames.push_back("ENEMIES");
    hnames.push_back("COLLECTABLES");
    levelGui->addLabel("Level Name");
    levelInput = levelGui->addTextInput("LEVEL", "Name of Level");
    ofxUILabelButton *openButton = gui->addLabelButton("OPEN EXISTING PATTERN", "OPEN");
    patternInput = gui->addTextInput("PATTERN", "pattern0");
    gui->addSpacer();
    levelGui->addLabel("Author");
    levelGui->addTextInput("AUTHOR", "Your Name");
    levelGui->addSpacer(320, 10);
    levelGui->addLabel("Speed");
    levelGui->addSlider("Starting", -.3, -9, -1, 150, 80);
    levelGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	levelGui->addSlider("Multiplier", 1, 3, 1, 150, 80);
	levelGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    levelGui->addSpacer(320, 20);

//    levelGui->setWidgetFontSize(OFX_UI_FONT_SMALL);

    patternList = levelGui->addDropDownList("Included Patterns", patterns, 150);
//    levelGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
//    ofxUILabelButton *removeButton = (ofxUILabelButton *) levelGui->addLabelButton("REMOVE PATTERN", "REMOVE PATTERN");
    levelGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);

    removeButton = (ofxUILabelButton *) levelGui->addLabelButton("REMOVE PATTERN", "REMOVE PATTERN", 150, 15);
    removeButton->setVisible(false);
    levelGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    levelGui->addSpacer(320, 200);
    levelGui->addLabelButton("SAVE LEVEL", "LEVEL", 310, 40);

    //    levelGui->addWidgetEastOf(new ofxUILabelButton("REMOVE PATTERN", "REMOVE PATTERN"), "REMOVE PATTERN BUTTON");
    //    gui->addWidgetEastOf(new ofxUISlider("0", 0.0, 255.0, 150, dim, 190), "PAD");

    //	levelGui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    

	gui->addLabel("Placing");
	ofxUIRadio *radio = gui->addRadio("PLACING", hnames, OFX_UI_ORIENTATION_HORIZONTAL);
    radio->activateToggle("COLLECTABLES");
    gui->addSpacer(320,10);
    gui->addLabel("Object Size");
    ofxUISlider *sizeSlider = gui->addBiLabelSlider("SIZE", "TINY", "LARGE", SPACER, 160, pointSize, 310, 10);
    sizeSlider->setIncrement(SPACER);

    
    
    sizeSlider->setIncrement(SPACER);
    gui->addSpacer(320, 40, "SPACE4");
    gui->addTextArea("INSTRUCTIONS4", "ARROW KEYS move an object");
    gui->addTextArea("INSTRUCTIONS2", "SPACE places an object");
    gui->addTextArea("INSTRUCTIONS3", "BACKSPACE removes the object");
    gui->addTextArea("INSTRUCTIONS4", "[ and ] moves the canvas position");

    gui->addSpacer(320, 30, "SPACE1");
    ofxUILabelButton *clearButton = gui->addLabelButton("CLEAR PATTERN", "CLEAR");
    ofxUILabelButton *saveButton = gui->addLabelButton("SAVE PATTERN", "SAVE");
    gui->addLabelButton("ADD TO LEVEL", "ADD TO LEVEL");


    gui->setName("Pattern");
    levelGui->setName("Level");
    guiTabBar->addCanvas(levelGui);
    guiTabBar->addCanvas(gui);
    guiTabBar->setPosition(250, 0);
	//load a monospaced font
	//which we will use to show part of the xml structure
    currentLocation.set(160, ofGetHeight()/2);
    canvasLocation.set(0, 500);
    
//    startLine.x = currentLocation.x - canvasLocation.x;
    startLine.y = 0;
    pointSize = 40;
    creatingEnemies = false;
    patternNumber = 0;
    ofSetRectMode(OF_RECTMODE_CORNER);
    startingTest.set(ofGetHeight()/2, ofGetWidth()/2);
    currentTest.set(startingTest.x, startingTest.y);
    endingTest.set(ofGetHeight(), ofGetWidth()/2);
    multiplier = 1;
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
    if (currentTest.y > ofGetHeight()) {
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
    
    if (mouseX <= ofGetWidth()/2) {
        ofSetColor(0, 255, 0);
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofRect(mouseX, mouseY, 25, 25);
        ofSetRectMode(OF_RECTMODE_CORNER);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    //no data gets saved unless you hit the s key

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
    
    if(name == "CLEAR PATTERN") {
        buttonCount++;
        if(buttonCount ==2) {
            enemies.clear();
            collectables.clear();
            XML.clear();
        }
    }

    if (name == "SAVE PATTERN") {
        buttonCount++;
        if (buttonCount == 2) {
            savePattern();
            //            patternNumber++;
//            patternInput->setTextString("pattern"+ofToString(patternNumber));
            buttonCount = 0;
        }
    }
    if (name == "ADD TO LEVEL") {
        buttonCount++;
        if (buttonCount == 2) {
            patterns.push_back(patternInput->getTextString() + ".xml");
            patternList->addToggle(patternInput->getTextString());
            savePattern();
            buttonCount = 0;
        }
    }
    if (name == "REMOVE PATTERN") {
        
    }
    if (name == "SAVE LEVEL") {
    
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
                //Limiting this example to one image so we delete previous ones
                
                string fileExtension = ofToUpper(file.getExtension());
                
                //We only want images
                if (fileExtension == "XML") {
                    cout << "it's XML" << " " << file.getAbsolutePath() << endl;
                    patternInput->setTextString(file.getBaseName());
//                    myString.substr(0, myString.size()-1);
                    canvasLocation.set(0, 500);
                    if( XML.loadFile(file.getAbsolutePath()) ){
                        enemies.clear();
                        collectables.clear();
                        for (int i = 0; i < XML.getNumTags("enemy"); i++) {
                            ofPoint tmpPoint;
                            tmpPoint.set(XML.getValue("enemy:x", 0, i), XML.getValue("enemy:y", 0, i), XML.getValue("enemy:size", 0, i));
                            enemies.push_back(tmpPoint);
                            cout << "creating enemy"<< endl;
                        }
                        for (int i = 0; i < XML.getNumTags("collectables"); i++) {
                            ofPoint tmpPoint;
                            tmpPoint.set(XML.getValue("collectable:x", 0, i), XML.getValue("collectable:y", 0, i), XML.getValue("collectable:size", 0, i));
                            collectables.push_back(tmpPoint);
                            cout << "creating collectable" << endl;
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
		//Limiting this example to one image so we delete previous ones

		string fileExtension = ofToUpper(file.getExtension());
		
		//We only want images
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
    
}