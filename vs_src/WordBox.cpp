#include "WordBox.h"

WordBox::WordBox() {
    // Constructor
    jitterFont.loadFont("GUI/NewMedia Fett.ttf", 100, true, true, true);
    indexOffset = 0;
}

void WordBox::update() {
    
}

void WordBox::draw() {
    ofPushMatrix();
    ofTranslate(position.x, position.y);
    ofPushStyle();
    
    // Background shade
    ofPushStyle();
    ofSetColor(255, 255, 255, 20);
    ofSetLineWidth(0);
    ofFill();
    ofRect(0, 0, width, height);
    ofPopStyle();
    
    
    // Waveform
    if (samples.size() > 0) {
        waveformPath.clear();
        
        waveformPath.setFilled(false);
        waveformPath.setFillColor(ofColor(255, 255, 255, 100));
        waveformPath.setStrokeColor(ofColor(255, 255, 255, 50));
        waveformPath.setStrokeWidth(5);
        waveformPath.moveTo(0, height / 2);
        
        // Let the wave flow
        indexOffset += 10;
        
        
        
        // Find max
        float maxAmplitude = 0;
        for (int i = 0; i < width; i += 10) {
            int sampleIndex = floor(ofMap(i, 0, width, 0, samples.size() - 1)) + indexOffset;
            sampleIndex %= samples.size();
            maxAmplitude = max(maxAmplitude, abs(samples[sampleIndex]));
        }
        
        // Plot form
        for (int i = 0; i < width; i += 10) {
            int sampleIndex = floor(ofMap(i, 0, width, 0, samples.size() - 1)) + indexOffset;
            sampleIndex %= samples.size();
            
            waveformPath.curveTo(i, ofMap(samples[sampleIndex], -maxAmplitude, maxAmplitude, 10, height - 10));
        }
        
        waveformPath.curveTo(width, height / 2);
        waveformPath.draw(0, 0);
    }
    
    if (_text != "") {
        // Draw the text, scale to fit according to bounding box
        ofRectangle targetRect = ofRectangle(textBoundingBox);
        targetRect.scaleTo(ofRectangle(0, 0, width, height), OF_SCALEMODE_FIT);
        
        ofPushStyle();
        ofSetColor(255, 255, 255, 150);
        ofFill();

        float textScale = targetRect.width / textBoundingBox.width;
        ofPushMatrix();
        
        float yCenterOffset = ((height / textScale) - textBoundingBox.height) / 2;
        
        ofScale(textScale, textScale);
        ofTranslate(0, textBoundingBox.height + yCenterOffset);
        
        jitterFont.drawStringAsShapes(_text, 0, 0, ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 5));
        ofPopMatrix();
        ofPopStyle();
    }
    
    ofPopStyle();
    ofPopMatrix();
}

void WordBox::setText(string text) {
    _text = text;
    textBoundingBox = jitterFont.getStringBoundingBox(_text, 0, 0);
}

void WordBox::setConfidence(float confidence) {
    _confidence = confidence;
}

void WordBox::addSample(float samplitude) {
    samples.push_back(samplitude);
}

void WordBox::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

void WordBox::tweenTo(float x, float y, float durationSeconds) {
    cout << "tweening"<< endl;
    Tweener.addTween(position.x, x, 1, &ofxTransitions::easeInOutQuad);
    Tweener.addTween(position.y, y, 1, &ofxTransitions::easeInOutQuad);
}
