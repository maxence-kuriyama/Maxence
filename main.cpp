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
#include "lib/modes/title.h"
#include "lib/modes/tutorial.h"
#include "lib/modes/scenario.h"
#include "lib/modes/battle.h"
#include "lib/modes/opening.h"
#include "lib/modes/ending.h"
#include "lib/modes/music_room.h"

#pragma comment(lib, "winmm.lib")

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

	Title title;
	Tutorial tutorial;
	MusicRoom musicRoom;
	Scenario scenario;
	Opening opening;
	Ending ending;
	Battle battle;

	int strColorDebug = GetColor(0, 0, 0);

	COM com;

	//メインループ
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
		// 入力情報を取得
		UserInput::update();

		// デバッグモード
		FlagStore::toggleDebug();

		int res, choice;
		switch (mode.current()) {
		case MODE_OPENING:
			res = opening.show();
			opening.route(mode, res);
			break;
		case MODE_TITLE:
			Music::load("sound/bgm03.ogg"); // シナリオ用
			Music::load("sound/bgm02.ogg"); // チュートリアル用
			choice = title.show();
			switch (choice) {
			case MENU_CHOICE_VS_HUMAN:
				if (battle.hasSaveFile()) {
					choice = MENU_CHOICE_NONE;
				}
				battle.startNewVsHuman();
				break;
			case MENU_CHOICE_VS_COM:
				if (scenario.hasSaveFile()) {
					choice = MENU_CHOICE_NONE;
				}
				break;
			case MENU_CHOICE_START:
				if (title.isBattleMode()) {
					battle.startVsHuman();
				}
				break;
			case MENU_CHOICE_LOAD:
				if (title.isBattleMode()) {
					battle.load();
				}
				else if (title.isScenarioMode()) {
					scenario.load();
				}
				break;
			}
			title.route(mode, choice);
			break;
		case MODE_TUTORIAL:
			res = tutorial.show();
			tutorial.route(mode, res);
			break;
		case MODE_BATTLE:
			res = battle.show(com);
			battle.route(mode, res);
			break;
		case MODE_ENDING:
			Music::load("sound/bgm09.ogg");
			SetBackgroundColor(0, 0, 0);
			ending.drawGameBoard(battle.game);
			res = ending.show(sync.fps);
			ending.route(mode, res);
			break;
		case MODE_SCENARIO:
			res = scenario.show(com);
			scenario.route(mode, res);
			break;
		case MODE_MUSIC_ROOM:
			res = musicRoom.show();
			musicRoom.route(mode, res);
			break;
		}

		battle.tick();
		sync.execute();

		// エンディングモードのデバッグ
		if (FlagStore::isDebug() && UserInput::onKeyEndingDebug()) {
			if (mode.current() != MODE_ENDING) {
				Music::unloadAll();
				mode.goEnding();
			}
		}

		// デバッグ情報出力
		if (FlagStore::isDebug()) {
			DrawFormatString(5, 45, strColorDebug, "fps: %d", sync.fps);
			DrawFormatString(5, 65, strColorDebug, "gameFlg: %d", mode.current());
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
