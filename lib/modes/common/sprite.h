#pragma once

#include <string>

struct Saying {
	char key[4];
	int who;
	char say[100];
};

// キャラクターオブジェクトのクラス
// グラフィックの表示を主に行っているが、メッセージの保持も行うべきか？
// ScenarioやEndingなどでの使用を想定
class MrK {
public:
	int x;
	int y;
	int visible = 1;
	int walkCnt = 0;
	int img[16];		// 0-3: Front, 4-7: Right, 8-11: Back, 12-15: Left
	int spImg[4];		// Special
	int trigger = MRK_TRIGGER_NONE;	// 次のシナリオに進むトリガー

private:
	int direction = 0;	// 0: Front, 1: Right, 2; Back, 3; Left
	int special = -1;	// spImg用のインデックス
	int walking = 0;	// 0: Stop, 1: Walking
	int walkSpeed = 2;
	int loopSpeed = 15;
	bool talked = 0;	// 会話した回数
	struct Saying *sayings;
	int sayCnt = 0;
	double expandRate = 0.0;
	bool expandFlg = false;
	int orgSizeX = 0;
	int orgSizeY = 0;

public:

	~MrK() {
		for (int i = 0; i < 16; ++i) {
			DeleteGraph(img[i]);
		}
		for (int i = 0; i < 4; ++i) {
			DeleteGraph(spImg[i]);
		}
	}

	void set(int posX, int posY, const char* imgName, double rate, int sizeX, int sizeY, int visibility = 1) {
		set(posX, posY, imgName, visibility);
		setExpand(rate, sizeX, sizeY);
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

	void setSerialImages(int size, const char* baseName, int start = 0) {
		for (int i = 0; i < size; ++i) {
			string name;
			name = baseName + to_string(i + start) + ".png";
			img[i] = LoadGraph(name.c_str());
		}
	}

	void setSayings(struct Saying src[]) {
		sayings = src;
	}

	void setExpand(double rate, int sizeX, int sizeY) {
		expandRate = rate;
		orgSizeX = sizeX;
		orgSizeY = sizeY;
		expandFlg = true;
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

	void setLoopSpeed(int speed) {
		loopSpeed = speed;
	}

	void move() {
		if (direction == 0) {
			y += walkSpeed;
		}
		else if (direction == 1) {
			x += walkSpeed;
		}
		else if (direction == 2) {
			y -= walkSpeed;
		}
		else if (direction == 3) {
			x -= walkSpeed;
		}
		walk();
	}

	void stop() {
		walking = 0;
	}

	void setSpecialImg(int idx) {
		special = idx;
	}

	void turn(int dir) {
		direction = dir;
	}

	void front() {
		turn(0);
	}

	void right() {
		turn(1);
	}

	void back() {
		turn(2);
	}

	void left() {
		turn(3);
	}

	void playSE(string fileName) {
		playSE(fileName.c_str());
	}

	void playSE(const char fileName[]) {
		PlaySoundFile(fileName, DX_PLAYTYPE_BACK);
	}

	void draw(int epX = 0, int epY = 0) {
		if (visible) {
			int image = 0;
			if (walking) {
				walkCnt = (walkCnt + 1) % (4 * loopSpeed);
				int idx = direction * 4 + (walkCnt / loopSpeed);
				image = img[idx];
			}
			else if (direction != 0) {
				walkCnt = (walkCnt + 1) % (4 * loopSpeed);
				int idx = direction * 4;
				image = img[idx];
			}
			else if (special >= 0) {
				image = spImg[special];
			}
			else {
				image = img[0];
			}
			if (!expandFlg) {
				DrawGraph(x + epX, y + epY, image, TRUE);
			}
			else {
				int dx = orgSizeX * (1.0 + (y - 240.0) * expandRate);
				int dy = orgSizeY * (1.0 + (y - 240.0) * expandRate);
				DrawExtendGraph(x + epX, y + epY, x + epX + dx, y + epY + dy, image, TRUE);
			}
		}
		special = -1;
	}

	bool isTriggered() {
		return (trigger == MRK_TRIGGER_FIRED);
	}

	void setTrigger(string how) {
		resetTrigger();
		if (how == "talk") {
			trigger = MRK_TRIGGER_TALK;
		}
		else if (how == "fired") {
			trigger = MRK_TRIGGER_FIRED;
		}
	}

	void resetTrigger() {
		trigger = MRK_TRIGGER_NONE;
		talked = 0;
	}

	Saying talk(const char key[]) {
		talked++;
		if (trigger == MRK_TRIGGER_TALK) {
			trigger = MRK_TRIGGER_FIRED;
		}
		int cnt = 0;
		int n = 0;
		while (true) {
			Saying say = sayings[n];
			if (strcmp(sayings[n].say, "") == 0 || sayings[n].who == -1) {
				return sayings[n];
			}
			if (strcmp(sayings[n].key, key) == 0) {
				if (cnt >= sayCnt) {
					return sayings[n];
				}
				else {
					cnt++;
				}
			}
			n++;
		}
	}

	void talkNext() {
		sayCnt++;
	}

	void talkReset() {
		sayCnt = 0;
	}

};
