#pragma once

const int CHARACTER_WHO_NONE(0);
const int CHARACTER_WHO_YELLOW(1);
const int CHARACTER_WHO_RED(2);
const int CHARACTER_WHO_GREEN(3);
const int CHARACTER_WHO_BLUE(4);
const int CHARACTER_WHO_DEER(5);
const int CHARACTER_WHO_PL_YELLOW(6);

class Character {
private:
	int who = CHARACTER_WHO_NONE;
	int imgHandle = 0;
	int w = 0;
	int h = 0;

	int imgYellow = LoadGraph("graph/enemy_yellow.png");
	int imgRed = LoadGraph("graph/enemy_red.png");
	int imgGreen = LoadGraph("graph/enemy_green.png");
	int imgBlue = LoadGraph("graph/enemy_blue.png");
	// int imgDeer   = LoadGraph("graph/enemy_deer.png");
	int imgPlYellow = LoadGraph("graph/player_yellow.png");

public:
	int x = 0;
	int y = 0;
	int scale = 10;
	int alpha = 255;

	Character& operator=(const Character& src);

	Character() {
		initialize();
	}

	void initialize() {
		x = 0;
		y = 0;
		alpha = 0;
		scale = 10;
	}

	void initialize(int srcWho) {
		who = srcWho;
		initializeImageHandle();

		initialize();
	}

	void draw(int screen1, int screen2) {
		//int screenHandle = MakeScreen(640, 480, TRUE);

		if (imgHandle != 0) {
			SetDrawScreen(screen1);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawExtendGraph(x, y, x + w * scale, y + h * scale, imgHandle, TRUE);
		}
		
		SetDrawScreen(screen2);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawGraph(0, 0, screen1, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//DeleteGraph(screenHandle);
	}

	int getWho() {
		return who;
	}

private:
	void initializeImageHandle() {
		switch (who) {
		case CHARACTER_WHO_YELLOW:
			imgHandle = imgYellow;
			w = 35; h = 52;
			break;
		case CHARACTER_WHO_RED:
			imgHandle = imgRed;
			w = 38; h = 50;
			break;
		case CHARACTER_WHO_GREEN:
			imgHandle = imgGreen;
			w = 36; h = 51;
			break;
		case CHARACTER_WHO_BLUE:
			imgHandle = imgBlue;
			w = 53; h = 51;
			break;
		case CHARACTER_WHO_DEER:
			// imgHandle = imgDeer;
			break;
		case CHARACTER_WHO_PL_YELLOW:
			imgHandle = imgPlYellow;
			w = 47; h = 50;
			break;
		default:
			imgHandle = 0;
			w = 0; h = 0;
			break;
		}
	}
};

Character &Character::operator=(const Character& src) {
	this->initialize(src.who);
	this->x = src.x;
	this->y = src.y;
	this->alpha = src.alpha;
	this->scale = src.scale;
	return (*this);
}
