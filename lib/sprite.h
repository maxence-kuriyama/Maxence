#pragma once

class MrK {
public:
	int x;
	int y;
	int img[16];		// 0-3: Front, 4-7: Right, 8-11: Back, 12-15: Left
	int walkCnt = 0;
	int visible = 1;
	int walking = 0;
	int loopSpeed = 15;

	~MrK() {
		for (int i = 0; i < 16; ++i) {
			DeleteGraph(img[i]);
		}
	}

	void set(int posX, int posY, const char* imgName, int visibility = 1) {
		x = posX;
		y = posY;
		img[0] = LoadGraph(imgName);
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

	void walk() {
		walking = 1;
	}

	void stop() {
		walking = 0;
	}

	void setLoopSpeed(int speed) {
		loopSpeed = speed;
	}

	void draw(int epX = 0, int epY = 0) {
		if (visible) {
			if (walking) {
				walkCnt = (walkCnt + 1) % (4 * loopSpeed);
				int idx = walkCnt / 10;
				DrawGraph(x + epX, y + epY, img[idx], TRUE);
			}
			else {
				DrawGraph(x + epX, y + epY, img[0], TRUE);
			}
		}
	}
};
