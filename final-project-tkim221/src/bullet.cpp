#include "bullet.h"

/**
 * This function is like a constructor for my bullet class which sets all the values 
 * @Param from is the player that the bullet belongs to
 * @Param pos the position of the player that is now the position of the bullet
 * @Param sp the speed that the bullet it being set to
 * @Param bullet_image is the image the bullet is set to
 */
void Bullet::setBullet(Player from, ofPoint pos, int sp, ofImage *bullet_image) {
	fromPlayer = from;
	position = pos;
	speed = sp;
	img = bullet_image;
	width = from.bullet_size;
	direction = from.direction;
}

/**
 * This function changes the position of a bullet based on the direction that was pressed
 */
void Bullet::update() {
	
	// movement of bullet
	if (direction == LEFT) {
		position.x -= speed;
	}

	if (direction == RIGHT) {
		position.x += speed;
	}

	if (direction == UP) {
		position.y -= speed;
	}
	
	if (direction == DOWN) {
		position.y += speed;
	}
	
}

