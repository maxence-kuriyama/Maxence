#pragma once

#include "lib/basic.h"

void DrawObtainsString2(int x, int y, int RightX, int AddY, const char* String, int StrColor, int FontHandle, int BoxColor);


// テキストの読み込み、保持オブジェクト
// Commentクラスのメンバとしての使用を想定
class Texts {
private:
	string textFileName = "data/sayings.txt";

public:
	string text[450];
	int maxSize = 450;
	int size = 0;

	// テキストファイルの読み込み
	void initialize() {
		ifstream read(textFileName);
		string str;
		int length = 30;
		double tmp = 0.0;
		int k = 0;

		if (!read) {
			cout << "テキストファイルの読み込み失敗" << endl;
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


// 試合中に流れるコメントのクラス
// Gameオブジェクトのメンバとしての使用を想定
class Comment {
private:
	int font = CreateFontToHandle("HG教科書体", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);

public:
	Texts texts;
	int x = rand() % 200;
	int y = rand() % 400;
	int textId = 0;		// テキストのインデックス
	int textSeq = 0;	// テキストがいくつ連続したかのカウンタ
	int cnt = 0;		// テキスト差し替え用カウンタ

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
		// テキストの差し替え
		if (cnt > 200) {
			//ある程度連番が続くように設定
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
	double rate = 1.4;	//デフォルトのフォントサイズ20に対して、今はフォントサイズ28にするため1.4倍
	//指定したフォントハンドルのフォントサイズを反映させるにはどうすればよいのか
	int r = 5;	//四角形の丸角の半径

	// 文字列全体のバイト数を取得
	StrLen = strlen(String);

	//二度手間ではあるが、一度吹き出しの大きさを測ってから、一文字ずつ描画する
	DrawX = x;
	BoxWidth = 0;
	BoxHeight = AddY * rate;
	for (i = 0; i < StrLen; ) {
		if (_mbbtype((unsigned char)String[i], 0) == _MBC_LEAD) {
			// 全角文字の場合
			TempStr[0] = String[i];
			TempStr[1] = String[i + 1];
			TempStr[2] = '\0';
			CharLen = 2;
		}
		else {
			// 半角文字の場合
			TempStr[0] = String[i];
			TempStr[1] = '\0';
			CharLen = 1;
		}
		// １文字の描画幅を取得
		DrawWidth = GetDrawStringWidth(String + i, CharLen) * rate;
		// 描画範囲からはみ出る場合は改行
		if (DrawX + DrawWidth > RightX) {
			DrawX = x;
			BoxWidth = RightX - x;
			BoxHeight += AddY * rate;
		}
		// 描画座標をずらす
		DrawX += DrawWidth;
		i += CharLen;
	}
	if (DrawX - x + 5 > BoxWidth) { BoxWidth = DrawX - x + 5; }
	DrawRoundBox(x, y, r, BoxWidth, BoxHeight, BoxColor);

	DrawX = x;
	DrawY = y;
	for (i = 0; i < StrLen; ) {
		if (_mbbtype((unsigned char)String[i], 0) == _MBC_LEAD) {
			// 全角文字の場合
			TempStr[0] = String[i];
			TempStr[1] = String[i + 1];
			TempStr[2] = '\0';
			CharLen = 2;
		}
		else {
			// 半角文字の場合
			TempStr[0] = String[i];
			TempStr[1] = '\0';
			CharLen = 1;
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