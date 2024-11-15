#pragma once

#include "lib/const.h"
#include "lib/mode.h"
#include "lib/utils/music.h"
#include "lib/utils/user_input.h"
#include "lib/components/menu.h"
#include "lib/components/fireflower.h"

const int TITLE_GAME_MODE(1);
const int TITLE_LOAD_BATTLE(11);
const int TITLE_LOAD_SCENARIO(12);

const int TITLE_MENU_CHOICE_START_BATTLE(200);
const int TITLE_MENU_CHOICE_START_SCENARIO(201);
const int TITLE_MENU_CHOICE_LOAD_BATTLE(210);
const int TITLE_MENU_CHOICE_LOAD_SCENARIO(211);

class Title {
private:
	FireFlower tama[FIRE_FLOWER_NUM];
	int title_logo = LoadGraph("graph/Maxence_after4.png");
	Menu menu;
	Button btnLonely;
	Button btnVsHuman;
	Button btnTutorial;
	Button btnMusicRoom;
	Button btnStart;
	Button btnLoad;
	int White = GetColor(255, 255, 255);
	int flg = TITLE_GAME_MODE;

public:

	Title() {
		initialize();
	}

	~Title() {}

	void initialize() {
		for (int i = 0; i < FIRE_FLOWER_NUM; ++i) {
			tama[i].initialize();
		}

		btnLonely.initialize(TEXT1_X, TEXT1_Y, "ぼっちで");
		btnVsHuman.initialize(TEXT2_X, TEXT2_Y, "隣の人と");
		btnTutorial.initialize(TEXT3_X, TEXT3_Y, "教えを乞う");
		btnMusicRoom.initialize(TEXT4_X, TEXT4_Y, "音楽室");
		Button btnsForTitle[4] = { btnLonely, btnVsHuman, btnTutorial, btnMusicRoom };
		menu.set(btnsForTitle, 4);

		btnStart.initialize(TEXT1_X, TEXT1_Y, "はじめから");
		btnLoad.initialize(TEXT2_X, TEXT2_Y, "つづきから");
		flg = TITLE_GAME_MODE;
	}

	int show() {
		SetBackgroundColor(0, 128, 128);

		toggleSound();
		showFireFlower();
		showTitleLogo();

		int choice = choose();
		if (choice != MENU_CHOICE_NONE) {
			// initialize();
		}
		return choice;
	}

	void route(Mode& mode, int res) {
		switch (res) {
		case MENU_CHOICE_TUTORIAL:
			mode.goTutorial();
			initialize();
			break;
		case MENU_CHOICE_MUSIC_ROOM:
			mode.goMusicRoom();
			initialize();
			break;
		case MENU_CHOICE_VS_HUMAN:
			mode.goBattle();
			initialize();
			break;
		case MENU_CHOICE_VS_COM:
			mode.goScenario();
			initialize();
			break;
		case TITLE_MENU_CHOICE_START_BATTLE:
			mode.goBattle();
			initialize();
			break;
		case TITLE_MENU_CHOICE_START_SCENARIO:
			mode.goScenario();
			initialize();
			break;
		case TITLE_MENU_CHOICE_LOAD_BATTLE:
			mode.goBattle();
			initialize();
			break;
		case TITLE_MENU_CHOICE_LOAD_SCENARIO:
			mode.goScenario();
			initialize();
			break;
		}
	}

	void showTitleLogo() {
		int upLeftx = 160 + (rand() % 11) - 5.0;
		int upLefty = 170;
		int downRightx = 490 + (rand() % 11) - 5.0;
		int downRighty = 260;
		DrawExtendGraph(upLeftx, upLefty, downRightx, downRighty, title_logo, TRUE);
	}

	bool isBattleMode() {
		return flg == TITLE_LOAD_BATTLE;
	}

	bool isScenarioMode() {
		return flg == TITLE_LOAD_SCENARIO;
	}

private:

	void toggleSound() {
		if (UserInput::onKeySound()) {
			tama[0].sound = !tama[0].sound;
		}
	}

	void showFireFlower() {
		for (int i = 0; i < FIRE_FLOWER_NUM; ++i) {
			tama[i].draw();
			tama[i].tick();
		}
	}

	int choose() {
		if (flg == TITLE_GAME_MODE) {
			return chooseMode();
		}
		else {
			return chooseLoad();
		}
	}

	//タイトル画面その１
	int chooseMode() {
		int choice = menuChoose();
		switch (choice) {
		case 0:
			setLoadMenu();
			flg = TITLE_LOAD_SCENARIO;
			return MENU_CHOICE_VS_COM;
		case 1:
			setLoadMenu();
			flg = TITLE_LOAD_BATTLE;
			return MENU_CHOICE_VS_HUMAN;
		case 2:
			return MENU_CHOICE_TUTORIAL;
		case 3:
			return MENU_CHOICE_MUSIC_ROOM;
		default:
			return MENU_CHOICE_NONE;
		}
	}

	//タイトル画面その２（セーブロード）
	int chooseLoad() {
		int choice = menuChoose();
		switch (choice) {
		case 0:
			if (isBattleMode()) {
				return TITLE_MENU_CHOICE_START_BATTLE;
			}
			else if (isScenarioMode()) {
				return TITLE_MENU_CHOICE_START_SCENARIO;
			}
			return MENU_CHOICE_START;
		case 1:
			if (isBattleMode()) {
				return TITLE_MENU_CHOICE_LOAD_BATTLE;
			}
			else if (isScenarioMode()) {
				return TITLE_MENU_CHOICE_LOAD_SCENARIO;
			}
			return MENU_CHOICE_LOAD;
		default:
			return MENU_CHOICE_NONE;
		}
	}

	void setLoadMenu() {
		menu.set(btnStart, btnLoad);
	}

	int menuChoose() {
		return menu.choose(White);
	}
};
