//
//  CloudsVisualSystemSavant.cpp
//

#include "CloudsVisualSystemSavant.h"


#include "CloudsRGBDVideoPlayer.h"
#ifdef AVF_PLAYER
#include "ofxAVFVideoPlayer.h"
#endif

//These methods let us add custom GUI parameters and respond to their events
void CloudsVisualSystemSavant::selfSetupGui() {
	customGui = new ofxUISuperCanvas("CUSTOM", gui);
	customGui->copyCanvasStyle(gui);
	customGui->copyCanvasProperties(gui);
	customGui->setName("Custom");
	customGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	
	customGui->addSlider("Custom Float 1", 1, 1000, &customFloat1);
	customGui->addSlider("Custom Float 2", 1, 1000, &customFloat2);
	customGui->addButton("Custom Button", false);
	customGui->addToggle("Custom Toggle", &customToggle);
	
	ofAddListener(customGui->newGUIEvent, this, &CloudsVisualSystemSavant::selfGuiEvent);
	
	guis.push_back(customGui);
	guimap[customGui->getName()] = customGui;
}

void CloudsVisualSystemSavant::selfGuiEvent(ofxUIEventArgs &e) {
	if(e.widget->getName() == "Custom Button"){
		cout << "Button pressed!" << endl;
	}
}

//Use system gui for global or logical settings, for exmpl
void CloudsVisualSystemSavant::selfSetupSystemGui(){
	
}

void CloudsVisualSystemSavant::guiSystemEvent(ofxUIEventArgs &e){
	
}
//use render gui for display settings, like changing colors
void CloudsVisualSystemSavant::selfSetupRenderGui(){
    
}

void CloudsVisualSystemSavant::guiRenderEvent(ofxUIEventArgs &e){
	
}

// selfSetup is called when the visual system is first instantiated
// This will be called during a "loading" screen, so any big images or
// geometry should be loaded here
void CloudsVisualSystemSavant::selfSetup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
	loadTestVideo();
    
    
    // Create speech engine
    setupSpeechEngine();
}

// selfPresetLoaded is called whenever a new preset is triggered
// it'll be called right before selfBegin() and you may wish to
// refresh anything that a preset may offset, such as stored colors or particles
void CloudsVisualSystemSavant::selfPresetLoaded(string presetPath){
	
}

// selfBegin is called when the system is ready to be shown
// this is a good time to prepare for transitions
// but try to keep it light weight as to not cause stuttering
void CloudsVisualSystemSavant::selfBegin(){
	
}

//do things like ofRotate/ofTranslate here
//any type of transformation that doesn't have to do with the camera
void CloudsVisualSystemSavant::selfSceneTransformation(){
	
}

//normal update call
void CloudsVisualSystemSavant::selfUpdate(){
    
}
// selfDraw draws in 3D using the default ofEasyCamera
// you can change the camera by returning getCameraRef()
void CloudsVisualSystemSavant::selfDraw(){
	
}

// draw any debug stuff here
void CloudsVisualSystemSavant::selfDrawDebug(){
	
}
// or you can use selfDrawBackground to do 2D drawings that don't use the 3D camera
void CloudsVisualSystemSavant::selfDrawBackground(){
    
	//turn the background refresh off
	bClearBackground = true;

    // Get references
    CloudsRGBDVideoPlayer &rgbdVideoPlayer = getRGBDVideoPlayer();
	ofxAVFVideoPlayer &avfVideoPlayer = rgbdVideoPlayer.getPlayer();

    // Draw video
    ofSetColor(ofColor::white);
    ofFill();
//    avfVideoPlayer.play();
//    avfVideoPlayer.update();
    avfVideoPlayer.draw(300, 0);
    
    // Update from sound buffer
    //avfVideoPlayer

    //int audioIndex = MIN(floor(avfVideoPlayer.getPosition() * avfVideoPlayer.getNumAmplitudes()), avfVideoPlayer.getNumAmplitudes() - 1);
    
    
    
    //cout << "audioIndex: " << audioIndex << endl;
    
    //myfft.powerSpectrum(idx,(int)BUFFER_SIZE/2, p->getAllAmplitudes(), BUFFER_SIZE, &magnitude[0],&phase[0],&power[0],&avg_power);
    
    
    
}
// this is called when your system is no longer drawing.
// Right after this selfUpdate() and selfDraw() won't be called any more
void CloudsVisualSystemSavant::selfEnd(){
	
}
// this is called when you should clear all the memory and delet anything you made in setup
void CloudsVisualSystemSavant::selfExit(){
    destroySpeechEngine();
}

//events are called when the system is active
//Feel free to make things interactive for you, and for the user!
void CloudsVisualSystemSavant::selfKeyPressed(ofKeyEventArgs & args){
	
}
void CloudsVisualSystemSavant::selfKeyReleased(ofKeyEventArgs & args){
	
}

void CloudsVisualSystemSavant::selfMouseDragged(ofMouseEventArgs& data){
	
}

void CloudsVisualSystemSavant::selfMouseMoved(ofMouseEventArgs& data){
	
}

void CloudsVisualSystemSavant::selfMousePressed(ofMouseEventArgs& data){
	
}

void CloudsVisualSystemSavant::selfMouseReleased(ofMouseEventArgs& data){
	
}



// Visual System Specific Methods -----------------



void CloudsVisualSystemSavant::setupSpeechEngine() {
    speechEngine = new ofxSphinxASR;
    speechEngineArgs = new ofAsrEngineArgs;
    speechEngineArgs->samplerate = 16000; // Hz
	speechEngineArgs->sphinx_mode = 4; // Mode 4 is for continuous recognition
    
    // Set up data, still finding the best ones
    //    e->sphinxmodel_am = ofToDataPath("sphinxmodel1/digit_8gau");
    //    e->sphinxmodel_lm = ofToDataPath("sphinxmodel1/digit.lm.DMP");
    //    e->sphinxmodel_dict = ofToDataPath("sphinxmodel1/dictionary");
    //    e->sphinxmodel_fdict = ofToDataPath("sphinxmodel1/fillerdict");
    
    //    e->sphinxmodel_am = ofToDataPath("sphinxmodel2/Communicator_40.cd_cont_4000");
    //    e->sphinxmodel_lm = ofToDataPath("sphinxmodel2/language_model.arpaformat.DMP");
    //    e->sphinxmodel_dict = ofToDataPath("sphinxmodel2/cmudict.hub4.06d.dict");
    //    e->sphinxmodel_fdict = ofToDataPath("sphinxmodel2/fillerdict");
    
    //    e->sphinxmodel_am = ofToDataPath("sphinxmodel3/wsj_all_cd30.mllt_cd_cont_4000");
    //    e->sphinxmodel_lm = ofToDataPath("sphinxmodel3/language_model.arpaformat.DMP");
    //    e->sphinxmodel_dict = ofToDataPath("sphinxmodel3/cmudict.hub4.06d.dict");
    //    e->sphinxmodel_fdict = ofToDataPath("sphinxmodel3/fillerdict");
    
    speechEngineArgs->sphinxmodel_am = getVisualSystemDataPath() + "speech_models/sphinxmodel4/sphinxmodel_voxforge-en-r0_1_3";
    speechEngineArgs->sphinxmodel_lm = getVisualSystemDataPath() + "speech_models/sphinxmodel4/voxforge_en_sphinx.lm.DMP";
    speechEngineArgs->sphinxmodel_dict = getVisualSystemDataPath() + "speech_models/sphinxmodel4/cmudict.0.7a";
    speechEngineArgs->sphinxmodel_fdict = getVisualSystemDataPath() + "speech_models/sphinxmodel4/noisedict";
    
    // Initialize the engine
    int startCode = speechEngine->engineInit(speechEngineArgs);
    if (startCode == OFXASR_SUCCESS) {
        ofLogVerbose("Speech Engine Started Successfully");
    }
    else {
        printf("ASR Engine initial failed. Error Code: %d\n", startCode);
		speechEngineResults = "ASR Engine initial failed. Check sphinx resource path";
    }
    
    // Initialize resampler
    int resampleQuality = 1; // 1 is "high"
    double minResampleFactor = 0.5; // What's good here? TODO
    double maxResampleFactor = 0.5; // What's good here? TODO
    resampleHandle = resample_open(resampleQuality, minResampleFactor, maxResampleFactor);    
}

void CloudsVisualSystemSavant::destroySpeechEngine() {
    if (speechEngine != NULL) {
        speechEngine->engineExit();
        delete speechEngine;
        speechEngine = NULL;
    }

    if (speechEngineArgs != NULL) {
        delete speechEngineArgs;
        speechEngineArgs = NULL;
    }
    
    // And associated resampling
    resample_close(resampleHandle);
}





