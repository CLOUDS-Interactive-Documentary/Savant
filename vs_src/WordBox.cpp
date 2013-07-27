#include "WordBox.h"

ofPath waveformPath;

WordBox::WordBox() {
    // Constructor
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
    
    ofPopStyle();
    ofPopMatrix();
}

void WordBox::setText(string text) {
    text = text;
}

void WordBox::addSample(float samplitude) {
    samples.push_back(samplitude);
}