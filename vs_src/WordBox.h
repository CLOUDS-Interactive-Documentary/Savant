#pragma once

#include "ofMain.h"

class WordBox {
public:
    float x;
    float y;
    void update();
    void draw();
    void setText(string text);
    
private:
    string text;
    
    
    // Nothing yet
};
