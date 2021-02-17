#pragma once
#include "ofMain.h"

// Enums that represents all possible directions that the player is facing
enum Direction {
	UP = 0,
	DOWN,
	RIGHT,
	LEFT
};

class Player {
public:
	ofPoint pos;
	int width;
	Direction direction;
	ofImage *img;
	string name;
	float last_shot;
	int speed;
	int bullet_size;
	 
	void setPlayer(ofImage *_img, int xPos, int yPos, string _name);
	bool can_shoot(double elapsed_time);
};