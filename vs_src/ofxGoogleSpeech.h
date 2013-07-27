//
//  ofxGoogleSpeech.h
//  emptyExample
//
//  Created by Martial on 03/04/13.
//
//

#ifndef __emptyExample__ofxGoogleSpeech__
#define __emptyExample__ofxGoogleSpeech__

#include "ofMain.h"
#include "ofxCurl.h"
#include "ofxJSONElement.h"

class ofxGoogleSpeech {
public:
    
    void sendFlac(string flacFile);
    ofEvent<string > event;
    
};

//-------------------------------------------------------------------------------------------callback function for curl response

struct callBackData{
	int id;
	int deviceId;
	long timestamp;
    class ofxGoogleSpeech * parent;
};


static size_t writeResponseFunc(void *ptr, size_t size, size_t nmemb, callBackData * data){
    
    
    ofxJSONElement json;
    json.parse(ofToString((char*)ptr));
        
    ofxJSONElement hypotheses = json["hypotheses"];
    
    ofLog(OF_LOG_NOTICE, json.getRawString());
    
    int i = 0;
    if (hypotheses.size() > 0 ) {
        
        string result = json["hypotheses"][i]["utterance"].asString();
        ofNotifyEvent(data->parent->event, result, data->parent);
        
              
    }
    
    
	return size * nmemb;
}




#endif /* defined(__emptyExample__ofxGoogleSpeech__) */
