#pragma once
#include "ofImage.h"
#include "ofFileUtils.h"
#include <sstream>

using namespace std;

//http://netpbm.sourceforge.net/doc/pfm.html

class ofxPFMImage : public ofFloatImage {
public:
	
		bool 	loadPFMImage(string fileName);
		void	savePFMImage(string filename);
};