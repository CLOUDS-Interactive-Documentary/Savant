#pragma once

#include "ofMain.h"

class JitterFont : public ofTrueTypeFont {
public:
    void drawStringAsShapes(string c, float x, float y, float jitterAmount);
    void drawCharAsShape(int c, float x, float y, float jitterAmount);
    
private:

};
