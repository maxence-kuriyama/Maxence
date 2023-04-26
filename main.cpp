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

#include "lib/music.h"
#include "lib/synchronizer.h"
#include "lib/logo.h"
#include "lib/title.h"
#include "lib/scenario.h"
#include "lib/battle.h"
#include "lib/opening.h"
#include "lib/ending.h"
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


	Mouse mouse;
	Key key;
	Music bgm;
	Synchronizer sync;
	Logo logo;

	Title title;
	Scenario scenario;
	Opening opening;
	Ending ending;
	Battle battle(&mouse, &key);
	int flg = FLAG_OPENING;

	bool debug_flg = false;
	int strColorDebug = GetColor(0, 0, 0);

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


	//メインループ
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
		// 入力情報を取得
		key.update();
		mouse.update();

		// 設定を切り替える
		battle.toggleByKey(debug_flg);

		//デバッグモード
		if (key.state[KEY_INPUT_G] == 1) {
			debug_flg = !debug_flg;
		}

		//音楽, SEの有無
		if (key.state[KEY_INPUT_P] == 1) {
			title.toggleSound();
		}

		// エンディングモードのデバッグ
		if (key.state[KEY_INPUT_MINUS] == 1) {
			bgm.unloadAll();
			if (flg != FLAG_ENDING) {
				ending.initialize();
				flg = FLAG_ENDING;
			}
			else {
				flg = FLAG_TITLE;
			}
		}

		// マウス操作か否かを判定する
		battle.game.toggleMouseOrKeyboard();


		if (flg == FLAG_OPENING) {
			opening.showDemo(mouse, key);
			if (opening.isOver()) {
				flg = FLAG_TITLE;
			}
		}
		else if (flg == FLAG_TITLE) {
			bgm.load("sound/bgm03.mp3");
			title.show();
			int title_mode = title.flg;
			int choice = title.choose(mouse, key);
			if (title_mode == MENU_GAME_MODE) {
				if (choice == 0) {
					battle.setVsCOM();
				}
				else if (choice == 1) {
					battle.startVsHuman();
					flg = FLAG_BATTLE;
				}
			}
			else if (title_mode == MENU_PLAYER_ORDER) {
				if (choice == 0) {
					flg = FLAG_SCENARIO;
					battle.setSenko();
				}
				else if (choice == 1) {
					flg = FLAG_SCENARIO;
					battle.setKoko();
				}
			}
		}
		else if (flg == FLAG_BATTLE) {
			// 盤面の描画
			//MV1DrawModel(ModelHandle);
			SetBackgroundColor(0, 0, 0);	//背景色

			int res = battle.show(com, scenario.flg, bgm, debug_flg);
			logo.draw();
			logo.update(key); // 動く

			if (res == FLAG_TITLE) {
				title.initialize();
				scenario.initialize();
				SetBackgroundColor(0, 128, 128);
			}
			flg = res;
		}
		else if (flg == FLAG_RESULT) {
			// 盤面の描画
			//MV1DrawModel(ModelHandle);
			SetBackgroundColor(0, 0, 0);	//背景色
			flg = battle.showResult();
			logo.draw();
		}
		else if (flg == FLAG_ENDING) {
			bgm.load("sound/bgm09.mp3");
			SetBackgroundColor(0, 0, 0);	//背景色
			battle.drawForEnding();
			logo.draw();
			ending.show(bgm, sync.fps);
		}
		else if (flg == FLAG_SCENARIO) {
			scenario.getKey(key);
			if (!bgm.drawLoadMsg() && scenario.show(mouse, bgm)) {
				flg = FLAG_BATTLE;
				battle.start(BATTLE_PLAYER_YELLOW, rand() % 3 + 1);
			}
		}

		battle.game.tick();
		sync.execute();

		// デバッグ情報出力
		if (debug_flg) {
			DrawFormatString(5, 25, strColorDebug, "fps: %d", sync.fps);
			DrawFormatString(5, 45, strColorDebug, "gameFlg: %d", flg);
			battle.debugDump();
			bgm.debugDump();
			scenario.debugDump();
			ending.debugDump();
			com.debugDump();
		}
	}

	InitGraph();
	//MV1DeleteModel(ModelHandle);
	DxLib_End();

	return 0;
}

