#pragma once

#include <sstream>
#include <iomanip>
#include "lib/music.h"
#include "lib/user_input.h"
#include "lib/modes/common/message.h"

const int MUSIC_NUM(15);

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
	int strColorDebug = GetColor(255, 255, 255);

	struct MusicInfo {
		int id;
		string ext;
		string name;
		string desc;
	};

	struct MusicInfo infoList[MUSIC_NUM] = {
		{ 1,	"m4a",	"It's Maxence!!!",					"---�܂����b�Z�[�W���Ȃ���---" },
		{ 2,	"ogg",	"Mr.K is talking to you",			"�`���[�g���A���̋Ȃł��B" },
		{ 3,	"ogg",	"�C�Ǝ��C�Ǝ�",						"�����̃e�[�}�ł��B" },
		{ 4,	"ogg",	"Mr.K is shouting at you",			"Mr.K�i�ԁj�̃e�[�}�ł��B" },
		{ 5,	"ogg",	"Mr.K is smiling on you",			"Mr.K�i�΁j�̃e�[�}�ł��B" },
		{ 6,	"ogg",	"Mr.K is lying to you",				"Mr.K�i�j�̃e�[�}�ł��B" },
		{ 7,	"ogg",	"�^���̕��t�_",						"�Ȃɂ��ɓ��B�������̋Ȃł��B" },
		{ 8,	"ogg",	"Mr.K is playing with you",			"Mr.K�i���j�̃e�[�}�ł��B" },
		{ 9,	"ogg",	"���͐Â��ɐX�ɋA��",				"�\�G���f�B���O�Ȃł��B" },
		{ 10,	"ogg",	"Waterly blue mountain",			"---�܂����b�Z�[�W���Ȃ���---" },
		{ 11,	"ogg",	"Dear The Deer",					"---�܂����b�Z�[�W���Ȃ���---" },
		{ 12,	"ogg",	"The biggest deer",					"���̃e�[�}�ł��B" },
		{ 13,	"ogg",	"�s��������������m�����̃u���[�X",	"���G���f�B���O�Ȃł��B" },
		{ 14,	"ogg",	"Fading memories",					"---�܂����b�Z�[�W���Ȃ���---" },
		{ 15,	"ogg",	"�ނ�D",							"�Q�[���I�[�o�[�̋Ȃł��B" },
	};

	Menu menu;
	Button buttons[MUSIC_NUM];
	Message msg;
	int choice = -1;
	bool isLoading = false;

public:

	MusicRoom() {
		initialize();
		msg.initialize();
		msg.setWithoutNext("����̓e�X�g����B\n�e�X�g�e�X�g�e�X�g", MESSAGE_WHO_YELLOW);
	}

	void show(UserInput& ui, Music& music) {
		msg.draw();
	
		int currentChoice = choose(ui);
		if (currentChoice != -1) {
			choice = currentChoice;

			MusicInfo info = infoList[choice];
			stringstream ss;
			ss << info.id << ".";
			ss << info.name << endl;
			ss << info.desc;
			msg.setWithoutNext(ss.str(), MESSAGE_WHO_YELLOW);
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
			buttons[i].initializeUsingLabelLen(x, y, infoList[i].name);
		}
		for (int i = (MUSIC_NUM + 1) / 2; i < MUSIC_NUM; ++i) {
			int x = (i - (MUSIC_NUM + 1) / 2) * MUSIC_DIV_X + MUSIC_LEFT_X2;
			int y = (i - (MUSIC_NUM + 1) / 2) * MUSIC_DIV_Y + MUSIC_TOP_Y;
			buttons[i].initializeUsingLabelLen(x, y, infoList[i].name);
		}
		menu.set(buttons, MUSIC_NUM);
		choice = -1;
		isLoading = false;
	}

	int choose(UserInput& ui) {
		return menu.choose(ui, strColorMenu);
	}

	bool reset(UserInput& ui) {}

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
};