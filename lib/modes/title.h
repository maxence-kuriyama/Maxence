#pragma once

#include "lib/const.h"
#include "lib/utils/music.h"
#include "lib/components/menu.h"
#include "./common/fireflower.h"

const int TITLE_GAME_MODE(1);
const int TITLE_LOAD_BATTLE(11);
const int TITLE_LOAD_SCENARIO(12);

class Title {
private:
	fireflower tama[FIRE_FLOWER_NUM];
	int title_logo = LoadGraph("graph/Maxence_after4.png");
	Menu menu;
	Button btnLonely;
	Button btnVsHuman;
	Button btnTutorial;
	Button btnMusicRoom;
	Button btnStart;
	Button btnLoad;
	int White = GetColor(255, 255, 255);
	bool soundFlg = false;
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
		showFireFlower();
		showTitleLogo();
		return choose();
	}

	void showTitleLogo() {
		int upLeftx = 160 + (rand() % 11) - 5.0;
		int upLefty = 170;
		int downRightx = 490 + (rand() % 11) - 5.0;
		int downRighty = 260;
		DrawExtendGraph(upLeftx, upLefty, downRightx, downRighty, title_logo, TRUE);
	}

	void toggleSound() {
		soundFlg = !soundFlg;
		if (soundFlg) {
			tama[0].sound = 1;
		}
		else {
			tama[0].sound = 0;
		}
	}

	bool isBattleMode() {
		return flg == TITLE_LOAD_BATTLE;
	}

	bool isScenarioMode() {
		return flg == TITLE_LOAD_SCENARIO;
	}

private:

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
			return MENU_CHOICE_START;
		case 1:
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
