#pragma once

#include <iostream>
#include <fstream>
#include "lib/nlohmann/json.hpp"
#include "lib/basic.h"

using namespace std;

void DrawObtainsString2(int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor);
string UTF8toSjis(string src);

const string FILE_JSON_SAYINGS("./data/sayings.json");


// �e�L�X�g��ێ�����I�u�W�F�N�g
// Comment�N���X�̃����o�Ƃ��Ă̎g�p��z��
class Texts {
public:
	nlohmann::json data;
	int size = 0;

	// �e�L�X�g�t�@�C���̓ǂݍ���
	void initialize() {
		ifstream ifs(FILE_JSON_SAYINGS.c_str());
		if (ifs.good()) ifs >> data;
		ifs.close();
		size = data.size();
	}

	string get(int id) {
		for (const auto& obj : data) {
			if (obj["id"] == to_string(id + 1)) {
				string str_utf8 = obj["msg"].get<string>();
				return UTF8toSjis(str_utf8);
			}
		}
		return string("���̃��b�Z�[�W�͕\������Ȃ��͂�����");
	}

};


// �������ɗ����R�����g�̃N���X
// Game�I�u�W�F�N�g�̃����o�Ƃ��Ă̎g�p��z��
class Comment {
private:
	int font = CreateFontToHandle("HG���ȏ���", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);

public:
	Texts texts;
	int x = rand() % 200;
	int y = rand() % 400;
	int textId = 0;		// �e�L�X�g�̃C���f�b�N�X
	int textSeq = 0;	// �e�L�X�g�������A���������̃J�E���^
	int cnt = 0;		// �e�L�X�g�����ւ��p�J�E���^
	int debugFlg = 0;

	void initialize() {
		texts.initialize();
		cnt = 0;
		textSeq = 0;
		x = rand() % 200;
		y = rand() % 400;
	}

	void draw(int strColor) {
		string text = texts.get(textId);
		DrawObtainsString2(x + 20, y + 10, 560, GetFontSize(), text.c_str(), strColor, font, GetColor(250, 250, 150));
	}

	void update() {
		// �e�L�X�g�̍����ւ�
		if (cnt > 200) {
			//������x�A�Ԃ������悤�ɐݒ�
			if (textId < texts.size && (rand() % 10000) / 10000.0 < pow(0.95, pow(2.0, textSeq))) {
				textId++;
				textSeq++;
			}
			else {
				textId = rand() % texts.size;
				textSeq = 0;
			}
			cnt = 0;
			x = rand() % 200;
			y = rand() % 400;
		}
		cnt++;
	}

	/*===========================*/
	//    �f�o�b�O���
	/*===========================*/
	void debugDump(int strColor) {
		//DrawFormatString(245, 25, strColor, "maxSize: %d", texts.maxSize);
		DrawFormatString(245, 45, strColor, "size: %d", texts.size);
		DrawFormatString(245, 65, strColor, "commX: %d", x);
		DrawFormatString(245, 85, strColor, "commY: %d", y);
		DrawFormatString(245, 105, strColor, "textId: %d", textId);
		DrawFormatString(245, 125, strColor, "textSeq: %d", textSeq);
		DrawFormatString(245, 145, strColor, "commCnt: %d", cnt);
	}
};


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