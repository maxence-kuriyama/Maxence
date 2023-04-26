#pragma once

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
