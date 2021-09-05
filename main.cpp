#include "DxLib.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <mbstring.h>
#include <mbctype.h>

using namespace DxLib;
using namespace std;
#include "lib/game.h"

#pragma comment(lib, "winmm.lib")


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);
	SetAlwaysRunFlag(TRUE);
	SetMainWindowText("Maxence 0.4.0");
	SetWindowIconID(101);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetBackgroundColor(0, 0, 0);	//背景色
	srand((unsigned)time(NULL));


	Game game;

	unsigned int White = GetColor(255, 255, 255);
	unsigned int Black = GetColor(0, 0, 0);

	//メインループ
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
		// 入力情報を取得
		game.mouse.update();

		game.drawBase();
		game.drawNextField();
		// プレイヤーの操作
		if (game.playTurn()) {
			game.update();
		}
		game.drawCurrentCoord();

		// 勝利判定
		game.mother.victory();
	}

	InitGraph();
	DxLib_End();

	return 0;
}

