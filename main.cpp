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
#include "lib/modes/music_room.h"
#include "lib/com.h"

#pragma comment(lib, "winmm.lib")

void routesBattle(int choice, int* flg, Title& title, Battle& battle);
void routesScenario(int choice, int* flg, Title& title, Scenario& scenario, Music& music);
void routesTutorial(int choice, int* flg, Title& title, Tutorial& tutorial);
void routesMusicRoom(int choice, int* flg, Title& title, MusicRoom& musicRoom);
void goEndingDebug(int* flg, Music& bgm, Ending& ending);
void goTitle(int* flg, Title& title);
void goResult(int* flg);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);
	SetAlwaysRunFlag(TRUE);
	SetMainWindowText("Maxence 0.5.2");
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
	MusicRoom musicRoom;
	Scenario scenario;
	Opening opening;
	Ending ending;
	Battle battle(&ui);
	BattleResult result(&ui);
	int flg = FLAG_OPENING;

	bool debug_flg = false;
	int strColorDebug = GetColor(0, 0, 0);

	COM com;

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
			opening.showDemo();
			if (opening.isOver()) flg = FLAG_TITLE;
		}
		else if (flg == FLAG_TITLE) {
			bgm.load("sound/bgm03.ogg"); // シナリオ用
			bgm.load("sound/bgm02.ogg"); // チュートリアル用
			int choice = title.show();
			routesBattle(choice, &flg, title, battle);
			routesScenario(choice, &flg, title, scenario, bgm);
			routesTutorial(choice, &flg, title, tutorial);
			routesMusicRoom(choice, &flg, title, musicRoom);
		}
		else if (flg == FLAG_TUTORIAL) {
			SetBackgroundColor(0, 0, 0);
			if (!bgm.drawLoadMsg()) {
				int res = tutorial.show(ui, bgm);
				if (res == FLAG_TITLE) {
					bgm.unloadAll();
					goTitle(&flg, title);
				}
			}
		}
		else if (flg == FLAG_BATTLE) {
			SetBackgroundColor(0, 0, 0);
			int res = battle.show(com, bgm, debug_flg);
			logo.draw(true);

			switch (res) {
			case FLAG_TITLE:
				goTitle(&flg, title);
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
			bgm.load("sound/bgm09.ogg");
			SetBackgroundColor(0, 0, 0);
			ending.drawGameBoard(battle.game);
			logo.draw();
			ending.show(bgm, sync.fps);
		}
		else if (flg == FLAG_SCENARIO) {
			SetBackgroundColor(0, 0, 0);
			if (!bgm.drawLoadMsg()) {
				int res = scenario.show(ui, com, bgm, debug_flg);
				if (res == FLAG_TITLE) {
					bgm.unloadAll();
					goTitle(&flg, title);
				}
			}
		}
		else if (flg == FLAG_MUSIC_ROOM) {
			SetBackgroundColor(0, 0, 0);
			int res = musicRoom.show(bgm);
			if (res == FLAG_TITLE) {
				bgm.unloadAll();
				goTitle(&flg, title);
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
			musicRoom.debugDump();
			scenario.debugDump();
			ending.debugDump();
			com.debugDump();
		}
	}

	InitGraph();
	DxLib_End();

	return 0;
}

void routesBattle(int choice, int* flg, Title& title, Battle& battle) {
	switch (choice) {
	case MENU_CHOICE_VS_HUMAN:
		if (!battle.hasSaveFile()) {
			battle.startVsHuman();
			*flg = FLAG_BATTLE;
		}
		break;
	case MENU_CHOICE_START:
		if (title.isBattleMode()) {
			battle.startVsHuman();
			*flg = FLAG_BATTLE;
		}
		break;
	case MENU_CHOICE_LOAD:
		if (title.isBattleMode()) {
			battle.load();
			*flg = FLAG_BATTLE;
		}
		break;
	}
}

void routesScenario(int choice, int* flg, Title& title, Scenario& scenario, Music& music) {
	switch (choice) {
	case MENU_CHOICE_VS_COM:
		if (!scenario.hasSaveFile()) {
			scenario.initialize();
			*flg = FLAG_SCENARIO;
		}
		break;
	case MENU_CHOICE_START:
		if (title.isScenarioMode()) {
			scenario.initialize();
			*flg = FLAG_SCENARIO;
		}
		break;
	case MENU_CHOICE_LOAD:
		if (title.isScenarioMode()) {
			scenario.load(music);
			*flg = FLAG_SCENARIO;
		}
		break;
	}
}

void routesTutorial(int choice, int* flg, Title& title, Tutorial& tutorial) {
	switch (choice) {
	case MENU_CHOICE_TUTORIAL:
		*flg = FLAG_TUTORIAL;
		tutorial.initialize();
		break;
	}
}

void routesMusicRoom(int choice, int* flg, Title& title, MusicRoom& musicRoom) {
	switch (choice) {
	case MENU_CHOICE_MUSIC_ROOM:
		*flg = FLAG_MUSIC_ROOM;
		musicRoom.initialize();
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

void goTitle(int* flg, Title& title) {
	title.initialize();
	SetBackgroundColor(0, 128, 128);
	*flg = FLAG_TITLE;
}

void goResult(int* flg) {
	*flg = FLAG_RESULT;
}
