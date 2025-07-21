#pragma once

#include "lib/utils/character.h"

const int BG_NUM(3);
const int BG_SIZE_SCALE(10);
const double BG_ALPHA_FRONT(240);
const double BG_ALPHA_LAST(130);
const int BG_MIN_X(-100);
const int BG_MAX_X(300);
const int BG_MIN_Y(0);
const int BG_MAX_Y(100);

class BgCharacter {
private:
	Character bg[BG_NUM];

public:

	BgCharacter() {
		initialize();
	}

	void initialize() {
		for (int i = 0; i < BG_NUM; ++i) {
			bg[i].initialize();
		}
	}

	void set(int who) {
		if (who == bg[0].getWho()) return;

		for (int i = BG_NUM - 1; i > 0; --i) {
			bg[i] = bg[i - 1];
			bg[i].alpha = BG_ALPHA_FRONT + (BG_ALPHA_LAST - BG_ALPHA_FRONT) * i / (BG_NUM - 1);
		}
		bg[0].initialize(who);
		bg[0].x = (BG_MAX_X - BG_MIN_X) * (rand() % 10) / 10.0 + BG_MIN_X;
		bg[0].y = (BG_MAX_Y - BG_MIN_Y) * (rand() % 10) / 10.0 + BG_MIN_Y;
		bg[0].alpha = BG_ALPHA_FRONT;
		bg[0].scale = BG_SIZE_SCALE;
	}

	void draw() {
		int screenHandle = MakeScreen(640, 480, TRUE);

		for (int i = BG_NUM - 1; i >= 0; --i) {
			bg[i].draw(screenHandle);
		}

		SetDrawScreen(DX_SCREEN_BACK);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawGraph(0, 0, screenHandle, TRUE);
		DeleteGraph(screenHandle);
	}
};