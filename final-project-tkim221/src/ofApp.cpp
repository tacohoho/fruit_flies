#include "ofApp.h"
using std::cout;
using std::endl;

// declared start variable and inititialized because auto variables must be initialized
auto start = std::chrono::steady_clock::now();

//--------------------------------------------------------------
void ofApp::setup() {
	// Seed random with current time
	srand(static_cast<unsigned>(time(0)));

	mysteryBox.loadImage("mysteryBox.png");
	blackhole.loadImage("blackholepixel.png");
	background.loadImage("skypixel.png");
	player1_image.loadImage("apple.png");
	player2_image.loadImage("banana.png");
	bullet_image.loadImage("flypixel.png");
	
	p1.setPlayer(&player1_image, kP1Start, kP1Start, "one");
	p2.setPlayer(&player2_image, kP2Start, kP1Start, "two");
	powerUp.setPowerUp(&mysteryBox);

	start = std::chrono::steady_clock::now();
	megalovania.load("megalovania.mid");
	megalovania.play();
	pew.load("pew.wav");
}

//--------------------------------------------------------------
void ofApp::update() {

	if (current_state == IN_PROGRESS) {

		// movement of players
		if (keyIsDown[OF_KEY_LEFT]) {
			p2.pos.x -= p2.speed;
			p2.direction = LEFT;
		}

		if (keyIsDown[OF_KEY_RIGHT]) {
			p2.pos.x += p2.speed;
			p2.direction = RIGHT;
		}

		if (keyIsDown[OF_KEY_UP]) {
			p2.pos.y -= p2.speed;
			p2.direction = UP;
		}

		if (keyIsDown[OF_KEY_DOWN]) {
			p2.pos.y += p2.speed;
			p2.direction = DOWN;
		}
		if (keyIsDown['a']) {
			p1.pos.x -= p1.speed;
			p1.direction = LEFT;
		}

		if (keyIsDown['d']) {
			p1.pos.x += p1.speed;
			p1.direction = RIGHT;
		}

		if (keyIsDown['w']) {
			p1.pos.y -= p1.speed;
			p1.direction = UP;

		}

		if (keyIsDown['s']) {
			p1.pos.y += p1.speed;
			p1.direction = DOWN;

		}
		// t and . is for shooting
		if (keyIsDown['t']) {
			auto end = std::chrono::steady_clock::now();
			double elapsed_time = double(std::chrono::duration_cast <std::chrono::seconds> (end - start).count());
			
			if (p1.can_shoot(elapsed_time)) {
				pew.play();
				Bullet b;
				b.setBullet(p1, p1.pos, elapsed_time, &bullet_image);
				bullets.push_back(b);
				p1.last_shot = elapsed_time;
			}
		}

		if (keyIsDown['.']) {
			auto end = std::chrono::steady_clock::now();
			double elapsed_time = double(std::chrono::duration_cast <std::chrono::seconds> (end - start).count());

			if (p2.can_shoot(elapsed_time)) {
				pew.play();
				Bullet b;
				b.setBullet(p2, p2.pos, elapsed_time, &bullet_image);
				bullets.push_back(b);
				p2.last_shot = elapsed_time;
			}
		}

		// update bullet images
		for (int i = 0; i < bullets.size(); i++) {
			bullets.at(i).update();
		}

		// check if any bullets have hit any player
		bulletCollisions();

		// check if anything has hit any of the black holes
		blackHoleCollisions(kBlackHoleLoc1, kBlackHoleLoc1);
		blackHoleCollisions(kBlackHoleLoc2, kBlackHoleLoc3);
		blackHoleCollisions(kBlackHoleLoc4, kBlackholeSize);

		// checks if player 1 or player 2 have gone out of bounds
		outOfBounds(p1);
		outOfBounds(p2);

		// checks if power ups have been taken
		powerUpCollision(p1, powerUp);
		powerUpCollision(p2, powerUp);

		// check if powerUp spawned on black hole
		checkPowerUp(powerUp);
	}
}


//--------------------------------------------------------------
void ofApp::draw() {
	background.draw(0, 0);
	blackhole.draw(kBlackHoleLoc1, kBlackHoleLoc1, kBlackholeSize, kBlackholeSize);
	blackhole.draw(kBlackHoleLoc2, kBlackHoleLoc3, kBlackholeSize, kBlackholeSize);
	blackhole.draw(kBlackHoleLoc4, kBlackholeSize, kBlackholeSize, kBlackholeSize);
	if (current_state == PAUSED) {
		drawGamePaused();
	}
	else if (current_state == FINISHED) {
		drawGameOver();
	}
	drawPlayer(p1);
	drawPlayer(p2);
	for (int i = 0; i < bullets.size(); i++) {
		drawBullet(bullets.at(i));
	}
	drawPowerUp(powerUp);
}

/**
 * This function draws the paused screen
 */
void ofApp::drawGamePaused() {
	string pause_message = "P to Unpause!";
	ofSetColor(0, 0, 0);
	ofDrawBitmapString(pause_message, ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
	ofSetColor(255, 255, 255);
}

/**
 * This function draws the game over screen and displays who won
 */
void ofApp::drawGameOver() {
	string game_over_message;
	if (p1Winner) {
		game_over_message = "Apple Wins! \n Press 'b' to play again";
	}
	else {
		game_over_message = "Banana Wins \n Press 'b' to play again";
	}
	ofSetColor(0, 0, 0);
	ofDrawBitmapString(game_over_message, ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
	ofSetColor(255, 255, 255);
}

/**
 * This function draws a player at their position
 * @param player parameter for knowing which player to draw
 */
void ofApp::drawPlayer(Player player) {
	player.img->draw(player.pos.x, player.pos.y, player.width, player.width);
}

/**
 * This function draws a bullet at its position
 * @param bullet parameter for knowing which bullet to draw
 */
void ofApp::drawBullet(Bullet bullet) {
	bullet.img->draw(bullet.position.x, bullet.position.y, bullet.width, bullet.width);
}

/**
 * This function draws a powerUp at its location
 * @param powerUp this is the powerUp that is being drawn
 */
void ofApp::drawPowerUp(PowerUp powerUp) {
	if (powerUp.draw) {
		powerUp.img->draw(powerUp.pos.x, powerUp.pos.y, kPlayerSize, kPlayerSize);
	}
}

/**
 * This function checks if a powerup has spawned on top of a black hole and if so puts it elsewhere
 * @Param powerUp the given powerUp that is being checked
 */
void ofApp::checkPowerUp(PowerUp &powerUp) {
	auto end = std::chrono::steady_clock::now();
	double elapsed_time = double(std::chrono::duration_cast <std::chrono::seconds> (end - start).count());

	if (elapsed_time == powerUp.time_to_place) {
		while (ofDist(kBlackHoleLoc1 + kBlackholeSize / 2, kBlackHoleLoc1 + kBlackholeSize / 2, powerUp.pos.x + kPlayerCenter, powerUp.pos.y + kPlayerCenter)
			< (powerUp.width + kBlackholeSize) / 2 ||
			ofDist(kBlackHoleLoc2 + kBlackholeSize / 2, kBlackHoleLoc3 + kBlackholeSize / 2, powerUp.pos.x + kPlayerCenter, powerUp.pos.y + kPlayerCenter)
			< (powerUp.width + kBlackholeSize) / 2 ||
			ofDist(kBlackHoleLoc4 + kBlackholeSize / 2, kBlackholeSize + kBlackholeSize / 2, powerUp.pos.x + kPlayerCenter, powerUp.pos.y + kPlayerCenter)
			< (powerUp.width + kBlackholeSize) / 2) {
			powerUp.pos.x = (rand() % (kWindowXSize - kPlayerSize)) + 1;
			powerUp.pos.y = (rand() % (kWindowYSize - kPlayerSize)) + 1;
		}
		powerUp.draw = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	keyIsDown[key] = true;
	if (keyIsDown['p'] && current_state != FINISHED) {
		// Pause or unpause
		current_state = (current_state == IN_PROGRESS) ? PAUSED : IN_PROGRESS;
	}
	if (keyIsDown['b'] && current_state == FINISHED) {
		// restart game
		current_state = IN_PROGRESS;
		setup();
		bullets.clear();
		p1.last_shot = 0;
		p2.last_shot = 0;
	}
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	keyIsDown[key] = false;
}
	
/**
 * This function checks if any bullets have collided with the players
 */
void ofApp::bulletCollisions() {
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i].fromPlayer.name == "one") {
			if (ofDist(bullets[i].position.x + bullets[i].width / 2, bullets[i].position.y + bullets[i].width / 2, p2.pos.x + kPlayerCenter, p2.pos.y + kPlayerCenter)
				< (p2.width + bullets[i].width) / 2) {
				p1Winner = true;
				current_state = FINISHED;
			}
		}
		else {
			if (ofDist(bullets[i].position.x + bullets[i].width / 2, bullets[i].position.y + bullets[i].width / 2, p1.pos.x + kPlayerCenter, p1.pos.y + kPlayerCenter)
				< (p1.width + bullets[i].width) / 2) {
				p1Winner = false;
				current_state = FINISHED;
			}
		}
	}
}

/**
 * This function checks if anything has collided with a black hole
 * @Param xPos the x coordinate of a black hole
 * @Param yPos the y coordinate of a black hole
 */
void ofApp::blackHoleCollisions(int xPos, int yPos) {
	// looks for any bullets that have collided with black hole
	for (int i = 0; i < bullets.size(); i++) {
		if (ofDist(bullets[i].position.x + kBulletCenter, bullets[i].position.y + kBulletCenter, xPos + kBlackholeSize / 2, yPos + kBlackholeSize / 2)
			< (kBlackholeSize + bullets[i].width) / 2) {
			bullets.erase(bullets.begin() + i);
		}
	}

	// looks for any players that have collided with black hole
	if (ofDist(xPos + kBlackholeSize / 2, yPos + kBlackholeSize / 2, p2.pos.x + kPlayerCenter, p2.pos.y + kPlayerCenter)
		< (p2.width + kBlackholeSize) / 2) {
		p1Winner = true;
		current_state = FINISHED;
	}
	if (ofDist(xPos + kBlackholeSize / 2, yPos + kBlackholeSize / 2, p1.pos.x + kPlayerCenter, p1.pos.y + kPlayerCenter)
		< (p1.width + kBlackholeSize) / 2) {
		p1Winner = false;
		current_state = FINISHED;
	}
}

/**
 * This function checks if any players have collided with any powerups and give them the powerup
 */
void ofApp::powerUpCollision(Player &player, PowerUp &powerUp) {
	if (ofDist(powerUp.pos.x + kPlayerSize / 2, powerUp.pos.y + kPlayerSize / 2, player.pos.x + kPlayerCenter, player.pos.y + kPlayerCenter)
		< (player.width + kPlayerSize) / 2) {
		if (powerUp.name == "Player Speed Up") {
			player.speed = player.speed * 2;
		}
		else if (powerUp.name == "Bullet Size Up") {
			player.bullet_size = player.bullet_size * 2;
		}
		powerUp.setPowerUp(&mysteryBox);
	}
}

/**
 * This function prevents a player from going out of bounds
 * @Param player this parameter is the given player that is being checked for out of bounds
 */
void ofApp::outOfBounds(Player &player) {
	if (player.pos.x + kPlayerSize > kWindowXSize) {
		player.pos.x = kWindowXSize - kPlayerSize;
	}
	if (player.pos.x < 0) {
		player.pos.x = 0;
	}
	if (player.pos.y + kPlayerSize > kWindowYSize) {
		player.pos.y = kWindowYSize - kPlayerSize;
	}
	if (player.pos.y < 0) {
		player.pos.y = 0;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}