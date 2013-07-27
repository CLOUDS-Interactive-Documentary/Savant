#include "WordBox.h"



WordBox::WordBox() {
    // Constructor
    jitterFont.loadFont("GUI/NewMedia Fett.ttf", 100, true, true, true);
}


void WordBox::update() {
    
}

void WordBox::draw() {
    ofPushMatrix();
    ofTranslate(x, y);
    ofPushStyle();
    
    ofNoFill();
    ofSetColor(ofColor::red);
    ofSetLineWidth(1);
    
    ofRect(0, 0, width, height);
    
    // Draw the waveform (normalized, then scale to fit rect)
    
    if (samples.size() > 0) {
        
        waveformPath.clear();
        
        waveformPath.setFilled(false);
        waveformPath.setColor(ofColor::red);
        waveformPath.setStrokeWidth(1);
        
        waveformPath.moveTo(0, height / 2);
        
        // Find max
        float maxAmplitude = 0;
        for (int i = 0; i < width; i += 10) {
            float sampleIndex = ofMap(i, 0, width, 0, samples.size() - 1);
            maxAmplitude = max(maxAmplitude, abs(samples[sampleIndex]));
        }
        
        // Plot form
        for (int i = 0; i < width; i += 10) {
            float sampleIndex = ofMap(i, 0, width, 0, samples.size() - 1);
            waveformPath.curveTo(i, ofMap(samples[sampleIndex], -maxAmplitude, maxAmplitude, 0, height));
        }
        
        waveformPath.curveTo(width, height / 2);
        waveformPath.draw(0, 0);
    }
    
    if (_text != "") {
        // Draw the text
        jitterFont.drawStringAsShapes(_text, 0, 0, ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 10));
    }
    
    
                                      
    ofPopStyle();
    ofPopMatrix();
}

void WordBox::setText(string text) {
    _text = text;
}

void WordBox::setConfidence(float confidence) {
    _confidence = confidence;
}


void WordBox::addSample(float samplitude) {
    samples.push_back(samplitude);
}