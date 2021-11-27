#pragma once

#include "lib/basic.h"

void DrawObtainsString2(int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor);


// �e�L�X�g�̓ǂݍ��݁A�ێ��I�u�W�F�N�g
// Comment�N���X�̃����o�Ƃ��Ă̎g�p��z��
class Texts {
private:
	string textFileName = "data/sayings.txt";

public:
	string text[450];
	int maxSize = 450;
	int size = 0;

	// �e�L�X�g�t�@�C���̓ǂݍ���
	void initialize() {
		ifstream read(textFileName);
		string str;
		int length = 30;
		double tmp = 0.0;
		int k = 0;

		if (!read) {
			cout << "�e�L�X�g�t�@�C���̓ǂݍ��ݎ��s" << endl;
		}
		else {
			while (k < maxSize) {
				getline(read, str);
				text[k] = str;
				k++;
			}
		}

		size = k;
		read.close();
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

	void initialize() {
		texts.initialize();
		cnt = 0;
		textSeq = 0;
		x = rand() % 200;
		y = rand() % 400;
		//font = Font0;
	}

	void draw(int strColor) {
		DrawObtainsString2(x + 20, y + 10, 560, GetFontSize(), texts.text[textId].c_str(), strColor, font, GetColor(250, 250, 150));
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
	double rate = 1.4;	//�f�t�H���g�̃t�H���g�T�C�Y20�ɑ΂��āA���̓t�H���g�T�C�Y28�ɂ��邽��1.4�{
	//�w�肵���t�H���g�n���h���̃t�H���g�T�C�Y�𔽉f������ɂ͂ǂ�����΂悢�̂�
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