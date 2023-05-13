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
#include "lib/modes/title.h"
#include "lib/modes/tutorial.h"
#include "lib/modes/scenario.h"
#include "lib/modes/battle.h"
#include "lib/modes/battle_result.h"
#include "lib/modes/opening.h"
#include "lib/modes/ending.h"
#include "lib/com.h"

#pragma comment(lib, "winmm.lib")

void routesTitle(int choice, int* flg, Battle& battle);
void goEndingDebug(int* flg, Music& bgm, Ending& ending);
void goTitle(int* flg, Title& title, Scenario& scenario);
void goBackScenario(int* flg, Scenario& scenario);
void goResult(int* flg);


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
	UserInput ui = { &key, &mouse };
	Music bgm;
	Synchronizer sync;
	Logo logo;

	Title title;
	Tutorial tutorial;
	Scenario scenario;
	Opening opening;
	Ending ending;
	Battle battle(&ui);
	BattleResult result(&ui);
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
		ui.update();

		// 設定を切り替える
		battle.toggleByKey(debug_flg);

		//デバッグモード
		if (ui.onKeyDebug()) debug_flg = !debug_flg;

		//音楽, SEの有無
		if (ui.onKeySound()) title.toggleSound();

		// エンディングモードのデバッグ
		if (ui.onKeyEndingDebug()) goEndingDebug(&flg, bgm, ending);


		if (flg == FLAG_OPENING) {
			opening.showDemo(ui);
			if (opening.isOver()) flg = FLAG_TITLE;
		}
		else if (flg == FLAG_TITLE) {
			bgm.load("sound/bgm03.ogg"); // シナリオ用
			bgm.load("sound/bgm02.ogg"); // チュートリアル用
			int choice = title.show(ui);
			routesTitle(choice, &flg, battle);
		}
		else if (flg == FLAG_TUTORIAL) {
			SetBackgroundColor(0, 0, 0);
			if (!bgm.drawLoadMsg()) {
				tutorial.show(ui, bgm);
			}
		}
		else if (flg == FLAG_BATTLE) {
			//MV1DrawModel(ModelHandle);
			SetBackgroundColor(0, 0, 0);
			int res = battle.show(com, bgm, debug_flg);
			logo.draw(ui);

			switch (res) {
			case FLAG_TITLE:
				goTitle(&flg, title, scenario);
				break;
			case FLAG_SCENARIO:
				goBackScenario(&flg, scenario);
				break;
			case FLAG_RESULT:
				goResult(&flg);
				break;
			}
		}
		else if (flg == FLAG_RESULT) {
			SetBackgroundColor(0, 0, 0);
			int res = result.show(battle.game);
			logo.draw();

			switch (res) {
			case FLAG_BATTLE:
				battle.start(BATTLE_PLAYER_NONE, BATTLE_PLAYER_NONE);
				break;
			case FLAG_RESULT_CANCEL:
				battle.start(BATTLE_PLAYER_NONE, BATTLE_PLAYER_NONE, false);
				res = FLAG_BATTLE;
				break;
			}
			flg = res;
		}
		else if (flg == FLAG_ENDING) {
			bgm.load("sound/bgm09.mp3");
			SetBackgroundColor(0, 0, 0);
			ending.drawGameBoard(battle.game);
			logo.draw();
			ending.show(bgm, sync.fps);
		}
		else if (flg == FLAG_SCENARIO) {
			if (!bgm.drawLoadMsg()) {
				int res = scenario.show(ui, bgm);
				if (res == SCENE_RES_GO_BATTLE) {
					flg = FLAG_BATTLE;
					battle.start(BATTLE_PLAYER_YELLOW, rand() % 3 + 1);
				}
			}
		}

		battle.tick();
		sync.execute();

		// デバッグ情報出力
		if (debug_flg) {
			DrawFormatString(5, 45, strColorDebug, "fps: %d", sync.fps);
			DrawFormatString(5, 65, strColorDebug, "gameFlg: %d", flg);
			DrawFormatString(5, 85, strColorDebug, "keyboardFlg: %d", ui.keyboard);
			battle.debugDump();
			bgm.debugDump();
			tutorial.debugDump();
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

void routesTitle(int choice, int* flg, Battle& battle) {
	switch (choice) {
	case MENU_CHOICE_VS_HUMAN:
		*flg = FLAG_BATTLE;
		battle.startVsHuman();
		break;
	case MENU_CHOICE_VS_COM:
		battle.setVsCOM();
		break;
	case MENU_CHOICE_VS_COM_SENKO:
		*flg = FLAG_SCENARIO;
		battle.setSenko();
		break;
	case MENU_CHOICE_VS_COM_KOKO:
		*flg = FLAG_SCENARIO;
		battle.setKoko();
		break;
	case MENU_CHOICE_TUTORIAL:
		*flg = FLAG_TUTORIAL;
		break;
	}
}

void goEndingDebug(int* flg, Music& bgm, Ending& ending) {
	bgm.unloadAll();
	if (*flg != FLAG_ENDING) {
		ending.initialize();
		*flg = FLAG_ENDING;
	}
	else {
		*flg = FLAG_TITLE;
	}
}

void goTitle(int* flg, Title& title, Scenario& scenario) {
	title.initialize();
	scenario.initialize();
	SetBackgroundColor(0, 128, 128);
	*flg = FLAG_TITLE;
}

void goBackScenario(int* flg, Scenario& scenario) {
	StopMusic();
	scenario.flg++;
	*flg = FLAG_SCENARIO;
}

void goResult(int* flg) {
	*flg = FLAG_RESULT;
}
