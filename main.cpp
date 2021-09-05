#include "DxLib.h"
#include <iostream>

using namespace DxLib;
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

