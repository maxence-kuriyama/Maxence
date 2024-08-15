#pragma once

#include "lib/music.h"
#include "lib/user_input.h"
#include "lib/modes/common/message.h"

const int MUSIC_NUM(15);

const int MUSIC_LEFT_X(30);
const int MUSIC_LEFT_X2(300);
const int MUSIC_DIV_X(10);
const int MUSIC_TOP_Y(40);
const int MUSIC_DIV_Y(40);

// 音楽室モード
class MusicRoom {
private:
	int strColorMenu = GetColor(255, 255, 255);
	//int strColor = GetColor(255, 255, 255);
	int strColorDebug = GetColor(255, 255, 255);

	struct MusicInfo {
		int id;
		string name;
		string desc;
	};

	struct MusicInfo info[MUSIC_NUM] = {
		{ 1,	"It's Maxence!!!",					"---まだメッセージがないよ---" },
		{ 2,	"Mr.K is talking to you",			"チュートリアルの曲です。" },
		{ 3,	"海と樹海と私",						"部屋のテーマです。" },
		{ 4,	"Mr.K is shouting at you",			"Mr.K（赤）のテーマです。" },
		{ 5,	"Mr.K is smiling on you",			"Mr.K（緑）のテーマです。" },
		{ 6,	"Mr.K is lying to you",				"Mr.K（青）のテーマです。" },
		{ 7,	"運命の平衡点",						"なにかに到達した時の曲です。" },
		{ 8,	"Mr.K is playing with you",			"Mr.K（黄）のテーマです。" },
		{ 9,	"鹿は静かに森に帰る",				"表エンディング曲です。" },
		{ 10,	"Waterly blue mountain",			"---まだメッセージがないよ---" },
		{ 11,	"Dear The Deer",					"---まだメッセージがないよ---" },
		{ 12,	"The biggest deer",					"鹿のテーマです。" },
		{ 13,	"行き場を失った博士たちのブルース",	"裏エンディング曲です。" },
		{ 14,	"Fading memories",					"---まだメッセージがないよ---" },
		{ 15,	"釣り船",							"ゲームオーバーの曲です。" },
	};

	Menu menu;
	Button buttons[MUSIC_NUM];
	Message msg;

public:

	MusicRoom() {
		initialize();
		msg.initialize();
		msg.setWithoutNext("これはテストだよ。\nテストテストテスト", MESSAGE_WHO_YELLOW);
	}

	int show(UserInput& ui) {
		msg.draw();
		return choose(ui);
	}

	void debugDump() {
		int strColor = strColorDebug;

		//DrawFormatString(365, 405, strColor, "tutoTrigger: %s", battle_trigger);
		//DrawFormatString(365, 425, strColor, "tutoAction: %d", scene.action);
	}

private:

	void initialize() {
		for (int i = 0; i < (MUSIC_NUM + 1) / 2; ++i) {
			int x = i * MUSIC_DIV_X + MUSIC_LEFT_X;
			int y = i * MUSIC_DIV_Y + MUSIC_TOP_Y;
			buttons[i].initializeUsingLabelLen(x, y, info[i].name);
		}
		for (int i = (MUSIC_NUM + 1) / 2; i < MUSIC_NUM; ++i) {
			int x = (i - (MUSIC_NUM + 1) / 2) * MUSIC_DIV_X + MUSIC_LEFT_X2;
			int y = (i - (MUSIC_NUM + 1) / 2) * MUSIC_DIV_Y + MUSIC_TOP_Y;
			buttons[i].initializeUsingLabelLen(x, y, info[i].name);
		}
		menu.set(buttons, MUSIC_NUM);
	}

	int choose(UserInput& ui) {
		return menu.choose(ui, strColorMenu);
	}

	bool reset(UserInput& ui) {}
};
