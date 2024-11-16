#pragma once

#include <string>
#include <regex>
#include "lib/components/sprite.h"
#include "lib/components/message.h"

using namespace std;

class ScenarioMessage {
private:
	Message msg;
	bool talking = false;	// NPC�Ɖ�b�����ۂ�
	string seName;			// SE�̃t�@�C����

public:

	ScenarioMessage() {
		initialize();
	}

	void initialize() {
		msg.initialize();
		talking = false;
		seName = "";
	}

	void draw() {
		msg.draw();
	}

	bool readNext(string str, int who) {
		if (msg.isShown) return false;

		if (str == "clear") {
			msg.setEmpty(who);
			return true;
		}
		else {
			msg.set(str, who);
		}
		return false;
	}

	void talkNext(Sprite* obj, const char key[], bool clicked = false) {
		// ����R�}���h
		Saying saying = obj->talk(key);
		if (strcmp(saying.say, "") == 0 || saying.who == -1) {
			finishTalk();
			return;
		}
		else {
			std::cmatch m;
			if (regex_match(saying.say, m, std::regex(R"(SE\[(.+)\])"))) {
				seName = m[1].str(); // �f�o�b�O�p
				playSE(seName);
				obj->talkNext();
				msg.isShown = false;
				return;
			}
		}

		// ���b�Z�[�W�\������
		if (!msg.isShown) {
			msg.set(saying.say, saying.who);
		}

		if (clicked && msg.skip()) obj->talkNext();
	}

	bool skip() {
		return msg.skip();
	}

	void debugDump(int strColor) {
		DrawFormatString(245, 245, strColor, "seName: %s", seName.c_str());
		DrawFormatString(245, 265, strColor, "textLen: %d", msg.textLen);
		DrawFormatString(245, 285, strColor, "charCnt: %d", int(msg.cnt * msg.cntPerFrame));
		DrawFormatString(245, 305, strColor, "who: %d", msg.who);
	}

	bool isTalking() {
		return talking;
	}

	void finishTalk() {
		talking = false;
		msg.setEmpty();
	}

	void startTalk() {
		talking = true;
	}

private:

	void playSE(string fileName) {
		PlaySoundFile(fileName.c_str(), DX_PLAYTYPE_BACK);
	}
};