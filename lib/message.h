#pragma once

#include "lib/basic.h"
#include "lib/sprite.h"

void DrawMessage(int cnt, int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor);
int MultiByteLength(const char* String);

class Message {
private:
	MrK sprite[6];
	string text;
	int strColor = GetColor(255, 255, 255);
	int boxColor = GetColor(250, 250, 150);
	int font = CreateFontToHandle("HG���ȏ���", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);

public:
	int who;
	int cnt = 0;					// �����`��J�E���^ (cnt * cntMlt <= textLen)
	float cntPerFrame = 15.0 / FPS;	// FPS�Ɉˑ����Ȃ��悤�ɂ���{��
	int textLen = 0;				// �e�L�X�g��

	void initialize() {
		for (int i = 0; i < 6; ++i) {
			sprite[i].x = 30;
			sprite[i].y = 380;
		}
		sprite[0].img = 0;
		sprite[1].img = LoadGraph("graph/stripe11.png");
		sprite[2].img = LoadGraph("graph/stripe12.png");
		sprite[3].img = LoadGraph("graph/stripe13.png");
		sprite[4].img = LoadGraph("graph/stripe14.png");
		sprite[5].img = LoadGraph("graph/stripe15.png");
	}

	void set(string text0, int who0) {
		text = text0;
		who = who0;
		cnt = 0;
		textLen = MultiByteLength(text0.c_str());
	}

	int skip() {
		if (cnt * cntPerFrame < textLen) {
			cnt = ceil(textLen / cntPerFrame);
			return 0;
		}
		return 1;
	}

	int draw(int strCol = 0) {
		if (strCol == 0) {
			strCol = strColor;
		}

		int charCnt = cnt * cntPerFrame;
		if (charCnt < textLen) {
			cnt++;
		}
		DrawRoundBox(15, 380, 10, 609, 89, boxColor);
		DrawMessage(charCnt, 110, 390, 600, GetFontSize(), text.c_str(), strColor, font, boxColor);

		sprite[who].draw();

		if (charCnt < textLen) {
			return 1;
		}

		return 0;
	}
};


void DrawMessage(int cnt, int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor) {
	char TempStr[3];
	int StrLen;
	int i;
	int DrawX;
	int DrawY;
	int CharLen;
	int DrawWidth;
	int BoxWidth;
	int BoxHeight;
	double rate = 1.2;	//�f�t�H���g�̃t�H���g�T�C�Y20�ɑ΂��āA���̓t�H���g�T�C�Y24�ɂ��邽��1.2�{
	int Counter = cnt;

	// ������S�̂̃o�C�g�����擾
	StrLen = strlen(String);

	DrawX = x;
	DrawY = y;
	for (i = 0; i < StrLen; ) {
		// �w�肳�ꂽ�������ŕ`�����߂�
		if (Counter <= 0) break;

		if (_mbbtype((unsigned char)String[i], 0) == _MBC_LEAD) {
			// �S�p�����̏ꍇ
			TempStr[0] = String[i];
			TempStr[1] = String[i + 1];
			TempStr[2] = '\0';
			CharLen = 2;
			Counter--;
		}
		else {
			// ���p�����̏ꍇ
			TempStr[0] = String[i];
			TempStr[1] = '\0';
			CharLen = 1;
			Counter--;
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

int MultiByteLength(const char* String) {
	int StrLen;
	int i;
	int CharLen;
	int Counter = 0;

	// ������S�̂̃o�C�g�����擾
	StrLen = strlen(String);

	for (i = 0; i < StrLen; ) {
		if (_mbbtype((unsigned char)String[i], 0) == _MBC_LEAD) {
			CharLen = 2;
			Counter++;
		}
		else {
			CharLen = 1;
			Counter++;
		}
		i += CharLen;
	}
	return Counter;
}
