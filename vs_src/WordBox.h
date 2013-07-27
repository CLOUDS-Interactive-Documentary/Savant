#pragma once

#include "ofMain.h"
#include "JitterFont.h"

class WordBox {
public:
    WordBox();
    float x;
    float y;
    float width;
    float height;
    void update();
    void draw();
    void setText(string text);
    void setConfidence(float confidence);
    void addSample(float samplitude);
    
private:
    string _text;
    float _confidence;
    vector<float> samples;
    ofPath waveformPath;
    JitterFont jitterFont;
    // Nothing yet
};
