#include "WordBox.h"

WordBox::WordBox() {
    // Constructor
    jitterFont.loadFont("GUI/NewMedia Fett.ttf", 100, true, true, true);
    indexOffset = 0;
    _confidence = 0;
    bGotResult = false;
}

void WordBox::update() {
    
}

void WordBox::draw() {
    if (position.y < -1100) return; // don't draw off screeners
    
    if (bGotResult && (_text == "")) {
        // Don't draw bad results
     return;
    }
    else if (bGotResult &&  (_text != "")) {
        // Scroll up after results are received
        position.y -= 1;
    }
    
    //cout << position.y << endl;

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
        
        jitterFont.drawStringAsShapes(_text, 0, 0, (1 - (_confidence * _confidence)) * _confidenceVizFactor);
        ofPopMatrix();
        ofPopStyle();
    }
    
    ofPopStyle();
    ofPopMatrix();
}

void WordBox::drawDebug() {
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(position.x, position.y);
    //    ofScale(2, 2);

    //glScalef(1, 1, 1);
    
    ofRotate(90);
    ofSetColor(255, 0, 0);
    ofFill();
    ofDrawBitmapString("Confidence: " + ofToString(_confidence), 0, 0);
    
    ofPopStyle();
    ofPopMatrix();
}

void WordBox::setText(string text) {
    _text = text;
    bGotResult = true;
    textBoundingBox = jitterFont.getStringBoundingBox(_text, 0, 0);
}

void WordBox::addSample(float samplitude) {
    samples.push_back(samplitude);
}

void WordBox::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

ofPoint WordBox::getPosition() {
    return position;
}

void WordBox::tweenTo(float x, float y, float durationSeconds) {
    Tweener.addTween(position.x, x, durationSeconds, &ofxTransitions::easeInOutQuad);
    Tweener.addTween(position.y, y, durationSeconds, &ofxTransitions::easeInOutQuad);
}

void WordBox::setConfidenceVizFactor(float factor) {
    _confidenceVizFactor = factor;
}


void WordBox::setConfidence(float confidence) {
    _confidence = confidence;
}


void WordBox::setDamageFactor(float factor) {
    _damageFactor = factor;
}