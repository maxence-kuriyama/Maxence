#pragma once

#include "lib/basic.h"


// ロゴマークオブジェクト
// リセットボタンとしての機能など
class Logo {
private:
	int titleX = 445;
	int titleY = 400;
	int image;

public:
	double acRate = 0.0;

	Logo() {
		initialize();
	}

	~Logo() {
		DeleteGraph(image);
	}

	void initialize() {
		acRate = 0.0;
		titleX = 445;
		titleY = 400;
		DeleteGraph(image);
		image = LoadGraph("graph/Maxence_after4.png");
	}

	void update() {
		if (acRate >= 0.5 && acRate <= 30.0) {
			acRate += 0.03;
		}
		titleX += acRate * (rand() % 11 - 5.0);
		titleY += acRate * (rand() % 11 - 5.0);
		if (titleX <= -10) {
			titleX = -10;
		}
		if (titleX >= 640 - 160) {
			titleX = 640 - 160;
		}
		if (titleY <= -10) {
			titleY = -10;
		}
		if (titleY >= 480 - 80) {
			titleY = 480 - 80;
		}
	}

	void draw() {
		DrawExtendGraph(titleX, titleY, titleX + 190, titleY + 60, image, TRUE);
	}

	void draw(Mouse& mouse) {
		if (mouse.onButton(titleX, titleY - 5, titleX + 185, titleY + 65)) {
			DrawBox(titleX, titleY - 5, titleX + 185, titleY + 65, GetColor(20, 150, 150), TRUE);
		}
		DrawExtendGraph(titleX, titleY, titleX + 190, titleY + 60, image, TRUE);
	}

	bool isClicked(Mouse& mouse) {
		int upLeftX = titleX;
		int upLeftY = titleY - 5;
		int lowRightX = titleX + 185;
		int lowRightY = titleY + 65;
		if (mouse.x > upLeftX && mouse.x < lowRightX && mouse.y > upLeftY && mouse.y < lowRightY) {
			if (mouse.click()) {
				return true;
			}
		}
		return false;
	}
};