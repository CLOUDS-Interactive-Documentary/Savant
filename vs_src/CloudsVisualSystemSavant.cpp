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
void CloudsVisualSystemSavant::selfSetup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    
    if(ofFile::doesFileExist(getVisualSystemDataPath() + "TestVideo/Jer_TestVideo.mov")){
		getRGBDVideoPlayer().setup(getVisualSystemDataPath() + "TestVideo/Jer_TestVideo.mov",
								   getVisualSystemDataPath() + "TestVideo/Jer_TestVideo.xml" );
		
		getRGBDVideoPlayer().swapAndPlay();
        
#ifdef DRAW_CLOUD
		for(int i = 0; i < 640; i += 2){
			for(int j = 0; j < 480; j+=2){
				simplePointcloud.addVertex(ofVec3f(i,j,0));
			}
		}
		
		pointcloudShader.load(getVisualSystemDataPath() + "shaders/rgbdcombined");
#endif
		
	}
    
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
    // Reset this
    ofLogVerbose("Self Begin");
    bAudioReady = false;
}

//do things like ofRotate/ofTranslate here
//any type of transformation that doesn't have to do with the camera
void CloudsVisualSystemSavant::selfSceneTransformation(){
	
}

//normal update call
void CloudsVisualSystemSavant::selfUpdate(){
    
    // Watch for audio ready
    CloudsRGBDVideoPlayer &rgbdVideoPlayer = getRGBDVideoPlayer();
    ofxAVFVideoPlayer &avfVideoPlayer = rgbdVideoPlayer.getPlayer();

    if (!bAudioReady && (lastRawAudioBufferLength > 0) && (lastRawAudioBufferLength == avfVideoPlayer.getNumAmplitudes())) {
        // Done, buffer length is greater than zero and stable
        cout << "Audio ready" << endl;
        bAudioReady = true;
        prepareAudioBuffer();
    }
    else if (!bAudioReady) {
        lastRawAudioBufferLength = avfVideoPlayer.getNumAmplitudes();
        cout << "Unstable buffer length: " << lastRawAudioBufferLength << endl;
    }

    if (speechListenerListening) updateSpeechListener();
}


// selfDraw draws in 3D using the default ofEasyCamera
// you can change the camera by returning getCameraRef()
void CloudsVisualSystemSavant::selfDraw(){
    
    #ifdef DRAW_CLOUD
    ofPushMatrix();
	setupRGBDTransforms();
	pointcloudShader.begin();
	getRGBDVideoPlayer().setupProjectionUniforms(pointcloudShader);
	simplePointcloud.drawVertices();
	pointcloudShader.end();
    ofPopMatrix();
    #endif
    
}

// draw any debug stuff here
void CloudsVisualSystemSavant::selfDrawDebug(){
	
}

// or you can use selfDrawBackground to do 2D drawings that don't use the 3D camera
void CloudsVisualSystemSavant::selfDrawBackground(){
    
	//turn the background refresh off
	bClearBackground = true;
    

    
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
    
    // infer sample rate
    //float sampleRate = avfVideoPlayer.getNumAmplitudes() / avfVideoPlayer.getDuration();
    //ofLogVerbose("Sample rate?: " + ofToString(sampleRate));
    
    
    
    //cout << "audioIndex: " << audioIndex << endl;
    
    //myfft.powerSpectrum(idx,(int)BUFFER_SIZE/2, p->getAllAmplitudes(), BUFFER_SIZE, &magnitude[0],&phase[0],&power[0],&avg_power);
    
    
    
}
// this is called when your system is no longer drawing.
// Right after this selfUpdate() and selfDraw() won't be called any more
void CloudsVisualSystemSavant::selfEnd(){
    #ifdef DRAW_CLOUD
	simplePointcloud.clear();
    #endif
    
    // Reset this, get fresh event if we start back
    bAudioReady = false;
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
	startSpeechListener();
}

void CloudsVisualSystemSavant::selfMouseReleased(ofMouseEventArgs& data){
	stopSpeechListener();
}



// Visual System Specific Methods -----------------



// Speech Control ------------------------------------

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
        ofLogError("ASR Engine initial failed. Error Code: " + ofToString(startCode));
		speechEngineResults = "ASR Engine initial failed. Check sphinx resource path";
    }
    
    // Initialize resampler
    int resampleQuality = 1; // 1 is "high"
    double minResampleFactor = 0.2; // conversion factor... e.g. 44 / 16
    double maxResampleFactor = 0.7; // conversion factor...
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


int speechStartedListeningIndex;
int speechLastUpdateIndex;

void CloudsVisualSystemSavant::startSpeechListener() {
    int retval = speechEngine->engineOpen();
    if (retval != OFXASR_SUCCESS) {
        ofLogError("ASR Engine failed to open. Error Code: " + ofToString(retval));
    }
    
    // Get references
    CloudsRGBDVideoPlayer &rgbdVideoPlayer = getRGBDVideoPlayer();
	ofxAVFVideoPlayer &avfVideoPlayer = rgbdVideoPlayer.getPlayer();
    speechLastUpdateIndex = speechStartedListeningIndex = getSoundBufferIndexAtVideoPosition(avfVideoPlayer.getPosition());
    
    speechListenerListening = true;
}


void CloudsVisualSystemSavant::updateSpeechListener() {
    // Get listening position, find range since last update....
    CloudsRGBDVideoPlayer &rgbdVideoPlayer = getRGBDVideoPlayer();
	ofxAVFVideoPlayer &avfVideoPlayer = rgbdVideoPlayer.getPlayer();
    int speechCurrentUpdateIndex = getSoundBufferIndexAtVideoPosition(avfVideoPlayer.getPosition());
    


    
    int startSoundIndex = speechLastUpdateIndex;
    int endSoundIndex = speechCurrentUpdateIndex; // TODO round down to buffer size...
    int speechAudioBufferSize = endSoundIndex - startSoundIndex;

    cout << "Start: " << startSoundIndex << " End: " << endSoundIndex << endl;
    
    // Pull in the correct chunk of buffer from the video
    short *speechAudioBuffer = new short[speechAudioBufferSize];
    
    for (int i = 0; i < speechAudioBufferSize; i++) {
        speechAudioBuffer[i] = downsampledAudioBuffer[startSoundIndex + i];
        cout << speechAudioBuffer[i] << endl;
    }
    
    // Send it to the engine // TODO correct for frame misalignment
    speechEngine->engineSentAudio(speechAudioBuffer, speechAudioBufferSize);
    
    // Update last index... // TODO correct for frame misalignment
    speechLastUpdateIndex = speechCurrentUpdateIndex;
    
}



void CloudsVisualSystemSavant::stopSpeechListener() {
    speechListenerListening = false;
    
    // Close the engines
    int retval;
    retval = speechEngine->engineClose();
    
    if (retval != OFXASR_SUCCESS) {
        ofLogError("Speech engine failed to close. Error code: " + ofToString(retval));
    }
    
    // Get results
	char *results = speechEngine->engineGetText();
	if (results) {
		string s1(results);
		speechEngineResults = s1;
        ofLogVerbose("Got words: " + ofToString(speechEngineResults));
	}
	else {
        ofLogVerbose("No result from the speech engine.");
	}
}

int CloudsVisualSystemSavant::getSoundBufferIndexAtVideoPosition(float videoPosition) {
    return MIN(floor(videoPosition * downsampledAudioBufferLength), downsampledAudioBufferLength - 1);
}

void CloudsVisualSystemSavant::prepareAudioBuffer() {
    // Do the sound conversion!
    CloudsRGBDVideoPlayer &rgbdVideoPlayer = getRGBDVideoPlayer();
	ofxAVFVideoPlayer &avfVideoPlayer = rgbdVideoPlayer.getPlayer();
    
    // Get the raw audio buffer from the video player (Always 44.1 khz?)
    int rawAudioBufferSize = avfVideoPlayer.getNumAmplitudes();
    float *rawAudioBuffer = avfVideoPlayer.getAllAmplitudes();
    
    
    
    int sourceSampleRate = 44100; // TODO calculate this
    int speechSampleRate = 16000; // todo make class const
    double resampleFactor = (double)speechSampleRate / (double)sourceSampleRate; // Todo cache this
    
    // Prep the intermediate buffer (correct length, but floats instead of shorts)
    int expectedResultBufferSize = ceil((double)rawAudioBufferSize * resampleFactor);
    float *tempDownsampleBuffer = new float[expectedResultBufferSize];

    ofLogVerbose("resample factor: " + ofToString(resampleFactor));
    ofLogVerbose("starting audio buffer size: " + ofToString(rawAudioBufferSize));
    ofLogVerbose("expectedResultBufferSize: " + ofToString(expectedResultBufferSize));
    
    // Downsample from 44.1 to 16 khz.
    int srcUsed; // What ees this? Bytes actually used?
    resample_process(resampleHandle, resampleFactor, rawAudioBuffer, rawAudioBufferSize, 1, &srcUsed, tempDownsampleBuffer, expectedResultBufferSize);
    ofLogVerbose("srcUsed: " + ofToString(srcUsed));
    
    // Clear the old
    downsampledAudioBuffer = NULL; // todo clean this?
    downsampledAudioBuffer = new short[expectedResultBufferSize];

    // Convert from float to signed short for the sound engine (16 bit PCM) // TODO correct for frame misalignment?
    for (int i = 0; i < expectedResultBufferSize; i++) {
        downsampledAudioBuffer[i] = short(tempDownsampleBuffer[i] * 0.5);
        //downsampledAudioBuffer[i] = short(tempDownsampleBuffer[i] * 32767.5 - 0.5);
        // -32768 to 32767
        if ((i % 100) == 0) cout << "Buffer: " << tempDownsampleBuffer[i] << "    " << downsampledAudioBuffer[i] << endl;
    }
    downsampledAudioBufferLength = expectedResultBufferSize;
}

/*
void CloudsVisualSystemSavant::videoStartedPlaying() {
    ofLogVerbose("Video Started Playing");
    

}

void CloudsVisualSystemSavant::videoStoppedPlaying() {
    ofLogVerbose("Video Stopped Playing");
}
*/





