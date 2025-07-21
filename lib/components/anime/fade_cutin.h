#pragma once

#include "lib/components/anime.h"
#include "lib/utils/character.h"

const int FADE_CUTIN_RIGHT_CHARA_X = 400;
const int FADE_CUTIN_RIGHT_CHARA_Y = 50;

// 試合中のアニメーションクラス
class FadeCutin : public AnimeBase {
private:
	int Black = GetColor(0, 0, 0);

	Character chara;

public:

	FadeCutin() {
		initialize();
	}

private:

	void initialize() {
		AnimeBase::initialize();

		chara.initialize(CHARACTER_WHO_RED);
		chara.alpha = 0;
		chara.x = FADE_CUTIN_RIGHT_CHARA_X;
		chara.y = FADE_CUTIN_RIGHT_CHARA_Y;
	}

	void _update() {
		if (cnt <= 15) {
			chara.alpha += 10;
		}
		else if (cnt <= 105) {
			// do nothing
		}
		else if (cnt <= 120) {
			chara.alpha -= 10;
		}
		chara.x -= 1;
		display();
	}

	void display() {
		int screenHandle = MakeScreen(640, 480, TRUE);

		SetDrawScreen(screenHandle);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawBox(-1, -1, 641, 481, Black, TRUE);
		chara.draw(screenHandle);

		SetDrawScreen(DX_SCREEN_BACK);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawGraph(0, 0, screenHandle, TRUE);

		DeleteGraph(screenHandle);
	}
};
