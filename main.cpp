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

#include "lib/const.h"
#include "lib/mode.h"
#include "lib/utils/flag_store.h"
#include "lib/utils/music.h"
#include "lib/utils/synchronizer.h"
#include "lib/utils/com.h"
#include "lib/components/logo.h"
#include "lib/modes/title.h"
#include "lib/modes/tutorial.h"
#include "lib/modes/scenario.h"
#include "lib/modes/battle.h"
#include "lib/modes/battle_result.h"
#include "lib/modes/opening.h"
#include "lib/modes/ending.h"
#include "lib/modes/music_room.h"

#pragma comment(lib, "winmm.lib")

void routesBattle(int choice, int* flg, Title& title, Battle& battle);
void routesScenario(int choice, int* flg, Title& title, Scenario& scenario);
void routesTutorial(int choice, int* flg, Title& title, Tutorial& tutorial);
void routesMusicRoom(int choice, int* flg, Title& title, MusicRoom& musicRoom);
void goEndingDebug(int* flg, Ending& ending);
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

	
	Mode mode;
	Synchronizer sync(FPS);
	Logo logo;

	Title title;
	Tutorial tutorial;
	MusicRoom musicRoom;
	Scenario scenario;
	Opening opening;
	Ending ending;
	Battle battle;
	BattleResult result;

	int strColorDebug = GetColor(0, 0, 0);

	COM com;

	//メインループ
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
		// 入力情報を取得
		UserInput::update();

		// デバッグモード
		FlagStore::toggleDebug();

		// エンディングモードのデバッグ
		if (UserInput::onKeyEndingDebug()) goEndingDebug(&flg, ending);

		switch (mode.current()) {
		case MODE_OPENING:
			opening.showDemo();
			if (opening.isOver()) mode.goTitle();
			break;
		case MODE_TITLE:
			Music::load("sound/bgm03.ogg"); // シナリオ用
			Music::load("sound/bgm02.ogg"); // チュートリアル用
			int choice = title.show();
			routesBattle(choice, &flg, title, battle);
			routesScenario(choice, &flg, title, scenario);
			routesTutorial(choice, &flg, title, tutorial);
			routesMusicRoom(choice, &flg, title, musicRoom);
			break;
		case MODE_TUTORIAL:
			SetBackgroundColor(0, 0, 0);
			if (!Music::drawLoadMsg()) {
				int res = tutorial.show();
				if (res == MODE_TITLE) {
					Music::unloadAll();
					goTitle(&flg, title);
				}
			}
			break;
		case MODE_BATTLE:
			SetBackgroundColor(0, 0, 0);
			int res = battle.show(com);
			logo.draw(true);

			switch (res) {
			case MODE_TITLE:
				goTitle(&flg, title);
				break;
			case MODE_RESULT:
				goResult(&flg);
				break;
			}
			break;
		case MODE_RESULT:
			SetBackgroundColor(0, 0, 0);
			int res = result.show(battle.game);
			logo.draw();

			switch (res) {
			case MODE_BATTLE:
				battle.start(BATTLE_PLAYER_NONE, BATTLE_PLAYER_NONE);
				break;
			case MODE_RESULT_CANCEL:
				battle.start(BATTLE_PLAYER_NONE, BATTLE_PLAYER_NONE, false);
				res = MODE_BATTLE;
				break;
			}
			flg = res;
			break;
		case MODE_ENDING:
			Music::load("sound/bgm09.ogg");
			SetBackgroundColor(0, 0, 0);
			ending.drawGameBoard(battle.game);
			logo.draw();
			ending.show(sync.fps);
			break;
		case MODE_SCENARIO:
			SetBackgroundColor(0, 0, 0);
			if (!Music::drawLoadMsg()) {
				int res = scenario.show(com);
				if (res == MODE_TITLE) {
					Music::unloadAll();
					goTitle(&flg, title);
				}
			}
			break;
		case MODE_MUSIC_ROOM:
			SetBackgroundColor(0, 0, 0);
			int res = musicRoom.show();
			if (res == MODE_TITLE) {
				Music::unloadAll();
				goTitle(&flg, title);
			}
			break;
		}

		battle.tick();
		sync.execute();

		// デバッグ情報出力
		if (FlagStore::isDebug()) {
			DrawFormatString(5, 45, strColorDebug, "fps: %d", sync.fps);
			DrawFormatString(5, 65, strColorDebug, "gameFlg: %d", flg);
			DrawFormatString(5, 85, strColorDebug, "keyboardFlg: %d", UserInput::usingKeyboard());
			battle.debugDump();
			Music::debugDump();
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
			*flg = MODE_BATTLE;
		}
		break;
	case MENU_CHOICE_START:
		if (title.isBattleMode()) {
			battle.startVsHuman();
			*flg = MODE_BATTLE;
		}
		break;
	case MENU_CHOICE_LOAD:
		if (title.isBattleMode()) {
			battle.load();
			*flg = MODE_BATTLE;
		}
		break;
	}
}

void routesScenario(int choice, int* flg, Title& title, Scenario& scenario) {
	switch (choice) {
	case MENU_CHOICE_VS_COM:
		if (!scenario.hasSaveFile()) {
			scenario.initialize();
			*flg = MODE_SCENARIO;
		}
		break;
	case MENU_CHOICE_START:
		if (title.isScenarioMode()) {
			scenario.initialize();
			*flg = MODE_SCENARIO;
		}
		break;
	case MENU_CHOICE_LOAD:
		if (title.isScenarioMode()) {
			scenario.load();
			*flg = MODE_SCENARIO;
		}
		break;
	}
}

void routesTutorial(int choice, int* flg, Title& title, Tutorial& tutorial) {
	switch (choice) {
	case MENU_CHOICE_TUTORIAL:
		*flg = MODE_TUTORIAL;
		tutorial.initialize();
		break;
	}
}

void routesMusicRoom(int choice, int* flg, Title& title, MusicRoom& musicRoom) {
	switch (choice) {
	case MENU_CHOICE_MUSIC_ROOM:
		*flg = MODE_MUSIC_ROOM;
		musicRoom.initialize();
		break;
	}
}

void goEndingDebug(int* flg, Ending& ending) {
	Music::unloadAll();
	if (*flg != MODE_ENDING) {
		ending.initialize();
		*flg = MODE_ENDING;
	}
	else {
		*flg = MODE_TITLE;
	}
}

void goTitle(int* flg, Title& title) {
	title.initialize();
	SetBackgroundColor(0, 128, 128);
	*flg = MODE_TITLE;
}

void goResult(int* flg) {
	*flg = MODE_RESULT;
}
