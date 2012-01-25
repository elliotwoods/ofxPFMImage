#pragma once
#include "ofImage.h"
#include "ofFileUtils.h"

class ofxPFMImage : public ofFloatImage {
public:
	
		bool 	loadPFMImage(string fileName);
		void	savePFMImage(string filename) const;
}