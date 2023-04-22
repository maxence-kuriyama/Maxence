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
#include "lib/music.h"
#include "lib/com.h"

#pragma comment(lib, "winmm.lib")


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


	Game game;
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
		if (game.flg == GAME_FLAG_DEMO_FIRST){
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
				game.flg = GAME_FLAG_DEMO_SECOND;
				logoX = M_PI_2;
			}
		}
		// OPアニメーション ClickToStart点滅
		else if (game.flg == GAME_FLAG_DEMO_SECOND) {
			DrawExtendGraph(170, 170, 258, 260, MLogo, TRUE);
			DrawExtendGraph(250, 170, 490, 260, axence, TRUE);

			logoX = (logoX + 0.05); if (logoX >= 360.0) logoX -= 360.0;
			SetDrawBright(155 + 100*sin(logoX), 155 + 100*sin(logoX), 155 + 100*sin(logoX));
			DrawExtendGraph(200, 290, 460, 360, ClickToStart, TRUE);
			SetDrawBright(255, 255, 255);

			if (game.mouse.click() || game.key.onReturn()) {
				game.flg = GAME_FLAG_DEMO_THIRD;
				SetBackgroundColor(0, 128, 128);	//背景色
				SetDrawBright(255, 255, 255);
			}
		}
		// OPアニメーション メインロゴ
		else if (game.flg == GAME_FLAG_DEMO_THIRD) {
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
				game.goTitle();
			}
		}
		else if (game.isTitle()) {
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
					game.mode = "隣の人と";
					game.initialize();
					game.goBattle(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_YELLOW);
				}
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
		else if (game.isBattle()) {
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
			game.drawLogo();

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
				game.goResult();
				game.key.initWait();
			}

			// 動作の取り消し
			if (game.key.onBack() && game.goBackHist()) {
				com.setWait();
			}

			// セーブ or リセット
			int choice = game.menuChoose();
			if (choice == 0 || choice == 1) {
				game.reset(bgm);
				for (int i = 0; i < FIRE_FLOWER_NUM; ++i) {
					tama[i].initialize();
				}
				scenario.initialize();
				SetBackgroundColor(0, 128, 128);
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

			// 対戦スキップ（一人用デバッグ）
			game.skipBattle(scenario.flg);

		}
		else if (game.isResult()) {
			// 盤面の描画
			//MV1DrawModel(ModelHandle);
			SetBackgroundColor(0, 0, 0);	//背景色
			game.drawBase();
			game.drawGlobalState();
			game.drawHistLast();
			game.drawNextField();
			game.drawLocalState();
			game.drawLogo();

			// メッセージの描画
			game.drawWinner(vict);
			game.btnAgain.display(game.mouse, game.option.strColor);
			if (game.btnAgain.isClicked(game.mouse) || game.key.onReturn()){
				game.initialize();
				game.goBattle();
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

			// TODO シナリオの場合の終了 + 遷移処理
		}
		else if (game.isEnding()) {
			bgm.load("sound/bgm09.mp3");
			SetBackgroundColor(0, 0, 0);	//背景色
			game.drawBase();
			game.drawGlobalState();
			game.drawHistLast();
			game.drawNextField();
			game.drawLocalState();
			game.drawCurrentCoord();
			game.drawLogo();
			ending.show(bgm, game.fps);
		}
		else if (game.isScenario()) {
			scenario.getKey(game.key);
			if (!bgm.drawLoadMsg() && scenario.show(game.mouse, bgm)) {
				game.initialize();
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
