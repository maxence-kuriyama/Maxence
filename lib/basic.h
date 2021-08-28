#pragma once


//======================================
//   マウス関連
//======================================

class Mouse {
public:
	int x;
	int y;
	int setx = 0;
	int sety = 0;
	int wheel;
	unsigned int button[8];

	//マウス状態の取得
	int update() {
		if (GetMousePoint(&x, &y) == -1) return -1;
		int MouseInput = GetMouseInput();
		for (int i = 0; i < 8; i++) {
			if (MouseInput & (1 << i)) {
				button[i]++;
			}
			else {
				button[i] = 0;
			}
		}
		wheel = GetMouseWheelRotVol();
		return 0;
	}

	void set() {
		setx = x;
		sety = y;
	}

	//矩形の上にマウスカーソルがいるか判定
	int onButton(int LeftUpx, int LeftUpy, int RightDownx, int RightDowny) {
		return x > LeftUpx && x < RightDownx && y > LeftUpy && y < RightDowny;
	}
};



//======================================
//   カメラ関連
//======================================


class Camera {
public:
	VECTOR pos;

	void initialize() {
		pos.x = 320.0;
		pos.y = 240.0;
		pos.z = -300.0;
	}

	void move(double dx, double dy, VECTOR Origin) {
		VECTOR tmp1 = VSub(pos, Origin);
		VECTOR tmp2 = VNorm(tmp1);
		pos.y += 2.0 * dy / cosh((double)tmp2.y);
		pos.x = Origin.x + tmp1.x * cos(dx / 100.0) + tmp1.z * sin(dx / 100.0);
		pos.z = Origin.z - tmp1.x * sin(dx / 100.0) + tmp1.z * cos(dx / 100.0);
	}
};



//======================================
//   コメント描画関連
//======================================

void DrawRoundBox(int x, int y, int r, int w, int h, int Color)
{
	unsigned int Black = GetColor(0, 0, 0);
	/* 四隅の円 */
	DrawCircle(x, y, r, Color, TRUE);
	DrawCircle(x + w, y, r, Color, TRUE);
	DrawCircle(x, y + h, r, Color, TRUE);
	DrawCircle(x + w, y + h, r, Color, TRUE);
	DrawCircle(x, y, r, Black, FALSE);
	DrawCircle(x + w, y, r, Black, FALSE);
	DrawCircle(x, y + h, r, Black, FALSE);
	DrawCircle(x + w, y + h, r, Black, FALSE);

	/* 円に被らない長方形 */
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
	double rate = 1.2;	//デフォルトのフォントサイズ20に対して、今はフォントサイズ24にするため1.2倍
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