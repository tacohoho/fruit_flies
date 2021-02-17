#include "powerUp.h"

/**
 * This function is like a constructor for my bullet class which sets all the values
 * @Param _img is the image the powerUp is set to
 */
void PowerUp::setPowerUp(ofImage *_img) {
	int random;
	img = _img;
	width = 40;
	// sets powerUp to random position within window size
	pos.x = (rand() % 984) + 1;
	pos.y = (rand() % 728) + 1;
	random = (rand() % 2) + 1;
	// powerUp spawns randomly within the first thirty seconds
	time_to_place = (rand() % 30) + 1;
	draw = false;

	switch (random) {
		case 1:
			name = "Player Speed Up";
			break;
		default:
			name = "Bullet Size Up";
			break;
	}
}