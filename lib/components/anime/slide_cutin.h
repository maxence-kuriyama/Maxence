#pragma once

#include "lib/components/anime.h"

// 試合中のアニメーションクラス
class SlideCutin : public AnimeBase {
private:
	int image0 = NULL;
	int image1 = NULL;

public:

	SlideCutin() {
		image0 = LoadGraph("graph/cutin1_0.png");
		image1 = LoadGraph("graph/cutin1_1.png");
		GraphBlend(image0, image1, 255, DX_GRAPH_BLEND_MULTIPLE);

		initialize();
	}

private:

	void initialize() {
		AnimeBase::initialize();
	}

	void _update() {
		if (cnt <= 15) {
			slideIn();
		}
		else if (cnt <= 105) {
			display();
		}
		else if (cnt <= 120) {
			slideOut();
		}
	}

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
