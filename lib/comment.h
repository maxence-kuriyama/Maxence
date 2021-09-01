#pragma once

void DrawObtainsString2(int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor);


class Texts {
public:
	string textFileName = "data/sayings.txt";
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


class Comment {
public:
	Texts texts;
	int x = rnd() % 200;
	int y = rnd() % 400;
	int textId = 0;		// �e�L�X�g�̃C���f�b�N�X
	int textSeq = 0;	// �e�L�X�g�������A���������̃J�E���^
	int cnt = 0;		// �e�L�X�g�����ւ��p�J�E���^
	int font;

	void initialize() {
		texts.initialize();
		cnt = 0;
		textSeq = 0;
		x = rnd() % 200;
		y = rnd() % 400;
		//font = Font0;
	}

	void draw(int strColor) {
		DrawObtainsString2(x + 20, y + 10, 560, GetFontSize(), texts.text[textId].c_str(), strColor, font, GetColor(250, 250, 150));
	}

	void update() {
		// �e�L�X�g�̍����ւ�
		if (cnt > 200) {
			//������x�A�Ԃ������悤�ɐݒ�
			if (textId < texts.size && (rnd() % 10000) / 10000.0 < pow(0.95, pow(2.0, textSeq))) {
				textId++;
				textSeq++;
			}
			else {
				textId = rnd() % texts.size;
				textSeq = 0;
			}
			cnt = 0;
			x = rnd() % 200;
			y = rnd() % 400;
		}
		cnt++;
	}
};


void DrawRoundBox(int x, int y, int r, int w, int h, int Color) {
	unsigned int Black = GetColor(0, 0, 0);
	/* �l���̉~ */
	DrawCircle(x, y, r, Color, TRUE);
	DrawCircle(x + w, y, r, Color, TRUE);
	DrawCircle(x, y + h, r, Color, TRUE);
	DrawCircle(x + w, y + h, r, Color, TRUE);
	DrawCircle(x, y, r, Black, FALSE);
	DrawCircle(x + w, y, r, Black, FALSE);
	DrawCircle(x, y + h, r, Black, FALSE);
	DrawCircle(x + w, y + h, r, Black, FALSE);

	/* �~�ɔ��Ȃ������` */
	DrawBox(x - r, y, x + w + r + 1, y + h, Color, TRUE);
	DrawBox(x, y - r, x + w, y + h + r + 1, Color, TRUE);
	DrawLine(x, y - r, x + w + 1, y - r, Black, FALSE);
	DrawLine(x, y + h + r + 1, x + w + 1, y + h + r + 1, Black, FALSE);
	DrawLine(x - r, y, x - r, y + h + 1, Black, FALSE);
	DrawLine(x + w + r + 1, y, x + w + r + 1, y + h + 1, Black, FALSE);
}

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