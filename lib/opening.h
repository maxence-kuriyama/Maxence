#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "lib/mouse.h"
#include "lib/keyboard.h"

const int DEMO_FIRST(1);
const int DEMO_SECOND(2);
const int DEMO_THIRD(3);
const int DEMO_OVER(0);

class Opening {
private:
	int flg = DEMO_FIRST;
	
	// 画像読み込み
	int MLogo = LoadGraph("graph/M.png");
	int axence = LoadGraph("graph/axence.png");
	int ClickToStart = LoadGraph("graph/click.png");
	int Logo0 = LoadGraph("graph/Maxence_after.png");
	int Logo1 = LoadGraph("graph/Maxence_after1.png");
	int Logo2 = LoadGraph("graph/Maxence_after2.png");
	int Logo3 = LoadGraph("graph/Maxence_after3.png");
	int Logo4 = LoadGraph("graph/Maxence_after4.png");
	double logoX = 0.0;
	
	unsigned int White = GetColor(255, 255, 255);
	unsigned int Black = GetColor(0, 0, 0);

public:

	bool isOver() {
		return (flg == DEMO_OVER);
	}

	void showDemo(Mouse& mouse, Key& key) {
		if (flg == DEMO_FIRST) {
			showDemoFirst(mouse, key);
		}
		else if (flg == DEMO_SECOND) {
			showDemoSecond(mouse, key);
		}
		else if (flg == DEMO_THIRD) {
			showDemoThird(mouse, key);
		}
	}

	// OPアニメーション ClickToStartまで
	void showDemoFirst(Mouse& mouse, Key& key) {
		SetBackgroundColor(0, 0, 0);	//背景色
		if (logoX <= 120.0) {
			DrawExtendGraph(270, 170, 358, 260, MLogo, TRUE);
		}
		else if (logoX <= 200.0) {
			DrawExtendGraph(270 - 1.25 * (logoX - 120), 170, 358 - 1.25 * (logoX - 120), 260, MLogo, TRUE);
			DrawExtendGraph(350 - 1.25 * (logoX - 120), 170, 358 + 1.65 * (logoX - 120), 260, axence, TRUE);
		}
		else {
			DrawExtendGraph(170, 170, 258, 260, MLogo, TRUE);
			DrawExtendGraph(250, 170, 490, 260, axence, TRUE);
		}
		if (logoX >= 300.0) {
			DrawExtendGraph(200, 290, 460, 360, ClickToStart, TRUE);
		}
		if (logoX >= 300.0 && logoX <= 480.0) {
			DrawBox(0, 290, 655 - 4.0 * (logoX - 300), 360, Black, TRUE);
			DrawBox(650 - 4.0 * (logoX - 300), 342, 670 - 4.0 * (logoX - 300), 345, White, TRUE);
		}
		logoX += 2.0;

		if (logoX > 480.0 || mouse.click() || key.onReturn()) {
			flg = DEMO_SECOND;
			logoX = M_PI_2;
		}
	}

	// OPアニメーション ClickToStart点滅
	void showDemoSecond(Mouse& mouse, Key& key) {
		DrawExtendGraph(170, 170, 258, 260, MLogo, TRUE);
		DrawExtendGraph(250, 170, 490, 260, axence, TRUE);

		logoX = (logoX + 0.05); if (logoX >= 360.0) logoX -= 360.0;
		SetDrawBright(155 + 100 * sin(logoX), 155 + 100 * sin(logoX), 155 + 100 * sin(logoX));
		DrawExtendGraph(200, 290, 460, 360, ClickToStart, TRUE);
		SetDrawBright(255, 255, 255);

		if (mouse.click() || key.onReturn()) {
			flg = DEMO_THIRD;
			SetBackgroundColor(0, 128, 128);	//背景色
			SetDrawBright(255, 255, 255);
		}
	}

	// OPアニメーション メインロゴ
	void showDemoThird(Mouse& mouse, Key& key) {
		if (logoX <= 37.5) {
			DrawExtendGraph(160, 170, 490, 260, Logo0, TRUE);
		}
		else if (logoX <= 45.0) {
			DrawExtendGraph(160, 170, 490, 260, Logo1, TRUE);
		}
		else if (logoX <= 50.0) {
			DrawExtendGraph(160, 170, 490, 260, Logo2, TRUE);
		}
		else if (logoX <= 55.0) {
			DrawExtendGraph(160, 170, 490, 260, Logo3, TRUE);
		}
		else if (logoX <= 90.0) {
			DrawExtendGraph(160, 170, 490, 260, Logo4, TRUE);
		}
		else {
			DrawExtendGraph(160 + (rand() % 11) - 5.0, 170, 485 + (rand() % 11) - 5.0, 260, Logo4, TRUE);
		}
		if (logoX < 1000.0) logoX += 1.0;
		if (logoX > 120 || mouse.click() || key.onReturn()) {
			flg = DEMO_OVER;
		}
	}
};
