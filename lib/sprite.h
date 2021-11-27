#pragma once


// キャラクターオブジェクトのクラス
// グラフィックの表示を主に行っているが、メッセージの保持も行うべきか？
// ScenarioやEndingなどでの使用を想定
class MrK {
public:
	int x;
	int y;
	int img[16];		// 0-3: Front, 4-7: Right, 8-11: Back, 12-15: Left
	int spImg[4];		// Special
	int walkCnt = 0;
	int visible = 1;
	int walking = 0;
	int special = -1;	// spImg用のインデックス
	int loopSpeed = 15;

	~MrK() {
		for (int i = 0; i < 20; ++i) {
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

	void walk(int speed = 0) {
		walking = 1;
		if (speed != 0) {
			setLoopSpeed(speed);
		}
	}

	void stop() {
		walking = 0;
	}

	void setLoopSpeed(int speed) {
		loopSpeed = speed;
	}

	void setSpecialImg(int idx) {
		special = idx;
	}

	void draw(int epX = 0, int epY = 0) {
		if (visible) {
			if (walking) {
				walkCnt = (walkCnt + 1) % (4 * loopSpeed);
				int idx = walkCnt / loopSpeed;
				DrawGraph(x + epX, y + epY, img[idx], TRUE);
			}
			else if (special >= 0) {
				DrawGraph(x + epX, y + epY, spImg[special], TRUE);
			}
			else {
				DrawGraph(x + epX, y + epY, img[0], TRUE);
			}
		}
		special = -1;
	}

};
