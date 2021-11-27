#include "DxLib.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <float.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <mbstring.h>
#include <mbctype.h>

using namespace DxLib;
using namespace std;
#include "lib/fireflower.h"
#include "lib/scenario.h"
#include "lib/game.h"
#include "lib/ending.h"
#include "lib/movie.h"
#include "lib/music.h"
#include "lib/com.h"

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
	SetCameraNearFar(100.0, 6000.0);
	SetGlobalAmbientLight(GetColorF(1.0, 0.0, 0.0, 0.0));
	ChangeLightTypePoint(VGet(320.0, 240.0, -300.0), 2000.0, 0.0, 0.001f, 0.0);
	int LightHandle = CreateDirLightHandle(VGet(0.0, 0.0, -1.0));
	srand((unsigned)time(NULL));


	Game game;
	Movie movie;
	Music bgm;
	Scenario scenario;
	Ending ending;
	fireflower tama[FIRE_FLOWER_NUM];
	COM com;


	// 種々のハンドル
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

	// 3Dモデル関係
	/*
	int ModelHandle = MV1LoadModel("movie/max0.mv1");
	float totalTime, playTime = 0.0;
	MV1SetPosition(ModelHandle, VGet(80.0, 150.0, 100.0));
	MV1SetScale(ModelHandle, VGet(0.1, 0.1, 0.1));
	//MV1SetRotationXYZ(ModelHandle, VGet(0.0, -0.5 * DX_PI_F, 0.0));
	MV1SetRotationXYZ(ModelHandle, VGet(0.0, DX_PI_F, 0.0));
	//int AttachIndex = MV1AttachAnim(ModelHandle, 1, -1, FALSE);
	//totalTime = MV1GetAttachAnimTotalTime(ModelHandle, AttachIndex);
	//int GrHandle = MV1GetTextureGraphHandle(ModelHandle, 0);
	*/
	double theta = 0.3;

	int vict = 0;			// 勝敗格納用の一時変数
	double reward = 0.0;	// 盤面評価格納用の一時変数


	//メインループ
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
		// 入力情報を取得
		game.key.update();
		game.mouse.update();

		// ローディングメッセージ
		movie.drawLoadMsg();

		// リセットボタンを表示する
		if (game.reset(bgm) == 1) {
			for (int i = 0; i < FIRE_FLOWER_NUM; ++i) {
				tama[i].initialize();
			}
			scenario.initialize();
		}
		game.drawLogo();

		// 設定を切り替える
		game.toggleByKey(bgm);
		if (game.option.soundFlg == 1) {
			tama[0].sound = 1;
		}
		else {
			tama[0].sound = 0;
		}

		// マウス操作か否かを判定する
		if (game.mouse.isUsed()) {
			game.keyboardFlg = 0;
		}
		if (game.key.isUsed()) {
			game.keyboardFlg = 1;
		}

		// エンディングのデバッグ
		if (game.debugEndingFlg) {
			if (game.flg != -4) {
				bgm.unloadAll();
				ending.initialize();
				game.goEnding();
			}
			else {
				bgm.unloadAll();
				game.goTitle();
			}
			game.debugEndingFlg = 0;
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
			bgm.load("sound/bgm03.mp3");
			for (int i = 0; i < FIRE_FLOWER_NUM; ++i) {
				tama[i].draw();
				tama[i].tick();
			}
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
					movie.loadOnce("movie/battle.ogv");
					game.mode = "隣の人と";
				}

				if (movie.isPrepared()){
					SetBackgroundColor(0, 0, 0);
					if (movie.play(game.key, game.debugFlg)) {
						movie.unload();
						game.initialize();
						initializeTrain();
					}
					SetBackgroundColor(0, 128, 128);
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
			game.drawHistLast();
			game.drawNextField();
			// プレイヤーの操作
			if (game.isPlayTurn() && game.playTurn()) {
				reward = game.update();
				if (reward > -10.0) {
					if (game.isVsCOM()) {
						com.setWait();
						// reward2 = -rwd_tmp;
					}
				}
			}
			game.drawLocalState();
			game.drawCurrentCoord();

			// メッセージの描画
			game.drawBattleMessage();

			// 学習機械の出力描画
			if (game.option.likeliFlg >= 1) {
				com.visualize();
			}

			// COMの手番
			if (!game.isPlayTurn()) {
				VectorXd input = game.stateToInput();
				com.play(input);
				//盤面の更新
				reward = game.update(com.globalX, com.globalY, com.localX, com.localY);
				if (reward > -10.0) {
					//temp_i[trainCnt] = input;
					//temp_o[trainCnt] = Reward1(output, input, 1 - 2 * (game.cnt % 2));
					//temp_o[trainCnt](comHistt[trainCnt]) = reward;
					//if (trainCnt >= 1) {
					//	if (game.vsCOM()) {
					//		temp_o[trainCnt - 1](comHistt[trainCnt - 1]) += gamma * max_val + reward2;
					//	}
					//	else if (game.taijin == 2) {
					//		temp_o[trainCnt - 1](comHistt[trainCnt - 1]) -= reward;
					//		if (trainCnt >= 2) {
					//			temp_o[trainCnt - 2](comHistt[trainCnt - 2]) += gamma * max_val;
					//		}
					//	}
					//}
					//trainCnt++;
					//if (game.isAutoLearning) setWait();
				}
			}

			// コメントの描画
			game.drawComment();

			// カットインアニメーション
			game.cutin.update();

			// 勝利判定
			vict = game.mother.victory();
			if (vict != 0) {
				game.goResult();
				game.key.initWait();
				//学習
				/*if (game.vsCOM()) {
					if (vict == teban * 2 - 1) {
						temp_o[trainCnt - 1](comHistt[trainCnt - 1]) = RWD_VICT;
					}else {
						temp_o[trainCnt - 1](comHistt[trainCnt - 1]) = -RWD_VICT;
					}
				}
				else if (game.taijin == 2) {
					temp_o[trainCnt - 2](comHistt[trainCnt - 2]) = -RWD_VICT;
					temp_o[trainCnt - 1](comHistt[trainCnt - 1]) = RWD_VICT;
				}
				train_i.setZero(trainCnt, lay_size[0]);
				train_o.setZero(trainCnt, lay_size[lay_len]);
				for (int i = 0; i < trainCnt; ++i) {
					train_i.block(i, 0, 1, lay_size[0]) = temp_i[i].transpose();
					train_o.block(i, 0, 1, lay_size[lay_len]) = temp_o[i].transpose();
				}
				if(!game.vsHuman()) critic.backprop(train_i, train_o);*/
			}

			// 動作の取り消し
			if (game.key.onBack() && game.goBackHist()) {
				/*
				for (int i = 0; i < 3; ++i) {
					for (int j = 0; j < 3; ++j) {
						for (int k = 0; k < 3; ++k) {
							for (int l = 0; l < 3; ++l) {
								input(27 * i + 9 * j + 3 * k + l) = game.child[i][j].state[k][l];
							}
						}
					}
				}
				output = critic.predict(input);
				*/
				com.setWait();
			}

			// カメラ操作
			game.camera.set();
			if (game.mouse.click()) {
				game.mouse.set();
			}
			if (game.mouse.onClick()) {
				theta -= (game.mouse.distDragX()) * 0.05;
				game.mouse.set();
			}
			//MV1SetRotationXYZ(ModelHandle, VGet(0.0, theta + DX_PI_F, 0.0));

			// 永遠に勝敗がつかない場合の処理
			game.stopDrawGame();

			// 高速学習モードへの切り替え
			game.toggleHighSpeedLearning();

			// 対戦スキップ（一人用デバッグ）
			game.skipBattle(scenario.flg);

		}
		// 勝敗表示
		else if (game.flg == 2) {
			// 盤面の描画
			//MV1DrawModel(ModelHandle);
			game.drawBase();
			game.drawGlobalState();
			game.drawHistLast();
			game.drawNextField();
			game.drawLocalState();

			// メッセージの描画
			game.drawWinner(vict);
			game.again.display(game.mouse, game.option.strColor);
			if (game.again.isClicked(game.mouse) || game.key.onReturn()){
				game.initialize();
				initializeTrain();
			}

			// 動作の取り消し
			if (game.key.onBack() && game.goBackHist()) {
				game.goBattle();
			}

			// カメラ操作
			if (game.mouse.click()) {
				game.mouse.set();
			}
			else if (game.mouse.onClick()) {
				game.camera.move(game.mouse.distDragX(), game.mouse.distDragY());
				game.mouse.set();
			}
			game.camera.zoom(game.mouse.wheel);

			// 自動学習モード
			if (game.taijin == 2) {
				game.initialize();
				initializeTrain();
			}
		}
		// Ending
		else if (game.flg == -4) {
			bgm.load("sound/bgm09.mp3");
			game.drawBase();
			game.drawGlobalState();
			game.drawHistLast();
			game.drawNextField();
			game.drawLocalState();
			game.drawCurrentCoord();
			game.logo.draw();
			ending.show(bgm, game.fps);
		}
		//高速学習モード
		//else if (game.flg == 5) {
		//	if (game.debugFlg == 1) {
		//		//デバッグ
		//		if (Key[KEY_INPUT_D] == 1) {
		//			if (dbg_cnt == 0) printf("%d\n", trainCnt);
		//			for (int j = 0; j < 9; ++j) {
		//				printf("\n");
		//				for (int k = 0; k < 9; ++k) {
		//					printf("%.3f  ", train_i(dbg_cnt, j * 9 + k));
		//					//printf("%.1f  ", temp_i[1](j));
		//				}
		//			}
		//			for (int j = 0; j < 9; ++j) {
		//				printf("\n");
		//				for (int k = 0; k < 9; ++k) {
		//					printf("%.3f  ", train_o(dbg_cnt, j * 9 + k));
		//					//printf("%.1f  ", temp_o[1](j));
		//				}
		//			}
		//			printf("\n");
		//			dbg_cnt++;
		//		}
		//		if (dbg_cnt >= trainCnt) {
		//			dbg_cnt = 0;
		//			debugFlg = 2;
		//		}
		//	}
		//	else if (game.debugFlg == 0) {
		//		//操作の処理
		//
		//		while (true) {
		//			//COMの手番
		//			input = StateToInput(lay_size[0], 1 - 2 * (game.cnt % 2));
		//			output = critic.predict(input);
		//			max_val = output.maxCoeff(&max_id);
		//			//COMGx = (max_id / 27) % 3; COMGy = (max_id / 9) % 3;
		//			//COMLx = (max_id / 3) % 3; COMLy = max_id % 3;
		//			p_output = softmax(output, alpha);
		//			double cum_tmp = 0.0;
		//			double ran_tmp = unif(mt);
		//			for (int k = 0; k < 81; ++k) {
		//				cum_tmp += p_output(k);
		//				if (ran_tmp <= cum_tmp) {
		//					COMGx = (k / 27) % 3; COMGy = (k / 9) % 3;
		//					COMLx = (k / 3) % 3; COMLy = k % 3;
		//					break;
		//				}
		//			}
		//			if (unif(mt) >= anl_rate) {
		//				//盤面の更新
		//				rwd_tmp = game.update(COMGx, COMGy, COMLx, COMLy);
		//				if (rwd_tmp > -10.0) {
		//					//comHistt[trainCnt] = max_id;
		//					comHistt[trainCnt] = COMGx * 27 + COMGy * 9 + COMLx * 3 + COMLy;
		//					train_correct_cnt++;
		//					correct_per_epic[epic]++;
		//				}
		//			}
		//			else {
		//				rwd_tmp = -100.0;
		//			}
		//			if (rwd_tmp < -10.0) {
		//				while (true) {
		//					COMGx = rand() % 3; COMGy = rand() % 3;
		//					COMLx = rand() % 3; COMLy = rand() % 3;
		//					//盤面の更新
		//					rwd_tmp = game.update(COMGx, COMGy, COMLx, COMLy);
		//					if (rwd_tmp > -10.0) {
		//						comHistt[trainCnt] = COMGx * 27 + COMGy * 9 + COMLx * 3 + COMLy;
		//						break;
		//					}
		//					//永遠に勝敗がつかない場合の処理
		//					game.stopDrawGame();
		//				}
		//			}
		//			temp_i[trainCnt] = input;
		//			temp_o[trainCnt] = Reward1(output, input, 1 - 2 * (game.cnt % 2));
		//			temp_o[trainCnt](COM_game.hist[trainCnt]) = rwd_tmp;
		//			//if (trainCnt >= 1) temp_o[trainCnt - 1](comHistt[trainCnt - 1]) -= rwd_tmp;
		//			if (trainCnt >= 2) temp_o[trainCnt - 2](comHistt[trainCnt - 2]) += gamma * max_val;
		//			trainCnt++;
		//			game.cnt++;
		//			train_turn_cnt++;
		//			turn_per_epic[epic]++;
		//
		//			//勝利判定
		//			vict = game.mother.victory();
		//			if (vict != 0) break;
		//
		//			//永遠に勝敗がつかない場合の処理
		//			game.stopDrawGame();
		//		}
		//		//学習
		//		temp_o[trainCnt - 2](comHistt[trainCnt - 2]) = -RWD_VICT;
		//		temp_o[trainCnt - 1](comHistt[trainCnt - 1]) = RWD_VICT;
		//		train_i.setZero(trainCnt, lay_size[0]);
		//		train_o.setZero(trainCnt, lay_size[lay_len]);
		//		for (int i = 0; i < trainCnt; ++i) {
		//			train_i.block(i, 0, 1, lay_size[0]) = temp_i[i].transpose();
		//			train_o.block(i, 0, 1, lay_size[lay_len]) = temp_o[i].transpose();
		//		}
		//		critic.backprop(train_i, train_o);
		//		tmp_loss = critic.loss(train_i, train_o);
		//		loss_per_epic[epic] += tmp_loss;
		//
		//		//game.debugFlg = 1;
		//		game.initialize(5);
		//
		//		//テキストの描画
		//		DrawFormatString(80, 400, StringColor, "試合数: %d", train_game_cnt);
		//		DrawFormatString(80, 424, StringColor, "試行手数: %d", train_turn_cnt);
		//		DrawFormatString(80, 448, StringColor, "正当率: %.2f%%", 100.0*(double)correct_per_epic[epic] / max(turn_per_epic[epic], 1));
		//		//DrawFormatString(240, 400, StringColor, "損失: %d", (int)tmp_loss);
		//		//DrawFormatString(240, 424, StringColor, "TPE: %d", turn_per_epic[epic]);
		//		DrawFormatString(240, 448, StringColor, "平均損失: %.2f", log(max(loss_per_epic[epic], 1.0)) - log(max(turn_per_epic[epic], 1.0)));
		//		DrawFormatString(40, 50, Red, "5.0");
		//		DrawFormatString(30, 350, Red, "-5.0");
		//		DrawFormatString(570, 50, Blue, "100%%");
		//		DrawFormatString(570, 350, Blue, "0%%");
		//
		//		train_game_cnt++;
		//		if (epic >= max_epic - 1) {
		//			if (train_game_cnt % game_per_epic == 0) {
		//				for (int k = 0; k < max_epic - 1; ++k) {
		//					loss_per_epic[k] = loss_per_epic[k + 1];
		//					correct_per_epic[k] = correct_per_epic[k + 1];
		//					turn_per_epic[k] = turn_per_epic[k + 1];
		//				}
		//				loss_per_epic[max_epic - 1] = 0;
		//				correct_per_epic[max_epic - 1] = 0;
		//				turn_per_epic[max_epic - 1] = 0;
		//				epic = max_epic - 1;
		//			}
		//		}
		//		else {
		//			epic = train_game_cnt / game_per_epic;
		//		}
		//
		//		for (int l = 0; l < 51; ++l) { DrawLine(70, 50 + 6 * l, 570, 50 + 6 * l, Black); }
		//		for (int l = 0; l < 11; ++l) { DrawLine(70, 50 + 30 * l + 1, 570, 50 + 30 * l + 1, Black); }
		//		for (int k = 1; k < max_epic; ++k) {
		//			DrawLine(500.0*(k - 1) / max_epic + 70,
		//				350.0 - 300.0*correct_per_epic[k - 1] / max(turn_per_epic[k - 1], 1),
		//				500.0*k / max_epic + 70,
		//				350.0 - 300.0*correct_per_epic[k] / max(turn_per_epic[k], 1),
		//				Blue);
		//			DrawLine(500.0*(k - 1) / max_epic + 70,
		//				350.0 - 30.0 * (5.0 + log(max(loss_per_epic[k - 1], 1.0)) - log(max(turn_per_epic[k - 1], 1))),
		//				500.0*k / max_epic + 70,
		//				350.0 - 30.0 * (5.0 + log(max(loss_per_epic[k], 1.0)) - log(max(turn_per_epic[k], 1))),
		//				Red);
		//		}
		//
		//		if (Key[KEY_INPUT_A] == 1) {
		//			game.mouse.set();
		//			game.goTitle();
		//			game.taijin = 0;
		//			for (int i = 0; i < 3; ++i) tama[i].initialize();
		//		}
		//
		//	}
		//	else if(debugFlg == 2){
		//		game.initialize(5);
		//		game.debugFlg = 0;
		//	}
		//}
		// シナリオ
		else if (game.flg == -6) {
			if (!bgm.drawLoadMsg() && scenario.show(game.mouse, bgm)) {
				game.initialize();
				initializeTrain();
			}
		}


		// 同期処理
		game.sync();

		// デバッグ情報出力
		game.debugDump();
		movie.debugDump(game.debugFlg);
		bgm.debugDump(game.debugFlg);
		scenario.debugDump(game.debugFlg);
		ending.debugDump(game.debugFlg);
		com.debugDump(game.debugFlg);
	}

	InitGraph();
	//MV1DeleteModel(ModelHandle);
	DxLib_End();

	return 0;
}
