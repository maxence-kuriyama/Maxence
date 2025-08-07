#pragma once

#include <iostream>
#include <regex>
#include "lib/mode.h"
#include "lib/components/menu.h"
#include "./scenario_base.h"

using namespace std;

const int SCENE_ACTION_TUTO_WIN(21);
const int SCENE_ACTION_TUTO_LOSE(22);
const int SCENE_ACTION_TUTO_DRAW(23);

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
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�����͂��́w�Ֆʁx�ōs�����\n�Ԃ��g���c��3�A����3����ł����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�����̐Ԃ��g�́A���ꂼ�ꂪ3�~3�}�X�ō\�����ꂽ�w���Ֆʁx�ɂȂ��Ă����\n�S�̂�9�~9��81�}�X�ō\������Ă����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�܂��͈��A�w���Ă݂悤\n�}�E�X�J�[�\���Ń}�X��I������\n�w�E�N���b�N�x�Ō��肾" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"play_once" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:LAST]���܌N��[PREV]�̘g�̒��́u[LAST]�v�ɐ΂�u������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:NEXT]����ƁA���肪���ɐ΂�u����ꏊ��\n�w�S�̂̒��Łu[LAST]�v�x�Ɉʒu���邱�̐Ԙg���ɐ���������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�d�v�Ȃ��Ƃ����������������񌾂���" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�w�N���u�����΂̈ʒu�ɂ���āA���ɑ��肪�u����ꏊ�����������x��" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"����̔ԁ��N�̔Ԃ̏ꍇ�ł��A���̃��[���͓�������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�������瑊��̔Ԃ�����A���肪�ǂ��ɒu�������ڂ����" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"play_once" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:LAST]�����[PREV]�̘g���́u[LAST]�v�ɒu�����悤��" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:NEXT]�N�����ɒu����ꏊ�͂���\n�w�S�̂̒��Łu[LAST]�v�x�̐Ԙg������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"���Ⴀ�A�����v�̂ł�����x�̂Ƃ���܂ŔՖʂ�i�߂Ă݂悤\n�l�͂��΂炭�ق��Ă����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�w�ǂꂩ�̘g�̒��ŏc���A�����A�΂߂̎O�ڂ���ׂ邱�Ɓx\n��ڎw���Ă݂��炢���񂶂�Ȃ�����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"local_victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:LAST][PREV]�̘g�ŎO�ڕ��񂾂悤����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:LAST]�������������Ֆʂ͐F������\n�N�����ƐԂɁA���肪���Ɛ�" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"���낻�돟�������������悤\n�w�N�̐F�̏��Ֆʂ��O�ڕ��ׂ邱�Ɓx\n���ꂪ�N�̏���������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�����ɑ����U�����āA�N���u�������ꏊ�ɐ΂�u����悤�ɂ��邩���d�v�ɂȂ��" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�t�Ɂw����̐F�̏��Ֆʂ��O�ڕ��ׁx����ƌN�̕����������\n�U������Ȃ��悤���ӂ����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"���ꂶ�Ⴀ�A���s�����܂�܂Ői�߂Ă݂悤\n���Ȃ݂ɁA�F�̂������Ֆʂɂ���ɐ΂�u�����Ƃ͂ł��Ȃ���" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�c�����������悤����" },
		{ SCENE_ACTION_TUTO_LOSE,	MESSAGE_WHO_YELLOW,		"����͑���̏�����\n�c�O��������" },
		{ SCENE_ACTION_TUTO_DRAW,	MESSAGE_WHO_YELLOW,		"�����ƈ���������\n�����������Ƃ�����" },
		{ SCENE_ACTION_TUTO_WIN,	MESSAGE_WHO_YELLOW,		"����͌N�̏�����\n�������" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_YELLOW,		"end" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_YELLOW,		"exibit_nowait" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"���̐��E�̌������@�͂��������킩��������\n���ꂶ�Ⴀ�������F���Ă����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�܂����" },
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
		switch (scene.action) {
		case SCENE_ACTION_TUTO_WIN:
		case SCENE_ACTION_TUTO_LOSE:
		case SCENE_ACTION_TUTO_DRAW:
			readMsgAfterBattle(scene.action, scene.how, scene.who);
			break;
		default:
			break;
		}

		int res = ScenarioBase::show();

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
		if (regex_match(str.c_str(), m, regex(R"(\[MOVE:(?:LAST|NEXT)\]([\s\S]*))"))) {
			int x = 300;
			int y = 140;
			Coordinate last = battle.last();
			displayStr = replaceLastToken(m[1].str(), last);
			displayStr = replacePrevtToken(displayStr, last);
			if (regex_match(str.c_str(), m, regex(R"(\[MOVE:LAST\][\s\S]*)"))) {
				x = 100 + 100 * last.global_x + 33 * last.x;
				y = 60 + 100 * last.global_y + 33 * last.y;
			}
			else if (regex_match(str.c_str(), m, regex(R"(\[MOVE:NEXT\][\s\S]*)"))) {
				x = 100 + 100 * last.x;
				y = 60 + 100 * last.y;
			}
			mrK[0].set(x, y);
			mrK[0].draw();
		}
		else if (regex_match(str.c_str(), m, regex(R"([\s\S]*\[VICT\][\s\S]*)"))) {
			displayStr = replaceVictToken(str);
		}

		ScenarioBase::readMsg(displayStr, who);
	}

	void readMsgAfterBattle(int action, string str, int who) {
		if ((action == SCENE_ACTION_TUTO_WIN && !battle.isWon()) 
			|| (action == SCENE_ACTION_TUTO_LOSE && !battle.isLost())
			|| (action == SCENE_ACTION_TUTO_DRAW && !battle.isDraw())) {
			goNext();
			return;
		}
		readMsg(str, who);
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

	// override
	bool hasMsg(Scene scene) {
		if (ScenarioBase::hasMsg(scene)) return true;

		return (scene.action == SCENE_ACTION_TUTO_WIN
			|| scene.action == SCENE_ACTION_TUTO_LOSE
			|| scene.action == SCENE_ACTION_TUTO_DRAW);
	}

	string getPositionStr(int x, int y) {
		stringstream lastSs;
		switch (x) {
		case 0:
			lastSs << "��";
			break;
		case 2:
			lastSs << "�E";
			break;
		}
		switch (y) {
		case 0:
			lastSs << "��";
			break;
		case 2:
			lastSs << "��";
			break;
		}
		return (lastSs.str() == "") ? "�^��" : lastSs.str();
	}

	string replaceLastToken(string srcStr, Coordinate last) {
		string lastStr = getPositionStr(last.x, last.y);
		return regex_replace(srcStr, regex(R"(\[LAST\])"), lastStr);
	}

	string replacePrevtToken(string srcStr, Coordinate last) {
		string prevStr = getPositionStr(last.global_x, last.global_y);
		return regex_replace(srcStr, regex(R"(\[PREV\])"), prevStr);
	}

	string replaceVictToken(string srcStr) {
		string displayStr = srcStr;
		if (battle.isLost()) {
			displayStr = regex_replace(displayStr, regex(R"(\[VICT\])"), "����");
			displayStr = regex_replace(displayStr, regex(R"(\[MSG:VICT\])"), "�c�O��������");
		}
		else {
			displayStr = regex_replace(displayStr, regex(R"(\[VICT\])"), "�N");
			displayStr = regex_replace(displayStr, regex(R"(\[MSG:VICT\])"), "�������");
		}
		return displayStr;
	}
};
