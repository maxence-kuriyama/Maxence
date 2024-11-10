#pragma once

const int BG_NUM(3);
const int BG_SIZE_SCALE(10);
const double BG_ALPHA_FRONT(210);
const double BG_ALPHA_LAST(240);
const int BG_MIN_X(-100);
const int BG_MAX_X(300);
const int BG_MIN_Y(0);
const int BG_MAX_Y(100);

const int BG_WHO_NONE(0);
const int BG_WHO_YELLOW(1);
const int BG_WHO_RED(2);
const int BG_WHO_GREEN(3);
const int BG_WHO_BLUE(4);
const int BG_WHO_DEER(5);
const int BG_WHO_PL_YELLOW(6);

class BgCharacter {
private:
	struct Character {
		int x;
		int y;
		int who;
	};

	struct Character bg[BG_NUM] = {
		{0, 0, BG_WHO_NONE}
	};

	int imgYellow = LoadGraph("graph/enemy_yellow.png");
	int imgRed = LoadGraph("graph/enemy_red.png");
	int imgGreen = LoadGraph("graph/enemy_green.png");
	int imgBlue = LoadGraph("graph/enemy_blue.png");
	// int imgDeer   = LoadGraph("graph/enemy_deer.png");
	int imgPlYellow = LoadGraph("graph/player_yellow.png");

public:

	BgCharacter() {
		initialize();
	}

	void initialize() {
		for (int i = 0; i < BG_NUM; ++i) {
			bg[i] = { 0, 0, BG_WHO_NONE };
		}
	}

	void set(int who) {
		if (who == bg[0].who) return;

		for (int i = BG_NUM - 1; i > 0; --i) {
			bg[i] = bg[i - 1];
		}
		bg[0].who = who;
		bg[0].x = (BG_MAX_X - BG_MIN_X) * (rand() % 10) / 10.0 + BG_MIN_X;
		bg[0].y = (BG_MAX_Y - BG_MIN_Y) * (rand() % 10) / 10.0 + BG_MIN_Y;
	}

	void draw() {
		int screenHandle = MakeScreen(640, 480, TRUE);

		for (int i = BG_NUM - 1; i >= 0; --i) {
			int imgHandle = 0;
			int w = 0;
			int h = 0;
			switch (bg[i].who) {
			case BG_WHO_YELLOW:
				imgHandle = imgYellow;
				w = 35; h = 52;
				break;
			case BG_WHO_RED:
				imgHandle = imgRed;
				w = 38; h = 50;
				break;
			case BG_WHO_GREEN:
				imgHandle = imgGreen;
				w = 36; h = 51;
				break;
			case BG_WHO_BLUE:
				imgHandle = imgBlue;
				w = 53; h = 51;
				break;
			case BG_WHO_DEER:
				// imgHandle = imgDeer;
				break;
			case BG_WHO_PL_YELLOW:
				imgHandle = imgPlYellow;
				w = 47; h = 50;
				break;
			}

			if (imgHandle != 0) {
				SetDrawScreen(screenHandle);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				DrawExtendGraph(bg[i].x, bg[i].y, bg[i].x + w * BG_SIZE_SCALE, bg[i].y + h * BG_SIZE_SCALE, imgHandle, TRUE);
			}

			int screenHandleSub = MakeScreen(640, 480, TRUE);
			SetDrawScreen(screenHandleSub);
			int alpha = BG_ALPHA_FRONT + (BG_ALPHA_LAST - BG_ALPHA_FRONT) * i / (BG_NUM - 1);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			DrawGraph(0, 0, screenHandle, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			DeleteGraph(screenHandle);
			screenHandle = screenHandleSub;
		}
		SetDrawScreen(DX_SCREEN_BACK);
		DrawGraph(0, 0, screenHandle, TRUE);
		DeleteGraph(screenHandle);
	}
};