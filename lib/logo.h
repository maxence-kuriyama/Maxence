#pragma once

#include "lib/user_input.h"


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

	void draw(UserInput& ui) {
		draw();
		update(*ui.key);
	}

	void draw() {
		DrawExtendGraph(titleX, titleY, titleX + 190, titleY + 60, image, TRUE);
	}

	void update(Key& key) {
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

		if (key.state[KEY_INPUT_AT] == 1) {
			if (acRate >= 0.5) {
				initialize();
			}
			else {
				acRate += 1.0;
			}
		}
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