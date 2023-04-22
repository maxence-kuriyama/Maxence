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
#include "lib/logo.h"
#include "lib/scenario.h"
#include "lib/game.h"
#include "lib/opening.h"
#include "lib/ending.h"
#include "lib/music.h"
#include "lib/com.h"

#pragma comment(lib, "winmm.lib")

const int FLAG_OPENING(-1);
const int FLAG_TITLE(0);
const int FLAG_BATTLE(1);
const int FLAG_RESULT(2);
const int FLAG_SCENARIO(-6);
const int FLAG_ENDING(-4);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);
	SetAlwaysRunFlag(TRUE);
	SetMainWindowText("Maxence 0.5.1");
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


	Mouse mouse;
	Key key;
	Logo logo;
	Music bgm;
	Scenario scenario;
	Opening opening;
	Ending ending;
	fireflower tama[FIRE_FLOWER_NUM];
	int title_logo = LoadGraph("graph/Maxence_after4.png");

	Game game(&mouse, &key);
	int flg = FLAG_OPENING;
	COM com;


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
		key.update();
		mouse.update();

		// 設定を切り替える
		game.toggleByKey(bgm);
		if (game.option.soundFlg == 1) {
			tama[0].sound = 1;
		}
		else {
			tama[0].sound = 0;
		}

		// マウス操作か否かを判定する
		game.toggleMouseOrKeyboard();

		// エンディングのデバッグ
		if (game.debugEndingFlg) {
			if (flg != FLAG_ENDING) {
				bgm.unloadAll();
				ending.initialize();
				flg = FLAG_ENDING;
			}
			else {
				bgm.unloadAll();
				flg = FLAG_TITLE;
			}
			game.debugEndingFlg = 0;
		}


		if (flg == FLAG_OPENING) {
			opening.showDemo(mouse, key);
			if (opening.isOver()) {
				flg = FLAG_TITLE;
			}
		}
		else if (flg == FLAG_TITLE) {
			bgm.load("sound/bgm03.mp3");
			for (int i = 0; i < FIRE_FLOWER_NUM; ++i) {
				tama[i].draw();
				tama[i].tick();
			}
			DrawExtendGraph(160 + (rand() % 11) - 5.0, 170, 490 + (rand() % 11) - 5.0, 260, title_logo, TRUE);
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
					flg = FLAG_BATTLE;
					game.goBattle(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_YELLOW);
				}
			}
			//タイトル画面その２（「ぼっちで」選択時）
			else if (game.isVsCOM()) {
				int choice = game.menuChoose();
				if (choice == 0) {
					flg = FLAG_SCENARIO;
					game.teban = TEBAN_SENKO;
				}
				else if (choice == 1) {
					flg = FLAG_SCENARIO;
					game.teban = TEBAN_KOKO;
				}
			}
		}
		else if (flg == FLAG_BATTLE) {
			// 盤面の描画
			//MV1DrawModel(ModelHandle);
			SetBackgroundColor(0, 0, 0);	//背景色
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
					}
					game.updateCommentInBattle();
				}
			}
			game.drawLocalState();
			game.drawCurrentCoord();
			game.drawPlayers();
			logo.draw();
			logo.update(key); // 動く

			// メッセージの描画
			game.drawBattleMessage();

			// 学習機械の出力描画
			if (game.option.likeliFlg >= 1) {
				com.visualize();
			}

			// COMの手番
			if (!game.isPlayTurn()) {
				// VectorXd input = game.stateToInput();
				// com.play(input);
				com.playMinMax(game.board, game.currentSide());
				// 盤面の更新
				game.update(com.choice);
			}

			// コメントの描画
			game.drawComment();

			// カットインアニメーション
			game.cutin.update();

			// 勝利判定
			vict = game.victory();
			if (vict != 0) {
				flg = FLAG_RESULT;
				key.initWait();
			}

			// 動作の取り消し
			if (key.onBack() && game.goBackHist()) {
				com.setWait();
			}

			// セーブ or リセット
			int choice = game.menuChoose();
			if (choice == 0 || choice == 1) {
				flg = FLAG_TITLE;
				game.reset(bgm);
				for (int i = 0; i < FIRE_FLOWER_NUM; ++i) {
					tama[i].initialize();
				}
				scenario.initialize();
				SetBackgroundColor(0, 128, 128);
			}

			// カメラ操作
			game.camera.set();
			if (mouse.click()) {
				mouse.set();
			}
			if (mouse.onClick()) {
				theta -= (mouse.distDragX()) * 0.05;
				mouse.set();
			}
			//MV1SetRotationXYZ(ModelHandle, VGet(0.0, theta + DX_PI_F, 0.0));

			// 対戦スキップ（一人用デバッグ）
			if (game.skipBattle(scenario.flg)) {
				flg = FLAG_SCENARIO;
			}

		}
		else if (flg == FLAG_RESULT) {
			// 盤面の描画
			//MV1DrawModel(ModelHandle);
			SetBackgroundColor(0, 0, 0);	//背景色
			game.drawBase();
			game.drawGlobalState();
			game.drawHistLast();
			game.drawNextField();
			game.drawLocalState();
			logo.draw();

			// メッセージの描画
			game.drawWinner(vict);
			game.btnAgain.display(mouse, game.option.strColor);
			if (game.btnAgain.isClicked(mouse) || key.onReturn()){
				game.initialize();
				flg = FLAG_BATTLE;
				game.goBattle();
			}

			// 動作の取り消し
			if (key.onBack() && game.goBackHist()) {
				flg = FLAG_BATTLE;
				game.goBattle();
			}

			// カメラ操作
			if (mouse.click()) {
				mouse.set();
			}
			else if (mouse.onClick()) {
				game.camera.move(mouse.distDragX(), mouse.distDragY());
				mouse.set();
			}
			game.camera.zoom(mouse.wheel);

			// TODO シナリオの場合の終了 + 遷移処理
		}
		else if (flg == FLAG_ENDING) {
			bgm.load("sound/bgm09.mp3");
			SetBackgroundColor(0, 0, 0);	//背景色
			game.drawBase();
			game.drawGlobalState();
			game.drawHistLast();
			game.drawNextField();
			game.drawLocalState();
			game.drawCurrentCoord();
			logo.draw();
			ending.show(bgm, game.fps);
		}
		else if (flg == FLAG_SCENARIO) {
			scenario.getKey(key);
			if (!bgm.drawLoadMsg() && scenario.show(mouse, bgm)) {
				game.initialize();
				flg = FLAG_BATTLE;
				game.goBattle(BATTLE_PLAYER_YELLOW, rand() % 3 + 1);
			}
		}


		// 同期処理
		game.sync();

		// デバッグ情報出力
		game.debugDump();
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
