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

const int MESSAGE_INT_Y(390); // 全体の始点y座標

void DrawMessage(int cnt, int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor);
int MultiByteLength(const char* String);


// シナリオメッセージ管理用クラス
// Scenarioオブジェクトのメンバとして使用
// 一括管理ではなく各キャラクターオブジェクトに紐づけるべきか？
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
	int iniX = 110; // メッセージ始点x座標
	int endX = 600; // メッセージ終端x座標

public:
	int who;
	int cnt = 0;					// 文字描画カウンタ (cnt * cntMlt <= textLen)
	float cntPerFrame = 15.0 / FPS;	// FPSに依存しないようにする倍率
	int textLen = 0;				// テキスト長
	bool isShown = false;

	Message() {
		if (Message::fontHandle == NULL) {
			Message::fontHandle = CreateFontToHandle("HG教科書体", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
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
	double rate = 1.45;	// 文字ごとの描画幅。デフォルトのフォントサイズは20に対し、今のフォントサイズ24（1.2倍）＋余裕を持たせる
	double marginRateY = 1.2; // 縦はより余裕を持たせた方が見栄えがいいので、その分の倍率
	int Counter = cnt;
	bool breakLine = false;

	// 文字列全体のバイト数を取得
	StrLen = strlen(String);

	DrawX = x;
	DrawY = y;
	for (i = 0; i < StrLen; ) {
		// 指定された文字数で描画をやめる
		if (Counter <= 0) break;
		breakLine = false;

		if (_mbbtype((unsigned char)String[i], 0) == _MBC_LEAD) {
			// 全角文字の場合
			TempStr[0] = String[i];
			TempStr[1] = String[i + 1];
			TempStr[2] = '\0';
			CharLen = 2;
			Counter--;
		}
		else {
			// 半角文字の場合
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
		// １文字の描画幅を取得
		DrawWidth = GetDrawStringWidth(String + i, CharLen) * rate;
		// 描画範囲からはみ出る場合は改行
		if (breakLine || DrawX + DrawWidth > RightX) {
			DrawX = x;
			DrawY += AddY * rate * marginRateY;
		}
		if (!breakLine) {
			// １文字描画
			DrawStringToHandle(DrawX, DrawY, TempStr, StrColor, FontHandle);
			// 描画座標をずらす
			DrawX += DrawWidth;
		}
		// 描画する文字を進める
		i += CharLen;
	}
}

int MultiByteLength(const char* String) {
	int StrLen;
	int i;
	int CharLen;
	int Counter = 0;

	// 文字列全体のバイト数を取得
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
