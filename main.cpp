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

	// 画像読み込み
	int MLogo = LoadGraph("graph/M.png");
	int axence = LoadGraph("graph/axence.png");
	int ClickToStart = LoadGraph("graph/click.png");
	int Logo0 = LoadGraph("graph/Maxence_after.png");
	int Logo1 = LoadGraph("graph/Maxence_after1.png");
	int Logo2 = LoadGraph("graph/Maxence_after2.png");
	int Logo3 = LoadGraph("graph/Maxence_after3.png");
	int Logo4 = LoadGraph("graph/Maxence_after4.png");
	double logoX = 0.0;		// デモ画面用変数


	//メインループ
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
		// 入力情報を取得
		game.mouse.update();

		// マウス操作か否かを判定する
		game.keyboardFlg = 0;

		// OPアニメーション ClickToStartまで
		if (game.flg == -3){
			SetBackgroundColor(0, 0, 0);	//背景色
			if (logoX <= 120.0 ) {
				DrawExtendGraph(270, 170, 358, 260, MLogo, TRUE);
			}
			else if (logoX <= 200.0) {
				DrawExtendGraph(270 - 1.25*(logoX - 120), 170, 358 - 1.25*(logoX - 120), 260, MLogo, TRUE);
				DrawExtendGraph(350 - 1.25*(logoX - 120), 170, 358 + 1.65*(logoX - 120), 260, axence, TRUE);
			}
			else {
				DrawExtendGraph(170, 170, 258, 260, MLogo, TRUE);
				DrawExtendGraph(250, 170, 490, 260, axence, TRUE);
			}
			if (logoX >= 300.0) {
				DrawExtendGraph(200, 290, 460, 360, ClickToStart, TRUE);
			}
			if (logoX >= 300.0 && logoX <= 480.0) {
				DrawBox(0, 290, 655 - 4.0*(logoX - 300), 360, Black, TRUE);
				DrawBox(650 - 4.0*(logoX - 300), 342, 670 - 4.0*(logoX - 300), 345, White, TRUE);
			}
			logoX += 2.0;

			if (logoX > 480.0 || game.mouse.click()) {
				game.flg = -2;
				logoX = M_PI_2;
			}
		}
		// OPアニメーション ClickToStart点滅
		else if (game.flg == -2) {
			DrawExtendGraph(170, 170, 258, 260, MLogo, TRUE);
			DrawExtendGraph(250, 170, 490, 260, axence, TRUE);

			logoX = (logoX + 0.05); if (logoX >= 360.0) logoX -= 360.0;
			SetDrawBright(155 + 100*sin(logoX), 155 + 100*sin(logoX), 155 + 100*sin(logoX));
			DrawExtendGraph(200, 290, 460, 360, ClickToStart, TRUE);
			SetDrawBright(255, 255, 255);

			if (game.mouse.click()) {
				game.flg = -1;
				SetBackgroundColor(0, 128, 128);	//背景色
				SetDrawBright(255, 255, 255);
			}
		}
		// OPアニメーション メインロゴ
		else if (game.flg == -1) {
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
			if(logoX < 1000.0) logoX += 1.0;
			if (logoX > 120 || game.mouse.click()) {
				game.flg = 0;
			}
		}
		// タイトル画面
		else if (game.flg == 0) {
			DrawExtendGraph(160 + (rand() % 11) - 5.0, 170, 490 + (rand() % 11) - 5.0, 260, Logo4, TRUE);
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

		// 同期処理
		game.sync();
	}

	InitGraph();
	DxLib_End();

	return 0;
}

