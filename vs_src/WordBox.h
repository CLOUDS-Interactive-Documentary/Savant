#pragma once

#include "ofMain.h"
#include "JitterFont.h"
#include "ofxTweener.h"

class WordBox {
public:
    WordBox();

    float width;
    float height;
    void update();
    void draw();
    void drawDebug();    
    void setText(string text);
    void addSample(float samplitude);

    void setDamageFactor(float factor);
    void setConfidenceVizFactor(float factor);
    void setConfidence(float confidence);
    void setPosition(float x, float y);
    ofPoint getPosition();
    void tweenTo(float x, float y, float durationSeconds);
private:
    ofPoint position;
    string _text;
    float _damageFactor;
    float _confidenceVizFactor;
    float _confidence;
    vector<float> samples;
    ofPath waveformPath;
    JitterFont jitterFont;
    // Nothing yet
    
    bool bGotResult;
    
    ofRectangle textBoundingBox;
    int indexOffset; // makes the waveform jitter
};
