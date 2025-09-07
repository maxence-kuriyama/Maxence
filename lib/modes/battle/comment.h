#pragma once

#include <iostream>
#include <fstream>
#include "lib/nlohmann/json.hpp"
#include "lib/components/draw_round_box.h"

using namespace std;

void DrawObtainsString2(int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor);
string UTF8toSjis(string src);

const string FILE_JSON_SAYINGS("./data/sayings.json");
const float REPLACE_TEXT_SECOND(3.0);


// �e�L�X�g��ێ�����I�u�W�F�N�g
// Comment�N���X�̃����o�Ƃ��Ă̎g�p��z��
class Texts {
public:
	nlohmann::json data;
	int size = 0;
	string types = "";

	// �e�L�X�g�t�@�C���̓ǂݍ���
	void initialize() {
		ifstream ifs(FILE_JSON_SAYINGS.c_str());
		if (ifs.good()) ifs >> data;
		ifs.close();
		size = data.size();
	}

	string get(int num) {
		string str_utf8 = data[num]["msg"].get<string>();
		types = data[num]["type"].dump();
		return UTF8toSjis(str_utf8);
	}

	bool isTypeOf(int num, string type) {
		for (const auto& elem : data[num]["type"]) {
			if (elem.get<string>() == type) return true;
		}
		return false;
	}

};


// �������ɗ����R�����g�̃N���X
// Game�I�u�W�F�N�g�̃����o�Ƃ��Ă̎g�p��z��
class Comment {
private:
	static int fontHandle;

	double rn() { return (rand() % 10000) / 10000.0; }

public:
	Texts texts;
	int x = rand() % 200;
	int y = rand() % 400;
	int textNum = 0;	// �e�L�X�g�̒ʂ��ԍ�
	int textSeq = 0;	// �e�L�X�g�������A���������̃J�E���^
	int cnt = 0;		// �e�L�X�g�����ւ��p�J�E���^

	Comment() {
		if (Comment::fontHandle == NULL) {
			Comment::fontHandle = CreateFontToHandle("HG���ȏ���", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
		}
	}

	void initialize() {
		texts.initialize();
		cnt = 0;
		textSeq = 0;
		x = rand() % 200;
		y = rand() % 400;
	}

	void draw(int strColor) {
		string text = texts.get(textNum);
		DrawObtainsString2(x + 20, y + 10, 560, GetFontSize(), text.c_str(), strColor, Comment::fontHandle, GetColor(250, 250, 150));
	}

	void update(int playCnt = 0) {
		// �e�L�X�g�̍����ւ�
		if (cnt > REPLACE_TEXT_SECOND * FPS) {
			//������x�A�Ԃ������悤�ɐݒ�
			if (textNum < texts.size - 1 && rn() < pow(0.90, pow(2.0, textSeq))) {
				if (playCnt > COMMENT_CHANGE_TYPE_LONG_THINKING_TIME && forceUpdate(COMMENT_CHANGE_TYPE_LONG_THINKING, 0.70)) {
					// ���l�Ɣ��f + �������Ȃ�
				}
				else if (texts.isTypeOf(textNum + 1, COMMENT_CHANGE_TYPE_SEQUENTIAL)) {
					goNext();
				}
				else {
					if (rn() < 0.30) goNext();
				}
				textSeq++;
			}
			else {
				textNum = rand() % texts.size;
				textSeq = 0;
				move();
			}
			cnt = 0;
		}
		cnt++;
	}

	bool forceUpdate(string type, double prob) {
		if (rn() > prob) return false;
		for (int i = 0; i <= 100; i++) {
			textNum = rand() % texts.size;
			if (texts.isTypeOf(textNum, type)) break;
		}
		textSeq = 0;
		cnt = 0;
		move();
		return true;
	}

	void goNext() {
		textNum++;
		move();
	}

	void move() {
		x = rand() % 200;
		y = rand() % 400;
	}

	/*===========================*/
	//    �f�o�b�O���
	/*===========================*/
	void debugDump(int strColor) {
		DrawFormatString(245, 25, strColor, "types: %s", texts.types);
		DrawFormatString(245, 45, strColor, "size: %d", texts.size);
		DrawFormatString(245, 65, strColor, "commX: %d", x);
		DrawFormatString(245, 85, strColor, "commY: %d", y);
		DrawFormatString(245, 105, strColor, "textNum: %d", textNum);
		DrawFormatString(245, 125, strColor, "textSeq: %d", textSeq);
		DrawFormatString(245, 145, strColor, "commCnt: %d", cnt);
	}
};

int Comment::fontHandle = NULL;


void DrawObtainsString2(int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor) {
	char TempStr[3];
	int StrLen;
	int i;
	int DrawX;
	int DrawY;
	int CharLen;
	int DrawWidth;
	int BoxWidth;
	int BoxHeight;
	double rate = 1.55;	// �������Ƃ̕`�敝�B�f�t�H���g�̃t�H���g�T�C�Y��20�ɑ΂��A���̃t�H���g�T�C�Y28�i1.4�{�j�{�]�T����������
	int r = 5;	//�l�p�`�̊ۊp�̔��a

	// ������S�̂̃o�C�g�����擾
	StrLen = strlen(String);

	//��x��Ԃł͂��邪�A��x�����o���̑傫���𑪂��Ă���A�ꕶ�����`�悷��
	DrawX = x;
	BoxWidth = 0;
	BoxHeight = AddY * rate;
	for (i = 0; i < StrLen; ) {
		if (_mbbtype((unsigned char)String[i], 0) == _MBC_LEAD) {
			// �S�p�����̏ꍇ
			TempStr[0] = String[i];
			TempStr[1] = String[i + 1];
			TempStr[2] = '\0';
			CharLen = 2;
		}
		else {
			// ���p�����̏ꍇ
			TempStr[0] = String[i];
			TempStr[1] = '\0';
			CharLen = 1;
		}
		// �P�����̕`�敝���擾
		DrawWidth = GetDrawStringWidth(String + i, CharLen) * rate;
		// �`��͈͂���͂ݏo��ꍇ�͉��s
		if (DrawX + DrawWidth > RightX) {
			DrawX = x;
			BoxWidth = RightX - x;
			BoxHeight += AddY * rate;
		}
		// �`����W�����炷
		DrawX += DrawWidth;
		i += CharLen;
	}
	if (DrawX - x + 5 > BoxWidth) { BoxWidth = DrawX - x + 5; }
	DrawRoundBox(x, y, r, BoxWidth, BoxHeight, BoxColor);

	DrawX = x;
	DrawY = y;
	for (i = 0; i < StrLen; ) {
		if (_mbbtype((unsigned char)String[i], 0) == _MBC_LEAD) {
			// �S�p�����̏ꍇ
			TempStr[0] = String[i];
			TempStr[1] = String[i + 1];
			TempStr[2] = '\0';
			CharLen = 2;
		}
		else {
			// ���p�����̏ꍇ
			TempStr[0] = String[i];
			TempStr[1] = '\0';
			CharLen = 1;
		}
		// �P�����̕`�敝���擾
		DrawWidth = GetDrawStringWidth(String + i, CharLen) * rate;
		// �`��͈͂���͂ݏo��ꍇ�͉��s
		if (DrawX + DrawWidth > RightX)
		{
			DrawX = x;
			DrawY += AddY * rate;
		}
		// �P�����`��
		DrawStringToHandle(DrawX, DrawY, TempStr, StrColor, FontHandle);
		// �`����W�����炷
		DrawX += DrawWidth;
		// �`�悷�镶����i�߂�
		i += CharLen;
	}
}

// Refer to https://sayahamitt.net/utf8��string���ꂽ��shiftjis��string�o�Ă���֐������/
std::string UTF8toSjis(std::string srcUTF8) {
	//Unicode�֕ϊ���̕����񒷂𓾂�
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

	//�K�v�ȕ�����Unicode������̃o�b�t�@���m��
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8����Unicode�֕ϊ�
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

	//ShiftJIS�֕ϊ���̕����񒷂𓾂�
	int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//�K�v�ȕ�����ShiftJIS������̃o�b�t�@���m��
	char* bufShiftJis = new char[lengthSJis];

	//Unicode����ShiftJIS�֕ϊ�
	WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

	std::string strSJis(bufShiftJis);

	delete bufUnicode;
	delete bufShiftJis;

	return strSJis;
}