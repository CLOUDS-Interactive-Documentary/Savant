#pragma once

// Optionally draw the cloud from the test video...
#define DRAW_CLOUD
#define LOOP_VIDEO

#include "CloudsVisualSystem.h"
#include "ofxGSTT.h"
#include "WordBox.h"
#include "ofxTweener.h"
#include <vector>

#ifdef AVF_PLAYER
// Nothing AVF-specfic yet
#else
#include "wav-file.h"
#include "WavFile.h"
#endif

//TODO: rename this to your own visual system
class CloudsVisualSystemSavant : public CloudsVisualSystem {
public:
    
	//TODO: Change this to the name of your visual system
	//This determines your data path so name it at first!
	//ie getVisualSystemDataPath() uses this
    string getSystemName(){
		return "Savant";
	}
    
	//These methods let us add custom GUI parameters and respond to their events
    void selfSetupGui();
    void selfGuiEvent(ofxUIEventArgs &e);
    
	//Use system gui for global or logical settings, for exmpl
    void selfSetupSystemGui();
    void guiSystemEvent(ofxUIEventArgs &e);
    
	//use render gui for display settings, like changing colors
    void selfSetupRenderGui();
    void guiRenderEvent(ofxUIEventArgs &e);
    
	// selfSetup is called when the visual system is first instantiated
	// This will be called during a "loading" screen, so any big images or
	// geometry should be loaded here
    void selfSetup();
    
	// selfBegin is called when the system is ready to be shown
	// this is a good time to prepare for transitions
	// but try to keep it light weight as to not cause stuttering
    void selfBegin();
    
	// selfPresetLoaded is called whenever a new preset is triggered
	// it'll be called right before selfBegin() and you may wish to
	// refresh anything that a preset may offset, such as stored colors or particles
	void selfPresetLoaded(string presetPath);
    
	//do things like ofRotate/ofTranslate here
	//any type of transformation that doesn't have to do with the camera
    void selfSceneTransformation();
	
	//normal update call
	void selfUpdate();
    
	// selfDraw draws in 3D using the default ofEasyCamera
	// you can change the camera by returning getCameraRef()
    void selfDraw();
	
    // draw any debug stuff here
	void selfDrawDebug();
    
	//draws through RGBD camera;
    //	void selfDrawRGBD();
	
	// or you can use selfDrawBackground to do 2D drawings that don't use the 3D camera
	void selfDrawBackground();
    
	// this is called when your system is no longer drawing.
	// Right after this selfUpdate() and selfDraw() won't be called any more
	void selfEnd();
    
	// this is called when you should clear all the memory and delet anything you made in setup
    void selfExit();
    
	//events are called when the system is active
	//Feel free to make things interactive for you, and for the user!
    void selfKeyPressed(ofKeyEventArgs & args);
    void selfKeyReleased(ofKeyEventArgs & args);
    
    void selfMouseDragged(ofMouseEventArgs& data);
    void selfMouseMoved(ofMouseEventArgs& data);
    void selfMousePressed(ofMouseEventArgs& data);
    void selfMouseReleased(ofMouseEventArgs& data);
	
    
    // if you use a custom camera to fly through the scene
	// you must implement this method for the transitions to work properly
	ofCamera& getCameraRef(){
        cloudsCamera.sideDistance = 200;
        cloudsCamera.frontDistance = 247.54;
		return cloudsCamera;
	}
	
protected:

    #ifdef DRAW_CLOUD
	ofShader pointcloudShader;
	ofVboMesh simplePointcloud;
    #endif
    
    // GUI Placeholder
	ofxUISuperCanvas* customGui;
	bool customToggle;
    bool bAddTestWord;
	float customFloat1;
	float customFloat2;
    
    // Speech Recognition
    // ugh, see: http://forum.openframeworks.cc/index.php/topic,8293.0.html
    void setupSpeechEngine();
    void destroySpeechEngine();
    void speechReceived(string & message);
    void startSpeechListener();
    void updateSpeechListener();
    void stopSpeechListener();
    bool speechListenerListening;
    float maxAmplitude;
    float timeOffsetSeconds;
    
    ofxGSTT gstt;
    void gsttResponse(ofxGSTTResponseArgs & response);

    bool bAudioReady;
    int lastRawAudioBufferLength; // It takes some time to read...
    int getSoundBufferIndexAtVideoPosition(float videoPosition);

    float damageFactor;
    float damageVizualizationFactor;
    float damageBackroundVizScaleFactor;
    float confidenceVizualizationFactor;
    bool bMouseYIsdamage;
    float randomRotateAmount;
    
    // renderer
    vector<WordBox> words;
    void addRandomWordBox();
    void updateWords();
    void drawWords();
    WordBox &getLatestBox();
    
    #ifdef AVF_PLAYER
    // Nothing AVF-specfic yet
    #else
    WavFileReader* pObjWavFile;
    short* videoSoundBuffer;
    int videoSoundBufferSize;
    #endif
    
    string testFile = "Jer_TestVideo";
    // string testFile = "Lauren_sentiment_anaylsis";
    // string testFile = "Fernanda_Sentiment_analysis";
    //string testFile = "Jen_data_autobiography";
    //string testFile = "Martin_verbal_programming_debate";
    
    void selfKeyDown(ofKeyEventArgs & args);
};
