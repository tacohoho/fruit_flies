#include "player.h"

/**
 * This function is like a constructor for my player class and sets all the values
 * @Param _img is the image the player is set to
 * @Param xPos is the x coordinate of the player
 * @Param yPos is the y coordinate of the player
 * @Param _name is the name of the player
 */
void Player::setPlayer(ofImage *_img, int xPos, int yPos, string _name) {
	img = _img;
	name = _name;
	pos.x = xPos;
	pos.y = yPos;
	width = 40;
	speed = 3;
	bullet_size = 20;
}

/**
 * This function checks if the player is allowed to shoot
 * @Param elapsed_time this is the amount of time that has passed since the start of the game
 */
bool Player::can_shoot(double elapsed_time) {
	float reload_time = 10 / elapsed_time;
	if (last_shot == 0) {
		return true;
	}
	if ((elapsed_time - last_shot) > reload_time) {
		return true;
	}
	return false;
}