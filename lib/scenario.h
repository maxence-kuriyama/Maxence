#pragma once

#include "lib/music.h"
#include "lib/message.h"
#include "lib/sprite.h"


// �V�i���I�Ǘ��p�N���X
// �P�̂ł̎g�p��z��
class Scenario {

// �V�[�� + ��l���̃A�N�V����
struct Scene {
	int action;
	int who;
	char how[100];
};


public:
	int flg = 0;		// �V�i���I�Ǘ��p�t���O
	MrK mrK[4];
	MrK deer;
	Message msg;

	Scenario() {
		imgRoom = LoadGraph("graph/room.bmp");
		imgCard = LoadGraph("graph/card.bmp");
		mrK[0].set(160, 120);
		mrK[0].setSerialImages(16, "graph/move_test", 1);
		mrK[1].set(480, 120, "graph/sprite12.png", 1);
		mrK[2].set(160, 240, "graph/sprite13.png", 1);
		mrK[3].set(480, 240, "graph/sprite14.png", 1);
		mrK[1].setSayings(sayings1);
		mrK[2].setSayings(sayings2);
		mrK[3].setSayings(sayings3);
		deer.set(270, 200, "graph/sprite15.png", 0);
		initialize();
		msg.initialize();
	}

private:
	int cnt = 0;		// �t���[���J�E���^
	int textCnt = 0;	// �e�L�X�g�J�E���^
	bool hasMsg = false;	// ���b�Z�[�W���Z�b�g����Ă��邩
	bool isTalking = false;	// NPC�Ɖ�b�����ۂ�
	string imgFront;	// �t�����g�T�C�h�ɕ\������摜
	int imgRoom;
	int imgCard;
	int eqX = 0;
	int eqY = 0;		// eq = earthquake
	int key = -1;		// �L�[�{�[�h���� 0: Up, 1: Right, 2: Down, 3: Left
	int onOk = 0;		// �L�[�{�[�h���́iEnter or Space�j
	int strColorDebug = GetColor(150, 0, 0);
	int strColorLoad = GetColor(0, 0, 0);
	struct Scene sceneList[400] = {
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_DESC,		"play" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_DESC,		"sound/bgm04.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"�\�\���E�͂P�̕����ŏo���Ă���B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_all" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"10" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_DEER,		"exibit" },
		{ SCENE_ACTION_GRAPH,	SCENE_WHO_DESC,		"card" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"�uMr.K�����E��łڂ��v" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_DEER,		"hide_nowait" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_GRAPH,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"�Ȃɂ��I�H" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		//{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: ����AMr.K�����Ȃ����A���Ȃ񂾁I" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_all" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"20" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_red" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"30" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_RED,		"swap" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_RED,		"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_RED,		"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_RED,		"sound/bgm05.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"�n���ȁA����Ȃ͂��ł͂Ȃ������̂Ɂc�B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"�Ō�̍Ŋ��͌�����Ȃ��ƌ��߂Ă����̂ɁA����Ȏ��ɗl�Ƃ͂ȁB" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"�����A�������˂ΐ��E�̕��󂪎~�܂�Ƃ����̂Ȃ�" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"���̎��ɂ��Ӗ�������������Ƃ������̂���Ȃ����c�c�B" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_RED,		"hide" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"hide_nowait" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_green" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"40" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_GREEN,	"swap" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_GREEN,	"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_GREEN,	"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_GREEN,	"sound/bgm06.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"���͌�������B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"�c�������O�ɂ����������鎞�����邾�낤�c�c�B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"��ɐ����Ă����B" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_GREEN,	"hide" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"exibit_nowait" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_blue" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"50" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_BLUE,		"pop" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_BLUE,		"" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_BLUE,		"sound/bgm07.mp3" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_BLUE,		"stop" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"�n���ȁA����ȁA���E�̉��ɐ����`�����X�𓦂��Ă��܂��Ȃ�āA�L�蓾�Ȃ��B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"�c�c�����A�����Ă������Ǝ��̂��傫�ȃ`�����X�������̂��c�c�B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"�c�c���܂����ȁB" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"hide" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_DESC,		"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_YELLOW,	"sound/bgm08.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K�͉��ɂȂ낤�Ƃ��Ă����̂��c�c�B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"���A�������I" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"K�Ƃ� king �̂��Ƃ������̂��B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K�B�l����������B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"�����A��͂��̐��E�ɂ���͖̂l�������B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"�����炱�̉��ł���l���E���Đ��E���~���Ă���B" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_YELLOW,	"pop" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_YELLOW,	"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_YELLOW,	"stop" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"���肪�Ƃ��B" },
		{ SCENE_ACTION_STOP,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"�����ƌ��͂������B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"���ƌ��͂������B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_PLAYER,	"���̃Q�[���̉������E��łڂ��Ƃ����Ȃ�c" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_PLAYER,	"���ł���v���C���[���A���̐��E�ɋ���M�l��łڂ��̂��낤�H" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"�C�Â��Ă��܂������B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"�Ȃ�Ύ��������B�B�A������ł��邱�Ƃ��m�F���邾�����B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"�������ė��Ȃ����B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"�c�c�c�c" },
		{ -1,					-1,					"" }
	};
	struct Saying sayings1[20] = {
		{ "10",		SCENE_WHO_BLUE,		"�M���͎��ɂƂ��āA���p���l�̂��鑶�݂ł��B" },
		{ "10",		SCENE_WHO_BLUE,		"���ꂩ������񂨕t���������������B" },
		{ "10",		SCENE_WHO_YELLOW,	"�c�c�i�����؂邩�j�B" },
		{ "20",		SCENE_WHO_BLUE,		"���̃J�[�h�c" },
		{ "20",		SCENE_WHO_BLUE,		"�{����������Ȃ����A�`�����X�����m��Ȃ��ȁB" },
		{ "20",		SCENE_WHO_YELLOW,	"�S�̐��A�R��Ă����B" },
		{ "20",		SCENE_WHO_BLUE,		"�z�b�I" },
		{ "30",		SCENE_WHO_BLUE,		"�u�c�u�c�c�i����͎g����I���܂��傤�I�j" },
		{ "30",		SCENE_WHO_YELLOW,	"�������Ă񂾂����B" },
		{ "40",		SCENE_WHO_DESC,		"��: �c�c�c�B" },
		{ "40",		SCENE_WHO_DESC,		"��: �c�c�c�c�c�c�c�B" },
		{ "40",		SCENE_WHO_DESC,		"���͊����I�܂��b��������ȁI�I" },
		{ "50",		SCENE_WHO_BLUE,		"���Ȃ�1�l�ɂȂ�܂ŉB��Ă��܂����B" },
		{ "50",		SCENE_WHO_BLUE,		"����2�l�������Ă���Ă��肪�Ƃ��B�����Ȃ邱�Ƃ̓J�[�h�����������番�����Ă�������ˁB" },
		{ "50",		SCENE_WHO_BLUE,		"�����ĘA��̋M�����n������΁A�����͎��̐��E�ɂȂ�󂾁B�������낤�H" },
		{ "50",		SCENE_WHO_BLUE,		"���͐��E��łڂ����肶��Ȃ��̂ł��B" },
		{ "50",		SCENE_WHO_BLUE,		"�M�����ז��Ȃ����Ȃ̂ł���B" },
		{ "999",	-1,					"" }
	};
	struct Saying sayings2[20] = {
		{ "10",		SCENE_WHO_RED,		"�������̓`�[���I�����ĉ���萳�`���I" },
		{ "10",		SCENE_WHO_YELLOW,	"�����A�������ȁB���ꂩ�������B" },
		{ "20",		SCENE_WHO_RED,		"�������̃J�[�h�͂��I����n���ɂ��Ă���̂����I�I" },
		{ "20",		SCENE_WHO_RED,		"�h�J�b�I�i�������R�鉹�j" },
		{ "20",		SCENE_WHO_BLUE,		"�I" },
		{ "20",		SCENE_WHO_YELLOW,	"���킟�A����������B�|���Ȃ��B" },
		{ "30",		SCENE_WHO_RED,		"���E���łт�Ƃ����͖̂{���̎��̂悤���ȁB"},
		{ "30",		SCENE_WHO_RED,		"�������A��̉�X�̂����̒N���łڂ��Ƃ����̂��H"},
		{ "30",		SCENE_WHO_RED,		"�M�l���H Mr.K�I�I�I"},
		{ "999",	-1,					"" }
	};
	struct Saying sayings3[20] = {
		{ "10",		SCENE_WHO_GREEN,	"���̕��a�̒��A���̂��߂ɐ����Ă���̂��낤�B" },
		{ "10",		SCENE_WHO_YELLOW,	"���O�͂������ꂾ�ȁB�l���Ă����傤���Ȃ��̂ɁB" },
		{ "10",		SCENE_WHO_BLUE,		"���͂��̐��E����ɓ��ꂽ���ł��˂��B" },
		{ "20",		SCENE_WHO_GREEN,	"�����������o�����̂ł��ˁc�B�M���͂ǂ����܂����H" },
		{ "20",		SCENE_WHO_YELLOW,	"�������ˑR���ꂽ�z�����҂����C�ɂȂ�Ȃ��B" },
		{ "20",		SCENE_WHO_GREEN,	"���������ł��B�z�̈Ӑ}�͓ǂ߂܂���ˁB" },
		{ "30",		SCENE_WHO_GREEN,	"���̒n�k�Ɛ���̃J�[�h�̊֌W�͂���̂ł��傤���H" },
		{ "30",		SCENE_WHO_YELLOW,	"�ł����̂܂܂ł͐��E���ۂ��Ȃ����B" },
		{ "30",		SCENE_WHO_GREEN,	"�����ł��ˁB���낻��ł��˂��c�B" },
		{ "40",		SCENE_WHO_GREEN,	"�M�l�AMr.K���E�����ȁI" },
		{ "40",		SCENE_WHO_GREEN,	"���̂��������荇���Ȃ��̂��낤�B�ł����ꂪ�������̓��m���W�܂������̐��Ȃ̂��낤�B" },
		{ "40",		SCENE_WHO_GREEN,	"������Mr.K��B�E���������B���̒��Ő������Ӗ������o��������Ȃ����B" },
		{ "40",		SCENE_WHO_GREEN,	"���������Ă���B" },
		{ "999",	-1,					"" }
	};

public:

	void initialize() {
		flg = 0;
		cnt = 0;
		hasMsg = false;
		mrK[0].exhibit();
		mrK[1].exhibit();
		mrK[2].exhibit();
		mrK[3].exhibit();
		deer.hide();
		imgFront = "";
		onOk = 0;
		isTalking = false;
	}

	int show(Mouse& mouse, Music& music) {
		// �w�i�E�l���̕`��
		DrawExtendGraph(0 + eqX, -50, 640 + eqX, 380, imgRoom, FALSE);
		drawMrKs();
		deer.draw();
		mrK[0].stop();
		showGraph();

		Scene scene = sceneList[flg];

		switch (scene.action) {
		case SCENE_ACTION_TALK:
			readMsg(scene.how, scene.who, mouse);
			break;
		case SCENE_ACTION_COCK:
			setTrigger(scene.how);
			break;
		case SCENE_ACTION_MOVE:
			doMove(scene.how, mouse);
			break;
		case SCENE_ACTION_EXIBIT:
			doExibit(scene.how, scene.who, mouse);
			break;
		case SCENE_ACTION_LOAD:
			music.load(scene.how);
			goNext();
			break;
		case SCENE_ACTION_MUSIC:
			performMusic(scene.how, music);
			break;
		case SCENE_ACTION_GRAPH:
			performGraph(scene.how, mouse);
			break;
		case SCENE_ACTION_EQ:
			performEQ(scene.how, mouse);
			break;
		case SCENE_ACTION_BATTLE:
			return 1;
		case SCENE_ACTION_STOP:
		default:
			break;
		}

		msg.draw();
		cnt = (cnt + 1) % 10000;

		return 0;
	}

	// �L�[�{�[�h���͂��擾����
	void getKey(Key& keyboard) {
		if (keyboard.onGoingDown()) {
			key = 0;
		}
		else if (keyboard.onGoingRight()) {
			key = 1;
		}
		else if (keyboard.onGoingUp()) {
			key = 2;
		}
		else if (keyboard.onGoingLeft()) {
			key = 3;
		}
		else {
			key = -1;
		}

		if (keyboard.onReturn()) {
			onOk = 1;
		}
		else {
			onOk = 0;
		}
	}

	void debugDump(int debug) {
		if (debug) {
			int strColor = strColorDebug;

			DrawFormatString(245, 205, strColor, "sceneFlg: %d", flg);
			DrawFormatString(245, 225, strColor, "frameCnt: %d", cnt);
			DrawFormatString(245, 245, strColor, "eqX: %d", eqX);
			DrawFormatString(245, 265, strColor, "textLen: %d", msg.textLen);
			DrawFormatString(245, 285, strColor, "charCnt: %d", int(msg.cnt * msg.cntPerFrame));
			DrawFormatString(245, 305, strColor, "who: %d", msg.who);
			DrawFormatString(245, 325, strColor, "mrK0.vis: %d", mrK[0].visible);
			DrawFormatString(245, 345, strColor, "mrK1.vis: %d", mrK[1].visible);
			DrawFormatString(245, 365, strColor, "mrK2.vis: %d", mrK[2].visible);
			DrawFormatString(245, 385, strColor, "mrK3.vis: %d", mrK[3].visible);
			DrawFormatString(245, 405, strColor, "deer.vis: %d", deer.visible);
			DrawFormatString(245, 425, strColor, "key: %d", key);
			DrawFormatString(245, 445, strColor, "isTalking: %s", isTalking ? "true" : "false");
			DrawFormatString(245, 465, strColor, "hasMsg: %s", hasMsg ? "true" : "false");
		}
	}


private:

	void waitClick(Mouse& mouse) {
		if (onOk || mouse.click()) {
			flg++;
		}
	}

	void goNext() {
		flg++;
	}

	// ���b�Z�[�W��ǂ�
	void readMsg(string str, int who, Mouse& mouse) {
		if (!hasMsg) {
			if (str == "clear") {
				msg.set("", who, false);
				flg++;
			}
			else {
				msg.set(str, who);
				hasMsg = true;
			}
		}
		if ((onOk || mouse.click()) && msg.skip()) {
			flg++;
			hasMsg = false;
		}
	}

	void doMove(const char how[], Mouse &mouse) {
		if (isTalking) {
			int who = checkMrK();
			talkMrK(who, how, mouse);
		}
		else {
			if (key != -1) {
				mrK[0].turn(key);
				mrK[0].move();
			}
			if (onOk || mouse.click()) {
				int who = checkMrK();
				talkResetMrK(who);
			}
			if (isTriggered()) goNext();
		}
	}

	void doExibit(string how, int who, Mouse& mouse) {
		if (how == "exibit") {
			if (who == SCENE_WHO_DEER) {
				deer.exhibit();
			}
			else {
				mrK[who - 1].exhibit();
			}
			waitClick(mouse);
		}
		else if (how == "hide") {
			if (who == SCENE_WHO_DEER) {
				deer.hide();
			}
			else {
				mrK[who - 1].hide();
			}
			waitClick(mouse);
		}
		else if (how == "exibit_nowait") {
			if (who == SCENE_WHO_DEER) {
				deer.exhibit();
			}
			else {
				mrK[who - 1].exhibit();
			}
			goNext();
		}
		else if (how == "hide_nowait") {
			if (who == SCENE_WHO_DEER) {
				deer.hide();
			}
			else {
				mrK[who - 1].hide();
			}
			goNext();
		}
	}

	void performMusic(string how, Music &music) {
		if (how == "play") {
			music.play();
		}
		else if (how == "stop") {
			music.stop();
			music.enableSwap();
		}
		else if (how == "pop_once") {
			music.popOnce();
		}
		else if (how == "swap") {
			if (music.swap(strColorLoad)) {
				music.enableSwap();
			}
		}
		else if (how == "pop") {
			music.pop(strColorLoad);
		}
		goNext();
	}

	void performGraph(string how, Mouse &mouse) {
		imgFront = how;
		if (how == "clear") {
			imgFront = "";
			goNext();
		}
		else {
			waitClick(mouse);
		}
	}

	void performEQ(string how, Mouse &mouse) {
		if (how == "true") {
			// happenEQ
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
			waitClick(mouse);
		}
		else {
			// stopEQ
			eqX = 0;
			goNext();
		}
	}

	void setTrigger(string trigger) {
		if (trigger == "talk_all") {
			mrK[0].setTrigger("fired");
			for (int i = 1; i < 4; ++i) {
				mrK[i].setTrigger("talk");
			}
		}
		else if (trigger == "talk_red") {
			for (int i = 0; i < 4; ++i) {
				mrK[i].setTrigger("fired");
			}
			mrK[SCENE_WHO_RED - 1].setTrigger("talk");
		}
		else if (trigger == "talk_green") {
			for (int i = 0; i < 4; ++i) {
				mrK[i].setTrigger("fired");
			}
			mrK[SCENE_WHO_GREEN - 1].setTrigger("talk");
		}
		else if (trigger == "talk_blue") {
			for (int i = 0; i < 4; ++i) {
				mrK[i].setTrigger("fired");
			}
			mrK[SCENE_WHO_BLUE - 1].setTrigger("talk");
		}
		else if (trigger == "none") {
			for (int i = 0; i < 4; ++i) {
				mrK[i].setTrigger("fired");
			}
		}
		goNext();
	}

	bool isTriggered() {
		for (int i = 0; i < 4; ++i) {
			if (!mrK[i].isTriggered()) {
				return false;
			}
		}
		return true;
	}

	void startMusic(const char* musicName) {
		if (CheckMusic() != 1) {
			PlayMusic(musicName, DX_PLAYTYPE_BACK);
		}
	}

	void showGraph() {
		if (imgFront == "card") {
			DrawExtendGraph(0, 0, 640, 400, imgCard, FALSE);
		}
	}

	void drawMrKs() {
		// ��U�S���`�悷��
		for (int i = 1; i < 4; ++i) {
			mrK[i].draw(eqX);
		}
		mrK[0].draw(eqX);

		// 0�Ԃ��O�ɂ��郄�c��������x�`��
		for (int i = 1; i < 4; ++i) {
			if (mrK[i].y > mrK[0].y) mrK[i].draw(eqX);
		}
	}

	int checkMrK() {
		// 0�Ԃ̋߂��ɂ��郄�c��pick up
		for (int i = 1; i < 4; ++i) {
			if (abs(mrK[i].y - mrK[0].y) < 45 && abs(mrK[i].x - mrK[0].x) < 45) {
				return i;
			}
		}
		return 0;
	}

	void talkResetMrK(int who) {
		if (who) {
			isTalking = true;
			mrK[who].talkReset();
		}
	}

	void talkMrK(int who, const char key[], Mouse &mouse) {
		if (who) {
			Saying saying = mrK[who].talk(key);
			if (strcmp(saying.say, "") == 0 || saying.who == -1) {
				isTalking = false;
				msg.set("", SCENE_WHO_DESC, false);
				return;
			}
			if (!hasMsg) {
				msg.set(saying.say, saying.who);
				hasMsg = true;
			}
			if ((onOk || mouse.click()) && msg.skip()) {
				mrK[who].talkNext();
				hasMsg = false;
			}
		}
	}

};
