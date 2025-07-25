#pragma once

#include <iostream>
#include <regex>
#include "lib/mode.h"
#include "lib/components/menu.h"
#include "./scenario_base.h"

using namespace std;

// �`���[�g���A���̃V�i���I�N���X
class Tutorial : public ScenarioBase {

public:

	Tutorial() {
		imgRoom = LoadGraph("graph/room0.png");

		sceneList.initialize(scenes);

		initialize();
	}

private:
	int imgRoom;
	Button btnReset;
	int strColorMenu = GetColor(255, 255, 255);

	int strColor = GetColor(255, 255, 255);

	struct Scene scenes[MAX_SCENE_NUM] = {
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_DESC,		"swap" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"�\�\���E�͂P�̕����ŏo���Ă���" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"����H�N�́c\n�ǂ����ŉ�����悤�ȁc" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�܂��A����͂Ƃ������Ƃ���" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�N�͂��̐��E�̌������@��m��K�v������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"���́c�H\n�c����͒ǁX�킩��񂶂�Ȃ�����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�Ƃɂ�������Ă݂悤��" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_YELLOW,		"start" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�����͂��̔Ֆʂōs�����\n�Ԃ��g���c��3�A����3����ł����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"���̐Ԃ��g�͂��ꂼ�ꂪ3�~3�̏��Ֆʂ���\n�S�̂�9�~9��81�}�X�ō\������Ă����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�܂��͈��A�w���Ă݂悤\n�}�E�X�J�[�\���Ń}�X��I�����āA�E�N���b�N�Ō��肾" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"play_once" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:LAST]���܌N�͐Ԃ��g�́u[LAST]�v�ɒu������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:NEXT]����ƁA���肪���ɒu����ꏊ�͑S�̂́u[LAST]�v�̐Ԙg���ɐ���������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�d�v�Ȃ��Ƃ����������������񌾂���" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�N���u�����΂̈ʒu�ɂ���āA���ɑ��肪�u����ꏊ������������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"����̔ԁ��N�̔Ԃ̏ꍇ�ł��A���̃��[���͓�������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�������瑊��̔Ԃ�����A���肪�ǂ��ɒu�������ڂ����" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"play_once" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"���Ȃ��̎��" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"local_victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"�I���܂ő����܂��傤" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"�I���" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_YELLOW,		"end" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_YELLOW,		"exibit_nowait" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"��������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"�Â��͂܂��Ȃ�" },
		{ -1,					-1,						"" },
	};

public:
	void initialize() {
		ScenarioBase::initialize();

		// �{�^��������
		btnReset.initialize(260, 440, "�^�C�g��");

		mrK[0].set(300, 140);
		mrK[1].hide();
		mrK[2].hide();
		mrK[3].hide();
	}

	int show() {
		SetBackgroundColor(0, 0, 0);

		if (Music::drawLoadMsg()) return MODE_TUTORIAL;

		// �w�i�̕`��
		DrawExtendGraph(0, -50, 640, 380, imgRoom, FALSE);

		Scene scene = sceneList.get();

		int res = ScenarioBase::show();
		//switch (scene.action) {
		//case SCENE_ACTION_TALK:
		//	moveForTeach(scene.how, scene.who);
		//	break;
		//default:
		//	break;
		//}

		return (res != SCENE_RES_DEFAULT) ? res : MODE_TUTORIAL;
	}

	void route(Mode& mode, int res) {
		if (res == MODE_TITLE) {
			initialize();
			Music::unloadAll();
			mode.goTitle();
		}
	}

	void debugDump() {
		int strColor = strColorDebug;
		Scene scene = sceneList.get();

		DrawFormatString(365, 405, strColor, "tutoTrigger: %s", battle.getTrigger());
		DrawFormatString(365, 425, strColor, "tutoAction: %d", scene.action);

		// game.debugDump();
	}

private:
	void readMsg(string str, int who) {
		cmatch m;
		string displayStr = str;
		int x = 300;
		int y = 140;
		stringstream lastSs;
		if (regex_match(str.c_str(), m, regex(R"(\[MOVE:(?:LAST|NEXT)\](.*))"))) {
			displayStr = m[1].str();
			Coordinate last = battle.last();
			switch (last.x) {
			case 0:
				lastSs << "��";
				break;
			case 2:
				lastSs << "�E";
				break;
			}
			switch (last.y) {
			case 0:
				lastSs << "��";
				break;
			case 2:
				lastSs << "��";
				break;
			}
			string lastStr = lastSs.str();
			if (lastStr == "") lastStr = "�^��";
			displayStr = regex_replace(displayStr, regex(R"(\[LAST\])"), lastStr);
			if (regex_match(str.c_str(), m, regex(R"(\[MOVE:LAST\].*)"))) {
				x = 100 + 100 * last.global_x + 33 * last.x;
				y = 60 + 100 * last.global_y + 33 * last.y;
			}
			else if (regex_match(str.c_str(), m, regex(R"(\[MOVE:NEXT\].*)"))) {
				x = 100 + 100 * last.x;
				y = 60 + 100 * last.y;
			}
			mrK[0].set(x, y);
			mrK[0].draw();
		}

		ScenarioBase::readMsg(displayStr, who);
	}

	// override
	void setBattle(string how) {
		if (how == "start") {
			battle.startTutorial(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_RED);
		}
		else if (how == "end") {
			battle.initialize();
		}
		goNext();
	}

	// override
	int doBattle(COM& com) {
		ScenarioBase::doBattle(com);

		if (reset()) return MODE_TITLE;

		return MODE_TUTORIAL;
	}

	bool reset() {
		btnReset.display(strColorMenu);
		if (btnReset.isClicked()) {
			battle.resetGame();
			return true;
		}
		return false;
	}
};
