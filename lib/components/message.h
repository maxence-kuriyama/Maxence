#pragma once

#include "lib/components/sprite.h"
#include "lib/components/draw_round_box.h"

const double NEXT_ICON_BLINK_SPEED(0.6);

const int MESSAGE_WHO_DESC(0);
const int MESSAGE_WHO_YELLOW(1);
const int MESSAGE_WHO_BLUE(2);
const int MESSAGE_WHO_RED(3);
const int MESSAGE_WHO_GREEN(4);
const int MESSAGE_WHO_DEER(5);
const int MESSAGE_WHO_PLAYER(-1);

const int MESSAGE_INT_Y(390); // �S�̂̎n�_y���W

void DrawMessage(int cnt, int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor);
int MultiByteLength(const char* String);


// �V�i���I���b�Z�[�W�Ǘ��p�N���X
// Scenario�I�u�W�F�N�g�̃����o�Ƃ��Ďg�p
// �ꊇ�Ǘ��ł͂Ȃ��e�L�����N�^�[�I�u�W�F�N�g�ɕR�Â���ׂ����H
class Message {
private:
	static int fontHandle;
	Sprite sprite[6];
	string text;
	int strColor = GetColor(255, 255, 255);
	int boxColor = GetColor(250, 250, 150);
	int nextIcon = LoadGraph("graph/next.png");
	int iconCnt = 0;
	bool iconVisible = false;
	bool existsNext = false;
	int iniX = 110; // ���b�Z�[�W�n�_x���W
	int endX = 600; // ���b�Z�[�W�I�[x���W

public:
	int who;
	int cnt = 0;					// �����`��J�E���^ (cnt * cntMlt <= textLen)
	float cntPerFrame = 15.0 / FPS;	// FPS�Ɉˑ����Ȃ��悤�ɂ���{��
	int textLen = 0;				// �e�L�X�g��
	bool isShown = false;

	Message() {
		if (Message::fontHandle == NULL) {
			Message::fontHandle = CreateFontToHandle("HG���ȏ���", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
		}
	}

	~Message() {
		DeleteGraph(nextIcon);
	}

	void initialize() {
		int x = 30;
		int y = MESSAGE_INT_Y;
		sprite[0].set(x, y);
		sprite[1].set(x, y, "graph/sprite11.png");
		sprite[2].set(x, y, "graph/sprite12.png");
		sprite[3].set(x, y, "graph/sprite13.png");
		sprite[4].set(x, y, "graph/sprite14.png");
		sprite[5].set(x, y, "graph/sprite15.png");
		setEmpty();
	}

	void initialize(int srcIniX, int srcEndX) {
		iniX = srcIniX;
		endX = srcEndX;
		initialize();
	}

	void set(string text0, int who0, bool next = true) {
		text = text0;
		who = who0;
		cnt = 0;
		textLen = MultiByteLength(text0.c_str());
		existsNext = next;
		isShown = true;
	}

	void setWithoutNext(string text0, int who0) {
		set(text0, who0, false);
		isShown = true;
	}

	void setEmpty(int who0 = MESSAGE_WHO_DESC) {
		setWithoutNext("", who0);
		isShown = false;
	}

	int skip() {
		if (isReading()) {
			cnt = ceil(textLen / cntPerFrame);
			return 0;
		}
		isShown = false;
		return 1;
	}

	int isReading() {
		return (cnt * cntPerFrame < textLen);
	}

	int draw(int strCol = 0) {
		if (strCol == 0) {
			strCol = strColor;
		}

		int charCnt = cnt * cntPerFrame;
		if (charCnt < textLen) {
			cnt++;
		}
		DrawRoundBox(15, MESSAGE_INT_Y - 2, 10, 609, 81, boxColor);
		DrawMessage(charCnt, iniX, MESSAGE_INT_Y + 2, endX, GetFontSize(), text.c_str(), strColor, Message::fontHandle, boxColor);
		if (existsNext) {
			if (isReading()) {
				drawBlinkIcon();
			}
			else {
				drawIcon();
			}
		}

		sprite[who].draw();

		if (charCnt < textLen) {
			return 1;
		}

		return 0;
	}

private:

	void drawIcon(bool noBlink = false) {
		iconCnt++;
		if (iconCnt > NEXT_ICON_BLINK_SPEED * FPS) {
			iconVisible = (iconVisible ? false : true);
			iconCnt = 0;
		}
		if (noBlink || iconVisible) {
			DrawExtendGraph(590, 448, 602, 460, nextIcon, TRUE);
		}
	}

	void drawBlinkIcon() {
		return drawIcon(true);
	}
};

int Message::fontHandle = NULL;


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
	double rate = 1.45;	// �������Ƃ̕`�敝�B�f�t�H���g�̃t�H���g�T�C�Y��20�ɑ΂��A���̃t�H���g�T�C�Y24�i1.2�{�j�{�]�T����������
	double marginRateY = 1.2; // �c�͂��]�T�����������������h���������̂ŁA���̕��̔{��
	int Counter = cnt;
	bool breakLine = false;

	// ������S�̂̃o�C�g�����擾
	StrLen = strlen(String);

	DrawX = x;
	DrawY = y;
	for (i = 0; i < StrLen; ) {
		// �w�肳�ꂽ�������ŕ`�����߂�
		if (Counter <= 0) break;
		breakLine = false;

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
			if (String[i] == '\n') {
				breakLine = true;
			}
			else {
				TempStr[0] = String[i];
				TempStr[1] = '\0';
			}
			CharLen = 1;
			Counter--;
		}
		// �P�����̕`�敝���擾
		DrawWidth = GetDrawStringWidth(String + i, CharLen) * rate;
		// �`��͈͂���͂ݏo��ꍇ�͉��s
		if (breakLine || DrawX + DrawWidth > RightX) {
			DrawX = x;
			DrawY += AddY * rate * marginRateY;
		}
		if (!breakLine) {
			// �P�����`��
			DrawStringToHandle(DrawX, DrawY, TempStr, StrColor, FontHandle);
			// �`����W�����炷
			DrawX += DrawWidth;
		}
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
