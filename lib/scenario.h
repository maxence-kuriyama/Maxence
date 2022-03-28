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
	string text[50];
	int who[50];

	Scenario() {
		imgRoom = LoadGraph("graph/room.bmp");
		imgCard = LoadGraph("graph/card.bmp");
		mrK[0].set(160, 120);
		mrK[0].setSerialImages(16, "graph/move_test", 1);
		mrK[1].set(480, 120, "graph/sprite12.png", 1);
		mrK[2].set(160, 240, "graph/sprite13.png", 1);
		mrK[3].set(480, 240, "graph/sprite14.png", 1);
		deer.set(270, 200, "graph/sprite15.png", 0);
		initialize();
		msg.initialize();
	}

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
		case SCENE_ACTION_MOVE:
			if (key != -1) {
				mrK[0].turn(key);
				mrK[0].move();
			}
			mrK[0].walk();
			waitClick(mouse);
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
			performEQ(scene.how);
			waitClick(mouse);
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
	}

	void debugDump(int debug) {
		if (debug) {
			int strColor = strColorDebug;

			DrawFormatString(245, 205, strColor, "sceneFlg: %d", flg);
			DrawFormatString(245, 225, strColor, "frameCnt: %d", cnt);
			//DrawFormatString(245, 245, strColor, "textCnt: %d", textCnt);
			DrawFormatString(245, 265, strColor, "eqX: %d", eqX);
			DrawFormatString(245, 285, strColor, "textLen: %d", msg.textLen);
			DrawFormatString(245, 305, strColor, "charCnt: %d", int(msg.cnt * msg.cntPerFrame));
			DrawFormatString(245, 325, strColor, "who: %d", msg.who);
			DrawFormatString(245, 345, strColor, "mrK0.vis: %d", mrK[0].visible);
			DrawFormatString(245, 365, strColor, "mrK1.vis: %d", mrK[1].visible);
			DrawFormatString(245, 385, strColor, "mrK2.vis: %d", mrK[2].visible);
			DrawFormatString(245, 405, strColor, "mrK3.vis: %d", mrK[3].visible);
			DrawFormatString(245, 425, strColor, "deer.vis: %d", deer.visible);
			DrawFormatString(245, 445, strColor, "key: %d", key);
		}
	}


private:
	int cnt = 0;		// �t���[���J�E���^
	int textCnt = 0;	// �e�L�X�g�J�E���^
	bool hasMsg = false;	// ���b�Z�[�W���Z�b�g����Ă��邩
	string imgFront;	// �t�����g�T�C�h�ɕ\������摜
	int imgRoom;
	int imgCard;
	int eqX = 0;
	int eqY = 0;		// eq = earthquake
	int key = -1;		// �L�[�{�[�h���� 0: Up, 1: Right, 2: Down, 3: Left
	int strColorDebug = GetColor(150, 0, 0);
	int strColorLoad = GetColor(0, 0, 0);
	struct Scene sceneList[400] = {
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_DESC,		"play" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_DESC,		"sound/bgm04.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"�\�\���E�͂P�̕����ŏo���Ă���B" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_DEER,		"exibit" },
		{ SCENE_ACTION_GRAPH,	SCENE_WHO_DESC,		"card" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"�uMr.K�����E��łڂ��v" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_DEER,		"hide_nowait" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"hide_nowait" },
		{ SCENE_ACTION_GRAPH,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: �Ȃɂ��I�H" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: ����AMr.K�����Ȃ����A���Ȃ񂾁I" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"Mr.K: ���E���łт�Ƃ����͖̂{���̎��̂悤���ȁB" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"Mr.K: �������A��̉�X�̂����̒N���łڂ��Ƃ����̂��H �M�l���H Mr.K�I�I�I" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_RED,		"swap" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_RED,		"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_RED,		"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_RED,		"sound/bgm05.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"Mr.K: �n���ȁA����Ȃ͂��ł͂Ȃ������̂Ɂc�B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"Mr.K: �Ō�̍Ŋ��͌�����Ȃ��ƌ��߂Ă����̂ɁA����Ȏ��ɗl�Ƃ͂ȁB" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"Mr.K: �����A�������˂ΐ��E�̕��󂪎~�܂�Ƃ����̂Ȃ�" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"Mr.K: ���̎��ɂ��Ӗ�������������Ƃ������̂���Ȃ����c�c�c�c�B" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_RED,		"hide" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: �M�l�AMr.K���E�����ȁI" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: ���̂��������荇���Ȃ��̂��낤�B�ł����ꂪ�������̓��m���W�܂������̐��Ȃ̂��낤�B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: ������Mr.K��B�E���������B���̒��Ő������Ӗ������o��������Ȃ����B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: ���������Ă���B" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_GREEN,	"swap" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_GREEN,	"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_GREEN,	"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_GREEN,	"sound/bgm06.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: ���͌�������B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: �c�������O�ɂ����������鎞�����邾�낤�c�c�c�c�B��ɐ����Ă����B" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_GREEN,	"hide" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"exibit" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"Mr.K: ���Ȃ�1�l�ɂȂ�܂ŉB��Ă��܂����B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"Mr.K: ����2�l�������Ă���Ă��肪�Ƃ��B�����Ȃ邱�Ƃ̓J�[�h�����������番�����Ă�������ˁB" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"Mr.K: �����ĘA��̋M�����n������΁A�����͎��̐��E�ɂȂ�󂾁B�������낤�H" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"Mr.K: ���͐��E��łڂ����肶��Ȃ��̂ł��B�M�����ז��Ȃ����Ȃ̂ł���B" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_BLUE,		"pop" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_BLUE,		"" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_BLUE,		"sound/bgm07.mp3" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_BLUE,		"stop" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"Mr.K: �n���ȁA����ȁA���E�̉��ɐ����`�����X�𓦂��Ă��܂��Ȃ�āA�L�蓾�Ȃ��B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"Mr.K: �c�c�c�c�����A�����Ă������Ǝ��̂��傫�ȃ`�����X�������̂��c�c�c�c���܂����ȁB" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"hide" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_DESC,		"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_YELLOW,	"sound/bgm08.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: Mr.K�͉��ɂȂ낤�Ƃ��Ă����̂��c�c�c�c�B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: ���A�������I" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: K�Ƃ� king �̂��Ƃ������̂��B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: Mr.K�B�l����������B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: �����A��͂��̐��E�ɂ���͖̂l�������B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: �����炱�̉��ł���l���E���Đ��E���~���Ă���B" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_YELLOW,	"pop" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_YELLOW,	"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_YELLOW,	"stop" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: ���肪�Ƃ��B" },
		{ SCENE_ACTION_STOP,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"��: �����ƌ��͂������B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"��: ���ƌ��͂������B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_PLAYER,	"���̃Q�[���̉������E��łڂ��Ƃ����Ȃ�c" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_PLAYER,	"���ł���v���C���[���A���̐��E�ɋ���M�l��łڂ��̂��낤�H" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"��: �C�Â��Ă��܂������B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"��: �Ȃ�Ύ��������B�B�A������ł��邱�Ƃ��m�F���邾�����B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"��: �������ė��Ȃ����B" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"��: �c�c�c�c" },
		{ -1,					-1,					"" }
	};

	// ���b�Z�[�W��ǂ�
	void readMsg(string str, int who, Mouse& mouse) {
		if (!hasMsg) {
			msg.set(str, who);
			hasMsg = true;
		}
		if (mouse.click() && msg.skip()) {
			flg++;
			hasMsg = false;
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
		}
		waitClick(mouse);
	}

	void waitClick(Mouse& mouse) {
		if (mouse.click()) {
			flg++;
		}
	}

	void goNext() {
		flg++;
	}

	void performEQ(string how) {
		if (how == "true") {
			// happenEQ
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
		}
		else {
			// stopEQ
			eqX = 0;
		}
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

};
