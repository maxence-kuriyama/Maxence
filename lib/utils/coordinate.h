#pragma once

class Coordinate {
public:
	int global_x = -1;
	int global_y = -1;
	int x = -1;
	int y = -1;
	int last_field = -1;

	void operator = (const int index) {
		setFromIndex(index);
	}

	Coordinate operator = (const Coordinate &src) {
		global_x = src.global_x;
		global_y = src.global_y;
		x = src.x;
		y = src.y;
		last_field = src.last_field;
		return src;
	}

	void setFromIndex(const int index) {
		global_x = (index / 27) % 3;
		global_y = (index / 9) % 3;
		x = (index / 3) % 3;
		y = index % 3;
		last_field = -1;
	}

	void clear() {
		global_x = 0;
		global_y = 0;
		x = 0;
		y = 0;
		last_field = -1;
	}
};
