#pragma once

class Coordinate {
public:
	int global_x;
	int global_y;
	int x;
	int y;
	int last_field;

	void clear() {
		global_x = 0;
		global_y = 0;
		x = 0;
		y = 0;
		last_field = -1;
	}
};
