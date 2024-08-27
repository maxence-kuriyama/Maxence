#pragma once

#include <sstream>
#include <iomanip>
#include "lib/music.h"
#include "lib/user_input.h"
#include "lib/modes/common/message.h"
#include "lib/modes/common/fireflower.h"

const int MUSIC_NUM(15);
const int MUSIC_MAX_INDEX(MUSIC_NUM);

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
	int strColorDebug = GetColor(255, 215, 0);

	struct MusicInfo {
		int id;
		string ext;
		string name;
		string desc;
	};

	struct MusicInfo infoList[MUSIC_NUM] = {
		{ 1,	"m4a",	"It's Maxence!!!",					"---まだメッセージがないよ---" },
		{ 2,	"ogg",	"Mr.K is talking to you",			"チュートリアルの曲です\nここになにか説明を入れられるよ" },
		{ 3,	"ogg",	"海と樹海と私",						"部屋のテーマです" },
		{ 4,	"ogg",	"Mr.K is shouting at you",			"Mr.K（赤）のテーマです" },
		{ 5,	"ogg",	"Mr.K is smiling on you",			"Mr.K（緑）のテーマです" },
		{ 6,	"ogg",	"Mr.K is lying to you",				"Mr.K（青）のテーマです" },
		{ 7,	"ogg",	"運命の平衡点",						"なにかに到達した時の曲です" },
		{ 8,	"ogg",	"Mr.K is playing with you",			"Mr.K（黄）のテーマです" },
		{ 9,	"ogg",	"鹿は静かに森に帰る",				"表エンディング曲です" },
		{ 10,	"ogg",	"Waterly blue mountain",			"---まだメッセージがないよ---" },
		{ 11,	"ogg",	"Dear The Deer",					"---まだメッセージがないよ---" },
		{ 12,	"ogg",	"The biggest deer",					"鹿のテーマです" },
		{ 13,	"ogg",	"行き場を失った博士たちのブルース",	"裏エンディング曲です" },
		{ 14,	"ogg",	"Fading memories",					"---まだメッセージがないよ---" },
		{ 15,	"ogg",	"釣り船",							"ゲームオーバーの曲です" },
	};
	string defaultMsgStr = "ここは音楽室だよ\nお気に入りは見つかったかな";

	Menu menu;
	Button buttons[MUSIC_NUM + 1]; // 末尾の要素は「タイトルへ」
	Message msg;
	fireflower tama[FIRE_FLOWER_NUM];
	int choice = -1;
	bool isLoading = false;

public:

	MusicRoom() {
		initialize();
	}

	void initialize() {
		msg.initialize();
		msg.setWithoutNext(defaultMsgStr, MESSAGE_WHO_YELLOW);
		for (int i = 0; i < (MUSIC_NUM + 1) / 2; ++i) {
			int x = i * MUSIC_DIV_X + MUSIC_LEFT_X;
			int y = i * MUSIC_DIV_Y + MUSIC_TOP_Y;
			buttons[i].initializeUsingLabelLen(x, y, infoList[i].name);
		}
		for (int i = (MUSIC_NUM + 1) / 2; i < MUSIC_NUM; ++i) {
			int x = (i - (MUSIC_NUM + 1) / 2) * MUSIC_DIV_X + MUSIC_LEFT_X2;
			int y = (i - (MUSIC_NUM + 1) / 2) * MUSIC_DIV_Y + MUSIC_TOP_Y;
			buttons[i].initializeUsingLabelLen(x, y, infoList[i].name);
		}
		buttons[MUSIC_MAX_INDEX].initializeUsingLabelLen(520, 340, "タイトルへ");
		menu.set(buttons, MUSIC_NUM + 1);
		for (int i = 0; i < FIRE_FLOWER_NUM; ++i) {
			tama[i].setRange(100.0, 540.0, 650.0, 700.0);
			tama[i].initialize();
		}
		choice = -1;
		isLoading = false;
	}

	int show(UserInput& ui, Music& music) {
		showFireFlower();
		msg.draw();
	
		int currentChoice = choose(ui);

		if (currentChoice == MUSIC_MAX_INDEX) return FLAG_TITLE;

		if (currentChoice != -1) {
			choice = currentChoice;
			updateMusicDesc();
		}

		if (currentChoice != -1 || isLoading) {
			if (play(music)) {
				isLoading = false;
			}
			else {
				isLoading = true;
				music.drawLoadMsg();
			}
		}
		return FLAG_MUSIC_ROOM;
	}

	void debugDump() {
		int strColor = strColorDebug;

		DrawFormatString(365, 65, strColor, "musicChoice: %d", choice);
	}

private:

	int choose(UserInput& ui) {
		return menu.choose(ui, strColorMenu);
	}

	bool reset(UserInput& ui) {}

	void updateMusicDesc() {
		MusicInfo info = infoList[choice];
		stringstream ss;
		ss << info.id << ".";
		ss << info.name << endl;
		ss << info.desc;
		msg.setWithoutNext(ss.str(), MESSAGE_WHO_YELLOW);
	}

	int play(Music& music) {
		if (choice == -1) return 1;

		MusicInfo info = infoList[choice];
		string filename = getMusicFileName(info);
		if (music.musicName[0] == filename) {
			return (music.stop() && music.play(MUSIC_START_FROM_TOP));
		}
		else if (music.musicName[1] == filename) {
			return (music.swapWithoutPlay() && music.play(MUSIC_START_FROM_TOP));
		}
		else {
			music.unload(1); // 必要?
			music.load(filename.c_str());
			return 0;
		}
	}

	string getMusicFileName(MusicInfo& info) {
		ostringstream ss;
		ss << "./sound/bgm";
		ss << setfill('0') << setw(2) << info.id;
		ss << "." << info.ext;
		return ss.str();
	}

	void showFireFlower() {
		for (int i = 0; i < FIRE_FLOWER_NUM; ++i) {
			tama[i].draw();
			tama[i].tick();
		}
	}
};
