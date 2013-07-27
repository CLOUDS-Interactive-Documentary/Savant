#pragma once

#include "ofMain.h"

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
    void addSample(float samplitude);
    
private:
    string text;
    vector<float> samples;
    
    // Nothing yet
};
