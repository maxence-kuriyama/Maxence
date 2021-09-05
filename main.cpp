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
					//COMの選ぶ座標
int COMWait = 0;
int waitOnCOM = 20;						//COMが手を打つまでのウェイト
int max_id = 0;
double max_val = 0.0;


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


	// 種々のハンドル
	int Font2 = CreateFontToHandle("HG教科書体", 36, 4, DX_FONTTYPE_ANTIALIASING_EDGE);
	int Font3 = CreateFontToHandle("HG教科書体", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
	int Font4 = CreateFontToHandle("Times New Roman", 72, 6, DX_FONTTYPE_ANTIALIASING_EDGE);
	unsigned int Green = GetColor(0, 255, 0);
	unsigned int Red = GetColor(255, 0, 0);
	unsigned int Blue = GetColor(0, 0, 255);
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


	double reward2, rwd_tmp;


	int vict = 0;	// 勝敗格納用の一時変数


	//メインループ
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
		// 入力情報を取得
		game.key.update();
		game.mouse.update();

		// リセットボタンを表示する
		game.reset();
		game.drawLogo();

		// 設定を切り替える
		game.toggleByKey();

		// マウス操作か否かを判定する
		if (game.mouse.isUsed()) {
			game.keyboardFlg = 0;
		}
		if (game.key.isUsed()) {
			game.keyboardFlg = 1;
		}


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

			if (logoX > 480.0 || game.mouse.click() || game.key.onReturn()) {
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

			if (game.mouse.click() || game.key.onReturn()) {
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
			if (logoX > 120 || game.mouse.click() || game.key.onReturn()) {
				game.flg = 0;
			}
		}
		// タイトル画面
		else if (game.flg == 0) {
			DrawExtendGraph(160 + (rand() % 11) - 5.0, 170, 490 + (rand() % 11) - 5.0, 260, Logo4, TRUE);
			//タイトル画面その１
			if (game.isVsHuman()) {
				int choice = game.menuChoose();
				if (choice == 0) {
					game.mode = "ぼっちで";
					game.taijin = 1;
					game.setOrderMenu();
				}
				else if (choice == 1) {
					game.mode = "隣の人と";
					game.initialize();
					initializeTrain();
				}

				//オートモード
				game.toggleAutoLearning();
			}
			//タイトル画面その２（「ぼっちで」選択時）
			else if (game.isVsCOM()) {
				int choice = game.menuChoose();
				if (choice == 0) {
					game.goScenario();
					game.teban = 0;
				}
				else if (choice == 1) {
					game.goScenario();
					game.teban = 1;
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
				rwd_tmp = game.update();
				if (rwd_tmp > -10.0) {
					if (game.isVsCOM()) {
						COMWait = waitOnCOM;
						// reward2 = -rwd_tmp;
					}
				}
			}
			game.drawLocalState();
			game.drawCurrentCoord();

			// メッセージの描画
			game.drawBattleMessage();


			// COMの手番
			if (!game.isPlayTurn()) {
				//input = StateToInput(lay_size[0], 1 - 2 * (game.cnt % 2));
				//output = critic.predict(input);
				//max_val = output.maxCoeff(&max_id);
				//while (COMWait <= 0) {
				//	if (unif(mt) < anl_rate) {
				//		COMGx = rand() % 3; COMGy = rand() % 3;
				//		COMLx = rand() % 3; COMLy = rand() % 3;
				//		comHistt[trainCnt] = COMGx * 27 + COMGy * 9 + COMLx * 3 + COMLy;
				//		anl_flg = 1;
				//	}
				//	else {
				//		COMGx = (max_id / 27) % 3;
				//		COMGy = (max_id / 9) % 3;
				//		COMLx = (max_id / 3) % 3;
				//		COMLy = max_id % 3;
				//		comHistt[trainCnt] = max_id;
				//		anl_flg = 0;
				//	}
				//	//盤面の更新
				//	rwd_tmp = game.update(COMGx, COMGy, COMLx, COMLy);
				//	if (rwd_tmp > -10.0) {
				//		temp_i[trainCnt] = input;
				//		temp_o[trainCnt] = Reward1(output, input, 1 - 2 * (game.cnt % 2));
				//		temp_o[trainCnt](comHistt[trainCnt]) = rwd_tmp;
				//		if (trainCnt >= 1) {
				//			if (game.vsCOM()) {
				//				temp_o[trainCnt - 1](comHistt[trainCnt - 1]) += gamma * max_val + reward2;
				//			}
				//			else if (game.taijin == 2) {
				//				temp_o[trainCnt - 1](comHistt[trainCnt - 1]) -= rwd_tmp;
				//				if (trainCnt >= 2) {
				//					temp_o[trainCnt - 2](comHistt[trainCnt - 2]) += gamma * max_val;
				//				}
				//			}
				//		}
				//		trainCnt++;
				//		if (game.taijin == 2) COMWait = waitOnCOM;
				//		break;
				//	}
				//}
			}

			// 勝利判定
			vict = game.mother.victory();
			if (vict != 0) {
				game.goResult();
				game.key.initWait();
			}

			// 永遠に勝敗がつかない場合の処理
			game.stopDrawGame();

			// 高速学習モードへの切り替え
			game.toggleHighSpeedLearning();

		}
		// 勝敗表示
		else if (game.flg == 2) {
			// 盤面の描画
			game.drawBase();
			game.drawGlobalState();
			game.drawNextField();
			game.drawLocalState();

			// メッセージの描画
			game.drawWinner(vict);
			game.again.display(game.mouse, game.option.strColor);
			if (game.again.isClicked(game.mouse) || game.key.onReturn()){
				game.initialize();
				initializeTrain();
			}

			// 自動学習モード
			if (game.taijin == 2) {
				game.initialize();
				initializeTrain();
			}
		}


		// 同期処理
		game.sync();
		if (COMWait > 0) COMWait--;

		// デバッグ情報出力
		game.debugDump();
	}

	InitGraph();
	DxLib_End();

	return 0;
}

