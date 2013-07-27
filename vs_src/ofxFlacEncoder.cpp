//
//  ofxFlacEncoder.cpp
//  emptyExample
//
//  Created by Martial on 03/04/13.
//
//

#include "ofxFlacEncoder.h"

bool ofxFlacEncoder::encode(string wavInput, string flacOutput) {
    
    //ofLog(OF_LOG_VERBOSE, "init encoding (device%d)",deviceId);
	FLAC__bool ok = true;
	FLAC__StreamEncoder *encoder = 0;
	FLAC__StreamEncoderInitStatus init_status;
	FILE *fin;
	unsigned sample_rate = 0;
	unsigned channels = 0;
	unsigned bps = 0;
    
	if((fin = fopen(ofToDataPath(wavInput).c_str(), "rb")) == NULL){
        
		//ofLog(OF_LOG_ERROR, "ERROR: opening %s for output\n", wavFile);
		return false;
	}
    
	// read and validate wav header
	if(fread(buffer, 1, 44, fin) != 44 || memcmp(buffer, "RIFF", 4)
       || memcmp(buffer + 8, "WAVEfmt \020\000\000\000\001\000\002\000", 16)
       || memcmp(buffer + 32, "\004\000\020\000data", 8)){
		ofLog(OF_LOG_ERROR,
              "invalid/unsupported WAVE file, only 16bps stereo WAVE in canonical form allowed");
		//fclose(fin);
		//return false;
	}
	sample_rate = ((((((unsigned) buffer[27] << 8) | buffer[26]) << 8) | buffer[25]) << 8)
    | buffer[24];
	channels = 2;
	bps = 16;
	total_samples = (((((((unsigned) buffer[43] << 8) | buffer[42]) << 8) | buffer[41]) << 8)
                     | buffer[40]) / 4;
    
	// allocate the encoder
	if((encoder = FLAC__stream_encoder_new()) == NULL){
		ofLog(OF_LOG_ERROR, " allocating encoder\n");
		fclose(fin);
		return false;
	}
    
	ok &= FLAC__stream_encoder_set_verify(encoder, true);
	ok &= FLAC__stream_encoder_set_compression_level(encoder, 5);
	ok &= FLAC__stream_encoder_set_channels(encoder, channels);
	ok &= FLAC__stream_encoder_set_bits_per_sample(encoder, bps);
	ok &= FLAC__stream_encoder_set_sample_rate(encoder, sample_rate);
	ok &= FLAC__stream_encoder_set_total_samples_estimate(encoder, total_samples);
    
	// initialize encoder
	if(ok){
		init_status = FLAC__stream_encoder_init_file(encoder, ofToDataPath(flacOutput).c_str(), NULL, NULL);
		if(init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK){
			ofLog(OF_LOG_ERROR, "initializing encoder: ");
			ofLog(OF_LOG_ERROR, FLAC__StreamEncoderInitStatusString[init_status]);
			ok = false;
		}
	}
    
	//ofLog(OF_LOG_VERBOSE, "start encoding (device%d)",deviceId);
	/* read blocks of samples from WAVE file and feed to encoder */
	if(ok){
		size_t left = (size_t) total_samples;
		while(ok && left){
			size_t need = (left > READSIZE ? (size_t) READSIZE : (size_t) left);
			if(fread(buffer, channels * (bps / 8), need, fin) != need){
				ofLog(OF_LOG_ERROR, "reading from WAVE file");
				ok = false;
			}else{
				/* convert the packed little-endian 16-bit PCM samples from WAVE into an interleaved FLAC__int32 buffer for libFLAC */
				size_t i;
				for(i = 0; i < need * channels; i++){
					/* inefficient but simple and works on big- or little-endian machines */
					pcm[i] = (FLAC__int32) (((FLAC__int16) (FLAC__int8) buffer[2 * i + 1] << 8)
                                            | (FLAC__int16) buffer[2 * i]);
				}
				/* feed samples to encoder */
				ok = FLAC__stream_encoder_process_interleaved(encoder, pcm, need);
			}
			left -= need;
		}
	}
    
	ok &= FLAC__stream_encoder_finish(encoder);
    
    //	fprintf(stderr, "encoding: %s\n", ok ? "succeeded" : "FAILED");
    //	fprintf(stderr,
    //			"   state: %s\n",
    //			FLAC__StreamEncoderStateString[FLAC__stream_encoder_get_state(encoder)]);
    
	FLAC__stream_encoder_delete(encoder);
	fclose(fin);
    
	return ok;


    
}