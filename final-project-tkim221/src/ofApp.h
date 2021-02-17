#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "ofMain.h"
#include "player.h"
#include "bullet.h"
#include "powerUp.h"

const int kSpeed = 3;
const int kP1Start = 255;
const int kP2Start = 600;
const int kPlayerCenter = 20;
const int kBulletCenter = 10;
const int kPlayerSize = 40;
const int kBulletSize = 20;
const int kBlackholeSize = 100;
const int kWindowXSize = 1024;
const int kWindowYSize = 768;
const int kBlackHoleLoc1 = 500;
const int kBlackHoleLoc2 = 150;
const int kBlackHoleLoc3 = 350;
const int kBlackHoleLoc4 = 700;


// Enum to represent the current state of the game
enum GameState {
	IN_PROGRESS = 0,
	PAUSED,
	FINISHED
};

class ofApp : public ofBaseApp{
	private:
		GameState current_state = IN_PROGRESS; // The current state of the game, used to determine possible actions
		bool keyIsDown[kP1Start];
		vector<Bullet> bullets;
		bool p1Winner;

	public:
		void setup();
		void update();
		void draw();
		void drawGamePaused();
		void drawGameOver();
		void drawPlayer(Player player);
		void drawBullet(Bullet bullet);
		void drawPowerUp(PowerUp powerUp);
		void bulletCollisions();
		void blackHoleCollisions(int xPos, int yPos);
		void powerUpCollision(Player &player, PowerUp &powerUp);
		void outOfBounds(Player &player);
		void checkPowerUp(PowerUp &powerUp);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofSoundPlayer megalovania;
		ofSoundPlayer pew;

		Player p1;
		Player p2;

		PowerUp powerUp;

		ofImage player1_image;
		ofImage player2_image;
		ofImage bullet_image;
		ofImage background;
		ofImage blackhole;
		ofImage mysteryBox;
};
