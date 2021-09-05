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

		// マウス操作か否かを判定する
		game.keyboardFlg = 0;

		// OPアニメーション ClickToStartまで
		if (game.flg == 0) {
			//タイトル画面その１
			if (game.isVsHuman()) {
				game.lonely.display(game.mouse, White);
				game.vsHuman.display(game.mouse, White);
				if (game.lonely.isClicked(game.mouse)) {
					game.mode = "ぼっちで";
					game.taijin = 1;
				}
				if (game.vsHuman.isClicked(game.mouse)) {
					game.mode = "隣の人と";
					game.initialize();
				}
			}
		}
		// Game Loop
		else if (game.flg == 1) {
			// 盤面の描画
			//MV1DrawModel(ModelHandle);
			game.drawBase();
			game.drawGlobalState();
			game.drawNextField();
			// プレイヤーの操作
			if (game.isPlayTurn() && game.playTurn()) {
				game.update();
			}
			game.drawLocalState();
			game.drawCurrentCoord();

			// 勝利判定
			game.mother.victory();
		}
	}

	InitGraph();
	DxLib_End();

	return 0;
}

