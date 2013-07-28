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
    void setText(string text);
    void setConfidence(float confidence);
    void addSample(float samplitude);

    void setPosition(float x, float y);
    void tweenTo(float x, float y, float durationSeconds);
private:
    ofPoint position;
    string _text;
    float _confidence;
    vector<float> samples;
    ofPath waveformPath;
    JitterFont jitterFont;
    // Nothing yet
    
    ofRectangle textBoundingBox;
    int indexOffset; // makes the waveform jitter
};
