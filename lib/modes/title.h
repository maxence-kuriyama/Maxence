#pragma once

#include "lib/const.h"
#include "lib/menu.h"
#include "lib/modes/title/fireflower.h"


class Title {
private:
	fireflower tama[FIRE_FLOWER_NUM];
	int title_logo = LoadGraph("graph/Maxence_after4.png");
	Menu menu;
	Button btnLonely;
	Button btnVsHuman;
	Button btnSenko;
	Button btnKoko;
	int White = GetColor(255, 255, 255);
	bool soundFlg = false;

	void showFireFlower() {
		for (int i = 0; i < FIRE_FLOWER_NUM; ++i) {
			tama[i].draw();
			tama[i].tick();
		}
	}

	void setOrderMenu() {
		menu.set(btnSenko, btnKoko);
		flg = MENU_PLAYER_ORDER;
	}

public:
	int flg = MENU_GAME_MODE;

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
		btnSenko.initialize(TEXT1_X, TEXT1_Y, "先攻");
		btnKoko.initialize(TEXT2_X, TEXT2_Y, "後攻");
		menu.set(btnLonely, btnVsHuman);
		flg = MENU_GAME_MODE;
	}

	void show() {
		showFireFlower();
		DrawExtendGraph(160 + (rand() % 11) - 5.0, 170, 490 + (rand() % 11) - 5.0, 260, title_logo, TRUE);
	}
	
	int choose(UserInput& ui) {
		if (flg == MENU_GAME_MODE) {
			return chooseMode(ui);
		}
		else {
			return chooseOrder(ui);
		}
	}

	//タイトル画面その１
	int chooseMode(UserInput& ui) {
		int choice = menuChoose(ui);
		if (choice == 0) {
			setOrderMenu();
		}
		return choice;
	}

	//タイトル画面その２（「ぼっちで」選択時）
	int chooseOrder(UserInput& ui) {
		int choice = menuChoose(ui);
		return choice;
	}

	int menuChoose(UserInput& ui) {
		return menu.choose(ui, White);
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

};
