#include "WordBox.h"

void WordBox::update() {
   
}

void WordBox::draw() {
    ofPushMatrix();
    ofTranslate(x, y);
    ofPushStyle();
    
    ofSetColor(ofColor::red);
    ofFill();
    ofRect(0, 0, 50, 50);
    
    ofPopStyle();
    ofPopMatrix();
}

void WordBox::setText(string text) {
    text = text;
}