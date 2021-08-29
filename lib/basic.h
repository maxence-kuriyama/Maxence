#pragma once


//======================================
//   マウス関連
//======================================

class Mouse {
public:
	int x;
	int y;
	int setX = 0;
	int setY = 0;		// ドラッグ時の始点
	int preX = 0;
	int preY = 0;		// 直前のカーソル位置
	int wheel;
	unsigned int button[8];

	//マウス状態の取得
	int update() {
		preX = x;
		preY = y;
		if (GetMousePoint(&x, &y) == -1) {
			return -1;
		}
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
		setX = x;
		setY = y;
	}

	bool click() {
		return button[0] == 1;
	}

	bool onClick() {
		return button[0] > 1;
	}
	
	bool clickRight() {
		return button[1] == 1;
	}

	//矩形の上にマウスカーソルがいるか判定
	bool onButton(int LeftUpx, int LeftUpy, int RightDownx, int RightDowny) {
		return (x > LeftUpx && x < RightDownx && y > LeftUpy && y < RightDowny);
	}

	bool isUsed() {
		return (x != preX || y != preY);
	}

	int distDragX() {
		return x - setX;
	}

	int distDragY() {
		return y - setY;
	}
};



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

