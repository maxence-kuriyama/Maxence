#pragma once

class MrK {
public:
	int x;
	int y;
	int img;
	int visible = 1;

	void set(int posX, int posY, const char* imgName, int visibility = 1) {
		x = posX;
		y = posY;
		img = LoadGraph(imgName);
		visible = visibility;
	}

	void set(int posX, int posY) {
		x = posX;
		y = posY;
	}

	void hide() {
		visible = 0;
	}

	void exhibit() {
		visible = 1;
	}

	void draw(int epX = 0, int epY = 0) {
		if (visible) {
			DrawGraph(x + epX, y + epY, img, TRUE);
		}
	}
};
