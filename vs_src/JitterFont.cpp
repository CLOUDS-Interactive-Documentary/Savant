#include "JitterFont.h"

//=====================================================================
void JitterFont::drawStringAsShapes(string c, float x, float y, float jitterAmount) {
    
    if (!bLoadedOk){
    	ofLog(OF_LOG_ERROR,"ofTrueTypeFont::drawStringAsShapes - Error : font not allocated -- line %d in %s", __LINE__,__FILE__);
    	return;
    };
    
	//----------------------- error checking
	if (!bMakeContours){
		ofLog(OF_LOG_ERROR,"ofTrueTypeFont::drawStringAsShapes - Error : contours not created for this font - call loadFont with makeContours set to true");
		return;
	}
    
	GLint		index	= 0;
	GLfloat		X		= 0;
	GLfloat		Y		= 0;
    
	ofPushMatrix();
	ofTranslate(x, y);
	int len = (int)c.length();
    
	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
            if (c[index] == '\n') {
                
				Y += lineHeight;
				X = 0 ; //reset X Pos back to zero
                
            }else if (c[index] == ' ') {
                int cy = (int)'p' - NUM_CHARACTER_TO_START;
                X += cps[cy].setWidth;
                //glTranslated(cps[cy].width, 0, 0);
            } else if(cy > -1){
				drawCharAsShape(c[index], X, Y, jitterAmount);
				X += cps[cy].setWidth;
				//glTranslated(cps[cy].setWidth, 0, 0);
            }
		}
		index++;
	}
    
	ofPopMatrix();
    
}


void JitterFont::drawCharAsShape(int c, float x, float y, float jitterAmount) {
	if (c >= nCharacters){
		//ofLog(OF_LOG_ERROR,"Error : char (%i) not allocated -- line %d in %s", (c + NUM_CHARACTER_TO_START), __LINE__,__FILE__);
		return;
	}
	//-----------------------
    
	ofTTFCharacter & charRef = charOutlines[c - NUM_CHARACTER_TO_START];
	charRef.setFilled(ofGetStyle().bFill);


    
    
    // start draw xy
	ofPushMatrix();
	ofTranslate(x,y);
    
    // start draw
    // we can also access the individual points
	ofPushMatrix();
    ofBeginShape();
    
    for(int k = 0; k <(int)charRef.getOutline().size(); k++){
        if( k!= 0)ofNextContour(true) ;
        for(int i = 0; i < (int)charRef.getOutline()[k].size(); i++){
            ofVertex(charRef.getOutline()[k].getVertices()[i].x + ofRandom(-jitterAmount, jitterAmount), charRef.getOutline()[k].getVertices()[i].y + ofRandom(-jitterAmount, jitterAmount));
        }
    }
    ofEndShape( true );
	ofPopMatrix();
    
    
    
    
    // end draw
    
    ofPopMatrix();
  
}