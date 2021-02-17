#pragma once
#include "ofMain.h"
#include "player.h"

class Bullet {
public:
	ofPoint position;
	int speed;
	ofImage *img;
	int width;
	Player fromPlayer;
	Direction direction;

	void setBullet(Player fromPlayer, ofPoint position, int sp, ofImage *bullet_image);
	void update();
};