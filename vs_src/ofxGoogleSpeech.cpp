//
//  ofxGoogleSpeech.cpp
//  emptyExample
//
//  Created by Martial on 03/04/13.
//
//

#include "ofxGoogleSpeech.h"

void ofxGoogleSpeech::sendFlac(string flacFile) {
    
    string url = "http://www.google.com/speech-api/v1/recognize?xjerr=1&client=chromium&lang=en";
    
    CURL *curl;
	CURLcode res;
    
	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL;
    
	curl_global_init(CURL_GLOBAL_ALL);
    
	// set form
	curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME,
                 "file",
                 CURLFORM_FILE,
                 ofToDataPath(flacFile).c_str(),
                 CURLFORM_CONTENTTYPE,
                 "audio/x-flac",// rate=16000",
                 CURLFORM_END);
    
	curl = curl_easy_init();
    
	// set header
	//headerlist = curl_slist_append(headerlist, "Expect:");
	headerlist = curl_slist_append(headerlist, "Content-Type: audio/x-flac; rate=16000");
	if(curl){
		//set options
		curl_easy_setopt(curl,
                         CURLOPT_URL,
                         url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        
		//set callback function and callback data for the https response
        
		callBackData data;
		data.id         = 0;
		data.timestamp  = ofGetSystemTime();
		data.deviceId   = 0;
        data.parent     = this;
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeResponseFunc);
        
        
		//submit form
		res = curl_easy_perform(curl);
        
		//cleanup
		curl_easy_cleanup(curl);
		curl_formfree(formpost);
		curl_slist_free_all(headerlist);
        
    }

    
}