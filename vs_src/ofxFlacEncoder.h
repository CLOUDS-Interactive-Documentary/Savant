#pragma once

#include "ofMain.h"
#include "metadata.h"
#include "stream_encoder.h"

#define READSIZE 1024

class ofxFlacEncoder {

public:
    
    bool encode(string wavInput, string flacOutput);
    
    unsigned total_samples; /* can use a 32-bit number due to WAVE size limitations */
    FLAC__byte buffer[READSIZE/*samples*/* 2/*bytes_per_sample*/* 2/*channels*/]; /* we read the WAVE data into here */
    FLAC__int32	pcm[READSIZE/*samples*/* 2/*channels*/];
    
};