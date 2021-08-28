#pragma once

class Logo {
public:
	double acRate = 0.0;
	int titleX = 445;
	int titleY = 400;
	int image = LoadGraph("graph/Maxence_after4.png");

	void initialize() {
		acRate = 0.0;
		titleX = 445;
		titleY = 400;
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
};