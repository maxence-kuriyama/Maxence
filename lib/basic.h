#pragma once

// 基礎的なUIに関するクラス

//======================================
//   カメラ関連
//======================================
class Camera {
public:
	VECTOR pos;
	VECTOR origin = VGet(320.0, 240.0, 0.0);

	void initialize() {
		pos.x = 320.0;
		pos.y = 240.0;
		pos.z = -300.0;
	}

	void move(double dx, double dy) {
		VECTOR temp1 = VSub(pos, origin);
		VECTOR temp2 = VNorm(temp1);
		pos.y += 2.0 * dy / cosh((double)temp2.y);
		pos.x = origin.x + temp1.x * cos(dx / 100.0) + temp1.z * sin(dx / 100.0);
		pos.z = origin.z - temp1.x * sin(dx / 100.0) + temp1.z * cos(dx / 100.0);
		set();
	}

	void set() {
		SetCameraPositionAndTarget_UpVecY(pos, origin);
	}

	VECTOR direction() {
		return VNorm(VSub(pos, origin));
	}

	void zoom(int wheel) {
		VECTOR temp = direction();
		pos.x -= temp.x * wheel * 5.0;
		pos.z -= temp.z * wheel * 5.0;
		set();
	}
};



//======================================
//   コメント関連
//======================================
void DrawRoundBox(int x, int y, int r, int w, int h, int Color) {
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