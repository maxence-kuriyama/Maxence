#pragma once
#pragma once

#include "lib/components/anime.h"

const int FADE_CUTIN_RIGHT_CHARA_X = 400;
const int FADE_CUTIN_RIGHT_CHARA_Y = 50;

// 試合中のアニメーションクラス
class FadeCutin : public AnimeBase {
private:
	int image0 = NULL;
	int image1 = NULL;
	int Black = GetColor(0, 0, 0);

	int alpha = 0;
	int x = FADE_CUTIN_RIGHT_CHARA_X;
	int y = FADE_CUTIN_RIGHT_CHARA_Y;

public:

	FadeCutin() {
		image0 = LoadGraph("graph/cutin2_0.png");
		image1 = LoadGraph("graph/cutin2_1.png");
		GraphBlend(image0, image1, 255, DX_GRAPH_BLEND_MULTIPLE);

		initialize();
	}

	void update() {
		if (!active) return;

		if (cnt <= 15) {
			alpha += 10;
		}
		else if (cnt <= 105) {
			// do nothing
		}
		else if (cnt <= 120) {
			alpha -= 10;
		}
		x -= 1;
		display();
		cnt++;

		if (cnt > 120) initialize();
	}

private:

	void initialize() {
		AnimeBase::initialize();

		alpha = 0;
		x = FADE_CUTIN_RIGHT_CHARA_X;
		y = FADE_CUTIN_RIGHT_CHARA_Y;
	}

	void display() {
		int screenHandle = MakeScreen(640, 480, TRUE);

		SetDrawScreen(screenHandle);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawBox(-1, -1, 641, 481, Black, TRUE);
		DrawExtendGraph(x, y, x + 400, y + 400, image1, TRUE);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		SetDrawScreen(DX_SCREEN_BACK);
		DrawGraph(0, 0, screenHandle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DeleteGraph(screenHandle);
	}
};
