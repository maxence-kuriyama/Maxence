#pragma once

#include "lib/basic.h"
#include "lib/sprite.h"

#define NEXT_ICON_BLINK_SPEED	0.6

void DrawMessage(int cnt, int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor);
int MultiByteLength(const char* String);


// シナリオメッセージ管理用クラス
// Scenarioオブジェクトのメンバとして使用
// 一括管理ではなく各キャラクターオブジェクトに紐づけるべきか？
class Message {
private:
	MrK sprite[6];
	string text;
	int strColor = GetColor(255, 255, 255);
	int boxColor = GetColor(250, 250, 150);
	int font = CreateFontToHandle("HG教科書体", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
	int nextIcon = LoadGraph("graph/next.png");
	int iconCnt = 0;
	bool iconVisible = false;
	bool existsNext = false;

public:
	int who;
	int cnt = 0;					// 文字描画カウンタ (cnt * cntMlt <= textLen)
	float cntPerFrame = 15.0 / FPS;	// FPSに依存しないようにする倍率
	int textLen = 0;				// テキスト長

	~Message() {
		DeleteGraph(nextIcon);
	}

	void initialize() {
		sprite[0].set(30, 380);
		sprite[1].set(30, 380, "graph/sprite11.png");
		sprite[2].set(30, 380, "graph/sprite12.png");
		sprite[3].set(30, 380, "graph/sprite13.png");
		sprite[4].set(30, 380, "graph/sprite14.png");
		sprite[5].set(30, 380, "graph/sprite15.png");
	}

	void set(string text0, int who0, bool next = true) {
		text = text0;
		who = who0;
		cnt = 0;
		textLen = MultiByteLength(text0.c_str());
		existsNext = next;
	}

	int skip() {
		if (isReading()) {
			cnt = ceil(textLen / cntPerFrame);
			return 0;
		}
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
		DrawRoundBox(15, 380, 10, 609, 89, boxColor);
		DrawMessage(charCnt, 110, 390, 600, GetFontSize(), text.c_str(), strColor, font, boxColor);
		if (existsNext) {
			if (isReading()) {
				drawIcon(true);
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
	double rate = 1.45;	// 文字ごとの描画幅。デフォルトのフォントサイズは20に対し、今のフォントサイズ24（1.2倍）＋余裕を持たせる
	int Counter = cnt;

	// 文字列全体のバイト数を取得
	StrLen = strlen(String);

	DrawX = x;
	DrawY = y;
	for (i = 0; i < StrLen; ) {
		// 指定された文字数で描画をやめる
		if (Counter <= 0) break;

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
			TempStr[0] = String[i];
			TempStr[1] = '\0';
			CharLen = 1;
			Counter--;
		}
		// １文字の描画幅を取得
		DrawWidth = GetDrawStringWidth(String + i, CharLen) * rate;
		// 描画範囲からはみ出る場合は改行
		if (DrawX + DrawWidth > RightX)
		{
			DrawX = x;
			DrawY += AddY * rate;
		}
		// １文字描画
		DrawStringToHandle(DrawX, DrawY, TempStr, StrColor, FontHandle);
		// 描画座標をずらす
		DrawX += DrawWidth;
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
