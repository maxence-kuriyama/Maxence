#pragma once

// 試合中のアニメーションクラス
class Anime {
private:
	int cnt = 0;
	bool active = false;
	int image0 = NULL;
	int image1 = NULL;

public:
	
	Anime() {
		int Cutin1 = LoadGraph("graph/cutin1.png");
		int Cutin10 = LoadGraph("graph/cutin10.png");
		GraphBlend(Cutin1, Cutin10, 255, DX_GRAPH_BLEND_MULTIPLE);
		image0 = Cutin1;
		image1 = Cutin10;

		initialize();
	}

	void start() {
		active = true;
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

	void initialize() {
		cnt = 0;
		active = false;
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
