#include "CloudsVisualSystemSavant.h"

#include "CloudsRGBDVideoPlayer.h"
#ifdef AVF_PLAYER
#include "ofxAVFVideoPlayer.h"
#endif

//These methods let us add custom GUI parameters and respond to their events
void CloudsVisualSystemSavant::selfSetupGui() {
	customGui = new ofxUISuperCanvas("Speech Recognition", gui);
	customGui->copyCanvasStyle(gui);
	customGui->copyCanvasProperties(gui);
	customGui->setName("Custom");
	customGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	
	customGui->addSlider("Custom Float 1", 1, 1000, &customFloat1);
	customGui->addSlider("Custom Float 2", 1, 1000, &customFloat2);
	customGui->addButton("Add Test Word", false);
	customGui->addButton("Sample Speech", false);
	customGui->addToggle("Map Mouse Y To Damage", &bMouseYIsdamage);
	customGui->addSlider("Sample Ahead (Seconds)", 0, 5000, &timeOffsetSeconds);
	customGui->addSlider("Damage Factor", 0, 1, &damageFactor);
	customGui->addSlider("Damage Viz Factor", 0, 1, &damageVizualizationFactor);
	customGui->addSlider("Damage Back Scale Factor", 0, 1, &damageBackroundVizScaleFactor);    
	customGui->addSlider("Confidence Viz Factor", 0, 20, &confidenceVizualizationFactor);
    
	
	ofAddListener(customGui->newGUIEvent, this, &CloudsVisualSystemSavant::selfGuiEvent);
	
	guis.push_back(customGui);
	guimap[customGui->getName()] = customGui;
}

void CloudsVisualSystemSavant::selfGuiEvent(ofxUIEventArgs &e) {
	if(e.widget->getName() == "Add Test Word"){
        if (e.getButton()->getValue()) {
            cout << "Button pressed!" << endl;
            addRandomWordBox();
        }
	}
	else if(e.widget->getName() == "Sample Speech"){
        if (e.getButton()->getValue()) {
            startSpeechListener();
        }
        else {
            stopSpeechListener();
        }
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
    

    
	if(ofFile::doesFileExist(getVisualSystemDataPath() + "TestVideo/" + testFile + ".mov")) {
		getRGBDVideoPlayer().setup(getVisualSystemDataPath() + "TestVideo/" + testFile + ".mov", getVisualSystemDataPath() + "TestVideo/" + testFile + ".xml" );
        
#ifdef AVF_PLAYER
        // Nothing to see here...
#else
        // Load sound manually
        string soundFileNameString = getVisualSystemDataPath() + "TestVideo/" + testFile + ".wav";
        char* soundFileName = (char *)soundFileNameString.c_str();
        
        pObjWavFile = new WavFileReader;
        pObjWavFile->openWavFile(soundFileName);
        if (EXIT_SUCCESS != pObjWavFile->openWavFile(soundFileName))   {
            cout<<"Can't load wav file: " << soundFileName << endl;
        }
        pObjWavFile->displayInformation(soundFileName);
        
        videoSoundBufferSize = pObjWavFile->getNumSamples();
        videoSoundBuffer = new short[videoSoundBufferSize];
        
        int index = 0;
        while (pObjWavFile->ifMoreDataAvailable()) {
            videoSoundBuffer[index] = pObjWavFile->readCurrentInput();
            index++;
        }
        
        
    #endif
        
        

        
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
    damageBackroundVizScaleFactor = 1;
    damageFactor = 1;
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
    Tweener.update();

    if (bMouseYIsdamage) {
        damageFactor = ofMap(ofGetMouseY(), 100, ofGetHeight(), 0, 1);
    }
    
#ifdef AVF_PLAYER
    
    // Watch for audio ready
    CloudsRGBDVideoPlayer &rgbdVideoPlayer = getRGBDVideoPlayer();
    ofxAVFVideoPlayer &avfVideoPlayer = rgbdVideoPlayer.getPlayer();
    
    if (!bAudioReady && (lastRawAudioBufferLength > 0) && (lastRawAudioBufferLength == avfVideoPlayer.getNumAmplitudes())) {
        // Done, buffer length is greater than zero and stable
        cout << "Audio ready" << endl;
        bAudioReady = true;
        
        // Test exporting video audio to wave.
        //        short* allAmplitudes = avfVideoPlayer.getAllAmplitudes();
        //        float* normalAmplitudes = new float[avfVideoPlayer.getNumAmplitudes()];
        //
        //        for (int i = 0; i < avfVideoPlayer.getNumAmplitudes(); i++) {
        //            normalAmplitudes[i] = allAmplitudes[i] / 32760.f;
        //        }
        //
        //        wav.setFormat(2, 44100, 16);
        //        wav.open(ofToDataPath(wavInput), WAVFILE_WRITE);
        //        wav.write(normalAmplitudes, avfVideoPlayer.getNumAmplitudes());
        //        wav.close();
        
    }
    else if (!bAudioReady) {
        lastRawAudioBufferLength = avfVideoPlayer.getNumAmplitudes();
        cout << "Unstable buffer length: " << lastRawAudioBufferLength << endl;
    }
    

    
    // Watch for video loop
    
#else
    
    // Work around, data loads synchronously in setup
    bAudioReady = true;
    
#endif
    
    
#ifdef LOOP_VIDEO
    if (getRGBDVideoPlayer().getPlayer().getPosition() >= 0.999) {
        cout << "LOOPING" << endl;
        getRGBDVideoPlayer().getPlayer().setPosition(0);
        getRGBDVideoPlayer().getPlayer().play();
    }
#endif
    
    if (speechListenerListening) updateSpeechListener();
    updateWords();
}


// selfDraw draws in 3D using the default ofEasyCamera
// you can change the camera by returning getCameraRef()
void CloudsVisualSystemSavant::selfDraw(){
    
#ifdef DRAW_CLOUD
    
    // Note hack to camera in camera constructor: CloudsRGBDCamera::CloudsRGBDCamera(); 
    
    ofPushMatrix();
	setupRGBDTransforms();

	pointcloudShader.begin();
	getRGBDVideoPlayer().setupProjectionUniforms(pointcloudShader);
	simplePointcloud.drawVertices();
	pointcloudShader.end();
    

    ofVec3f headPosition = getRGBDVideoPlayer().headPosition;
    ofVec3f offsetFromHeadPosition = ofVec3f((ofGetWidth() / -2) - 100, (-ofGetHeight() / 2) + 300, 0);
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(headPosition);
    ofTranslate(offsetFromHeadPosition);
    glDisable(GL_DEPTH_TEST);
    drawWords();
    glEnable(GL_DEPTH_TEST);
    ofPopMatrix();
    ofPopStyle();
    
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
    
    
    // Draw noise grid
   
    if (damageVizualizationFactor > 0) {
    ofPushStyle();
        // ofEnableAlphaBlending();
        //  ofPushMatrix();
        //        ofScale(damageBackroundVizScaleFactor, damageBackroundVizScaleFactor);
        
        float boxSize = damageBackroundVizScaleFactor * 100;
        
    for (int x = -boxSize; x < ofGetWidth() + boxSize; x += 20) {
        for (int y = -boxSize; y < ofGetHeight() + boxSize; y += 20) {

            int grayLevel = (255 * ofRandom(1) * ofRandom(1) * damageFactor * damageVizualizationFactor);
            
               ofSetColor(grayLevel, grayLevel, grayLevel);
                  ofFill();
            ofPushMatrix();
            
            ofTranslate(x + (ofGetFrameNum() % 40), y + (ofGetFrameNum() % 20));
            
            ofTranslate(-boxSize/2, -boxSize/2);
            ofRotate(ofRandom(360));
            //ofScale(1 + ofRandom(damageFactor), 1 + ofRandom(damageFactor));
            ofTranslate(boxSize, boxSize);
            
            ofRect(0, 0, boxSize, boxSize);

            
            ofPopMatrix();
        }
    }
        
        //        ofPopMatrix();

        // ofDisableAlphaBlending();
    ofPopStyle();
    }
    
}
// this is called when your system is no longer drawing.
// Right after this selfUpdate() and selfDraw() won't be called any more
void CloudsVisualSystemSavant::selfEnd(){
    
#ifdef DRAW_CLOUD
	simplePointcloud.clear();
#endif
    
    // Reset this, get fresh event if we start back
    
    if (bAddTestWord) {
        addRandomWordBox();
    }
    
    bAudioReady = false;
}

// this is called when you should clear all the memory and delet anything you made in setup
void CloudsVisualSystemSavant::selfExit(){
    destroySpeechEngine();
}

//events are called when the system is active
//Feel free to make things interactive for you, and for the user!

int lastKey;

void CloudsVisualSystemSavant::selfKeyPressed(ofKeyEventArgs & args){
    
    if (lastKey != args.key) {
        selfKeyDown(args);
    }
    
    lastKey = args.key;
}

void CloudsVisualSystemSavant::selfKeyDown(ofKeyEventArgs & args){
    if (args.key == 'z') {
        startSpeechListener();
    }
}

void CloudsVisualSystemSavant::selfKeyReleased(ofKeyEventArgs & args){
    if (args.key == 'z') {
        stopSpeechListener();
    }
    
    lastKey = -1;
}

void CloudsVisualSystemSavant::selfMouseDragged(ofMouseEventArgs& data){
	
}

void CloudsVisualSystemSavant::selfMouseMoved(ofMouseEventArgs& data){
	
}

void CloudsVisualSystemSavant::selfMousePressed(ofMouseEventArgs& data){
	//startSpeechListener();
    
//    for (std::vector<WordBox>::size_type i = 0; i < words.size(); i++) {
//        words[i].tweenTo(ofGetMouseX(), ofGetMouseY(), ofRandom(1, 5));
//    }
    
}

void CloudsVisualSystemSavant::selfMouseReleased(ofMouseEventArgs& data){
	//stopSpeechListener();
}



#pragma mark - New Methods


#pragma mark -- Speech To Text

int speechStartedListeningIndex;
int speechLastUpdateIndex;

void CloudsVisualSystemSavant::setupSpeechEngine() {
    speechListenerListening = false;
    gstt.setup();
	ofAddListener(gsttApiResponseEvent, this, &CloudsVisualSystemSavant::gsttResponse);
}

void CloudsVisualSystemSavant::destroySpeechEngine() {
	ofRemoveListener(gsttApiResponseEvent,this,&CloudsVisualSystemSavant::gsttResponse);
}

void CloudsVisualSystemSavant::startSpeechListener() {
    speechLastUpdateIndex = speechStartedListeningIndex = getSoundBufferIndexAtVideoPosition(getRGBDVideoPlayer().getPlayer().getPosition());
    speechListenerListening = true;
    gstt.startListening();
    addRandomWordBox(); // add box to represent this
    getLatestBox().setDamageFactor(damageFactor);
}

float* currentChunkBuffer = new float[20000];

void CloudsVisualSystemSavant::updateSpeechListener() {
    int speechCurrentUpdateIndex;
    short* allAmplitudes;
    int allAmplitudesCount;
    
#ifdef AVF_PLAYER
    
    // Get listening position, find range since last update....
	ofxAVFVideoPlayer &avfVideoPlayer = getRGBDVideoPlayer().getPlayer();
    speechCurrentUpdateIndex = getSoundBufferIndexAtVideoPosition(avfVideoPlayer.getPosition());
    allAmplitudesCount = avfVideoPlayer.getNumAmplitudes();
    allAmplitudes = avfVideoPlayer.getAllAmplitudes();
    
#else
    
    allAmplitudesCount = videoSoundBufferSize;
    allAmplitudes = videoSoundBuffer;
    
    // Alt implementation
	ofVideoPlayer &videoPlayer = getRGBDVideoPlayer().getPlayer();
    speechCurrentUpdateIndex = getSoundBufferIndexAtVideoPosition(videoPlayer.getPosition());
    
#endif
    
    int startSoundIndex = speechLastUpdateIndex;
    int endSoundIndex = speechCurrentUpdateIndex; // TODO round down to buffer size...
    
    // Time shift for look-ahead
    int indexOffset = timeOffsetSeconds * 441000 * 2; // times 2 for num channels?
    
    // If index offset overshoots...
    startSoundIndex = min(startSoundIndex + indexOffset, (2 * allAmplitudesCount - 1));
    endSoundIndex = min(endSoundIndex + indexOffset, (2 * allAmplitudesCount - 1));
    
    // Todo, what's the minimum sample size? Need to break for that too.
    if (startSoundIndex == endSoundIndex) {
        ofLogWarning("Index offset would overshoot buffer, stopping recording");
        stopSpeechListener();
        return;
    }
    
    int currentChunkBufferSize = endSoundIndex - startSoundIndex;
    
    // Get chunk of audio from video (and normalize)
    //cout << "Chunk buffer size: " << currentChunkBufferSize << endl;
    //float* currentChunkBuffer = new float[currentChunkBufferSize]; // reuse this to avoid allocation
    
    for (int i = 0; i < currentChunkBufferSize; i++) {
        
        if (damageFactor > 0) {
        currentChunkBuffer[i] = ((allAmplitudes[startSoundIndex + i]) / 32760.f) * (1 + (ofRandom(-damageFactor, damageFactor)));
        }
        else {
        currentChunkBuffer[i] = ((allAmplitudes[startSoundIndex + i]) / 32760.f);
        }
        
        getLatestBox().addSample(currentChunkBuffer[i]);
    }
    
    
    gstt.addAudio(currentChunkBuffer, currentChunkBufferSize, 2, 44100);
    speechLastUpdateIndex = speechCurrentUpdateIndex;
}

void CloudsVisualSystemSavant::stopSpeechListener() {
    cout << "Finished listening" << endl;
    speechListenerListening = false;
    gstt.stopListening(); // and try to process
}

void CloudsVisualSystemSavant::gsttResponse(ofxGSTTResponseArgs & response){
	cout << "Response: " << response.msg << endl << "with confidence: " << ofToString(response.confidence) << endl;
    getLatestBox().setText(response.msg);
    getLatestBox().setConfidence(response.confidence);

    
    //getLatestBox().tweenTo(getLatestBox().getPosition().x, getLatestBox().getPosition().y - 2000, 40);
}

#pragma mark -- Helpers

int CloudsVisualSystemSavant::getSoundBufferIndexAtVideoPosition(float videoPosition) {
    
#ifdef AVF_PLAYER
    CloudsRGBDVideoPlayer &rgbdVideoPlayer = getRGBDVideoPlayer();
	ofxAVFVideoPlayer &avfVideoPlayer = rgbdVideoPlayer.getPlayer();
    return MIN(floor(videoPosition * avfVideoPlayer.getNumAmplitudes()) * 2, 2 * avfVideoPlayer.getNumAmplitudes() - 1);
#else
    return MIN(floor(videoPosition * videoSoundBufferSize), videoSoundBufferSize - 1);
#endif
}




#pragma mark -- Renderer

void CloudsVisualSystemSavant::addRandomWordBox() {
    // Build a fake word box for testing
    
    WordBox wordBox;
    wordBox.setPosition(50, 20);
    wordBox.width = ofGetWidth() - 100;
    wordBox.height = 180;
    //wordBox.setText(string("Bla bla bla"));
    words.push_back(wordBox);
}

void CloudsVisualSystemSavant::updateWords() {
    for (std::vector<WordBox>::size_type i = 0; i < words.size(); i++) {
        words[i].setConfidenceVizFactor(confidenceVizualizationFactor);
        words[i].update();
    }
}

void CloudsVisualSystemSavant::drawWords() {
    // Lay the out, newest at top
//    float yOffset = 20;
//    for (int i = (words.size() - 1); i >= 0; i--) {
//        words[i].setPosition(50, yOffset);
//        yOffset += 200;
//    }
    
    for (std::vector<WordBox>::size_type i = 0; i < words.size(); i++) {
        words[i].draw();
        if (bDebug) words[i].drawDebug();        
    }
}

WordBox& CloudsVisualSystemSavant::getLatestBox() {
    
    
    
    if (words.size() > 0) {
        return words[words.size() - 1];
    }
}

