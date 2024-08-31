#pragma once

#include <sstream>
#include <iomanip>
#include "lib/music.h"
#include "lib/user_input.h"
#include "lib/modes/common/message.h"
#include "lib/modes/common/fireflower.h"

const int MUSIC_NUM(15);
const int MUSIC_MAX_INDEX(MUSIC_NUM);
const int MUSIC_ROOM_FIRE_FLOWER_NUM(2);

const int MUSIC_LEFT_X(30);
const int MUSIC_LEFT_X2(300);
const int MUSIC_DIV_X(10);
const int MUSIC_TOP_Y(40);
const int MUSIC_DIV_Y(40);

// ���y�����[�h
class MusicRoom {
private:
	int strColorMenu = GetColor(255, 255, 255);
	//int strColor = GetColor(255, 255, 255);
	int strColorDebug = GetColor(255, 215, 0);

	struct MusicInfo {
		int id;
		string ext;
		string name;
		int who;
		string desc;
	};

	struct MusicInfo infoList[MUSIC_NUM] = {
		{ 1,	"m4a",	"It's Maxence!!!",					MESSAGE_WHO_YELLOW,	"---�܂����b�Z�[�W���Ȃ���---" },
		{ 2,	"ogg",	"Mr.K is talking to you",			MESSAGE_WHO_YELLOW,	"�`���[�g���A���̋Ȃ���\n�����ɂȂɂ��������������" },
		{ 3,	"ogg",	"�C�Ǝ��C�Ǝ�",						MESSAGE_WHO_YELLOW,	"�����̃e�[�}����" },
		{ 4,	"ogg",	"Mr.K is shouting at you",			MESSAGE_WHO_RED,	"Mr.K�i�ԁj�̃e�[�}��" },
		{ 5,	"ogg",	"Mr.K is smiling on you",			MESSAGE_WHO_GREEN,	"Mr.K�i�΁j�̃e�[�}�ł���" },
		{ 6,	"ogg",	"Mr.K is lying to you",				MESSAGE_WHO_BLUE,	"Mr.K�i�j�̃e�[�}�ł�" },
		{ 7,	"ogg",	"�^���̕��t�_",						MESSAGE_WHO_YELLOW,	"�Ȃɂ��ɓ��B�������̋Ȃ���" },
		{ 8,	"ogg",	"Mr.K is playing with you",			MESSAGE_WHO_YELLOW,	"Mr.K�i���j�̃e�[�}����" },
		{ 9,	"ogg",	"���͐Â��ɐX�ɋA��",				MESSAGE_WHO_YELLOW,	"�\�G���f�B���O�Ȃ���" },
		{ 10,	"ogg",	"Waterly blue mountain",			MESSAGE_WHO_YELLOW,	"---�܂����b�Z�[�W���Ȃ���---" },
		{ 11,	"ogg",	"Dear The Deer",					MESSAGE_WHO_YELLOW,	"---�܂����b�Z�[�W���Ȃ���---" },
		{ 12,	"ogg",	"The biggest deer",					MESSAGE_WHO_DEER,	"���̃e�[�}����" },
		{ 13,	"ogg",	"�s��������������m�����̃u���[�X",	MESSAGE_WHO_PLAYER,	"���G���f�B���O�Ȃ���" },
		{ 14,	"ogg",	"Fading memories",					MESSAGE_WHO_YELLOW,	"---�܂����b�Z�[�W���Ȃ���---" },
		{ 15,	"ogg",	"�ނ�D",							MESSAGE_WHO_YELLOW,	"�Q�[���I�[�o�[�̋Ȃ���" },
	};
	string defaultMsgStr = "�����͉��y��\n�����̕�������Ȃ��ǂ�������";

	Menu menu;
	Button buttons[MUSIC_NUM + 1]; // �����̗v�f�́u�^�C�g���ցv
	Message msg;
	fireflower tama[MUSIC_ROOM_FIRE_FLOWER_NUM];
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
		buttons[MUSIC_MAX_INDEX].initializeUsingLabelLen(520, 340, "�^�C�g����");
		menu.set(buttons, MUSIC_NUM + 1);
		for (int i = 0; i < MUSIC_ROOM_FIRE_FLOWER_NUM; ++i) {
			tama[i].initialize(60.0, 600.0, 520.0, 600.0);
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
			music.stop();
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
		msg.setWithoutNext(ss.str(), info.who);
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
			music.unload(1); // �K�v?
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
		for (int i = 0; i < MUSIC_ROOM_FIRE_FLOWER_NUM; ++i) {
			tama[i].draw();
			tama[i].tick();
		}
	}
};
