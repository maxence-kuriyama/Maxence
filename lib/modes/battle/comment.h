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


// テキストを保持するオブジェクト
// Commentクラスのメンバとしての使用を想定
class Texts {
public:
	nlohmann::json data;
	int size = 0;
	string types = "";

	// テキストファイルの読み込み
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


// 試合中に流れるコメントのクラス
// Gameオブジェクトのメンバとしての使用を想定
class Comment {
private:
	static int fontHandle;

	double rn() { return (rand() % 10000) / 10000.0; }

public:
	Texts texts;
	int x = rand() % 200;
	int y = rand() % 400;
	int textNum = 0;	// テキストの通し番号
	int textSeq = 0;	// テキストがいくつ連続したかのカウンタ
	int cnt = 0;		// テキスト差し替え用カウンタ

	Comment() {
		if (Comment::fontHandle == NULL) {
			Comment::fontHandle = CreateFontToHandle("HG教科書体", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
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
		// テキストの差し替え
		if (cnt > REPLACE_TEXT_SECOND * FPS) {
			//ある程度連番が続くように設定
			if (textNum < texts.size - 1 && rn() < pow(0.90, pow(2.0, textSeq))) {
				if (playCnt > COMMENT_CHANGE_TYPE_LONG_THINKING_TIME && forceUpdate(COMMENT_CHANGE_TYPE_LONG_THINKING, 0.70)) {
					// 長考と判断 + 何もしない
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
	//    デバッグ情報
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
	double rate = 1.55;	// 文字ごとの描画幅。デフォルトのフォントサイズは20に対し、今のフォントサイズ28（1.4倍）＋余裕を持たせる
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

// Refer to https://sayahamitt.net/utf8なstring入れたらshiftjisなstring出てくる関数作った/
std::string UTF8toSjis(std::string srcUTF8) {
	//Unicodeへ変換後の文字列長を得る
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8からUnicodeへ変換
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

	//ShiftJISへ変換後の文字列長を得る
	int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//必要な分だけShiftJIS文字列のバッファを確保
	char* bufShiftJis = new char[lengthSJis];

	//UnicodeからShiftJISへ変換
	WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

	std::string strSJis(bufShiftJis);

	delete bufUnicode;
	delete bufShiftJis;

	return strSJis;
}