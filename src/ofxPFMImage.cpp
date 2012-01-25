#include "ofxPFMImage.h"

//http://netpbm.sourceforge.net/doc/pfm.html

bool ofxPFMImage::loadPFMImage(string fileName) {

	ifstream file;
	file.open(ofToDataPath(fileName, true), ios::binary | ios::in);

	char lineBuffer[2048];
	
	try {
		////
		//HEADER line 1
		file.getline(lineBuffer, 2048);
		bool colour;
		if (lineBuffer[0] == 'P' && lineBuffer[1] == 'F')
			colour = true;
		else if (lineBuffer[0] == 'P' && lineBuffer[1] == 'f')
			colour = false;
		else
			throw(0);
		//
		////
		
		////
		//HEADER line 2
		file.getline(lineBuffer, 2048);
		int width, height;
		istringstream line(lineBuffer);
		
		line >> width;
		line >> height;

		if (width <= 0 || height <= 0)
			throw(0);

		this->allocate(width, height, colour ? ofImageType::OF_IMAGE_COLOR : ofImageType::OF_IMAGE_GRAYSCALE);
		//
		/////

		////
		//HEADER line 3
		file.getline(lineBuffer, 2048);
		float scale;
		line = istringstream(lineBuffer);

		line >> scale;
		scale = abs(scale);

		/**HACK**/
		//for the time being we ignore endianness
		//
		////

		////
		//BODY
		float* pix = this->getPixels();
		float value;
				
		int expectedPixels = this->getWidth() * this->getHeight() * this->getPixelsRef().getNumChannels();
		
		for (int i=0; i<expectedPixels; i++) {
			file.read((char*)&value, sizeof(float));
			*pix++ = value * scale;
		}
		//
		////

		this->update();
		return true;

	} catch (...) {
		this->pixels.clear();
		return false;
	}
}

void ofxPFMImage::savePFMImage(string filename) {
	if (!this->isAllocated())
		return;

	ofFile file(filename, ofFile::WriteOnly, true);

	////
	//HEADER line 1
	int channelCount = this->getPixelsRef().getNumChannels();
	bool colour;
	switch (channelCount) {
	case 1:
		colour = false;
		break;
	case 3:
		colour = true;
		break;
	default:
		ofLogError() << "ofxPFMImage::savePFMImage - cannot save this image as it doesn't have either 1 or 3 channels";
		return;
		break;
	}

	file.write("P", 1);
	file.write(colour ? "F" : "f", 1);
	file.write("\n", 1);
	file << endl;
	//
	////

	////
	//HEADER line 2
	file << ofToString(this->getWidth()) << " " << ofToString(this->getHeight()) << endl;
	//
	/////

	////
	//HEADER line 3
	/**HACK**/
	//for the time being we ignore endianness
	file << "-1.000" << endl;
	//
	/////

	////
	//BODY
	file.write((char*) this->getPixels(), this->getWidth() * this->getHeight() * this->getPixelsRef().getNumChannels() * sizeof(float));
	//
	////
}