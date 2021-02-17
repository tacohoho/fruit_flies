#pragma once
#include "ofMain.h"

class PowerUp {
public:
	ofPoint pos;
	int width;
	string name;
	ofImage *img;
	int time_to_place;
	bool draw;

	void setPowerUp(ofImage *_img);
};