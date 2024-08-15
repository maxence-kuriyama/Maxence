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

// ���y�����[�h
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
		{ 1,	"It's Maxence!!!",					"---�܂����b�Z�[�W���Ȃ���---" },
		{ 2,	"Mr.K is talking to you",			"�`���[�g���A���̋Ȃł��B" },
		{ 3,	"�C�Ǝ��C�Ǝ�",						"�����̃e�[�}�ł��B" },
		{ 4,	"Mr.K is shouting at you",			"Mr.K�i�ԁj�̃e�[�}�ł��B" },
		{ 5,	"Mr.K is smiling on you",			"Mr.K�i�΁j�̃e�[�}�ł��B" },
		{ 6,	"Mr.K is lying to you",				"Mr.K�i�j�̃e�[�}�ł��B" },
		{ 7,	"�^���̕��t�_",						"�Ȃɂ��ɓ��B�������̋Ȃł��B" },
		{ 8,	"Mr.K is playing with you",			"Mr.K�i���j�̃e�[�}�ł��B" },
		{ 9,	"���͐Â��ɐX�ɋA��",				"�\�G���f�B���O�Ȃł��B" },
		{ 10,	"Waterly blue mountain",			"---�܂����b�Z�[�W���Ȃ���---" },
		{ 11,	"Dear The Deer",					"---�܂����b�Z�[�W���Ȃ���---" },
		{ 12,	"The biggest deer",					"���̃e�[�}�ł��B" },
		{ 13,	"�s��������������m�����̃u���[�X",	"���G���f�B���O�Ȃł��B" },
		{ 14,	"Fading memories",					"---�܂����b�Z�[�W���Ȃ���---" },
		{ 15,	"�ނ�D",							"�Q�[���I�[�o�[�̋Ȃł��B" },
	};

	Menu menu;
	Button buttons[MUSIC_NUM];
	Message msg;

public:

	MusicRoom() {
		initialize();
		msg.initialize();
		msg.setWithoutNext("����̓e�X�g����B\n�e�X�g�e�X�g�e�X�g", MESSAGE_WHO_YELLOW);
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
