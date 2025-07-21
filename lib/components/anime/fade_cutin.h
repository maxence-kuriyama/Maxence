#pragma once
#pragma once

#include "lib/components/anime.h"

// 試合中のアニメーションクラス
class FadeCutin : public AnimeBase {
private:
	int image0 = NULL;
	int image1 = NULL;

public:

	FadeCutin() {
		image0 = LoadGraph("graph/cutin2_0.png");
		image1 = LoadGraph("graph/cutin2_1.png");
		GraphBlend(image0, image1, 255, DX_GRAPH_BLEND_MULTIPLE);

		AnimeBase::initialize();
	}

	void update() {
		if (!active) return;

		if (cnt <= 15) {
			slideIn();
		}
		else if (cnt <= 105) {
			display();
		}
		else if (cnt <= 120) {
			slideOut();
		}
		cnt++;

		if (cnt > 120) initialize();
	}

private:

	void slideIn() {
		SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
		DrawGraph(pow(cnt - 15, 2.0) * 640 / pow(15.0, 2.0), 0, image1, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 225);
		DrawGraph(pow(cnt - 15, 2.0) * 640 / pow(15.0, 2.0), 0, image0, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	void display() {
		SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
		DrawGraph(0, 0, image1, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 225);
		DrawGraph(0, 0, image0, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	void slideOut() {
		SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
		DrawGraph(-pow(cnt - 105, 2.0) * 640 / pow(15.0, 2.0), 0, image1, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 225);
		DrawGraph(-pow(cnt - 105, 2.0) * 640 / pow(15.0, 2.0), 0, image0, FALSE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
};
