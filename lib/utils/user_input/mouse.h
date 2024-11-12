#pragma once

class Mouse {
private:
	int setX = 0;
	int setY = 0;		// �h���b�O���̎n�_
	int preX = 0;
	int preY = 0;		// ���O�̃J�[�\���ʒu
	unsigned int button[8];

public:
	int x;
	int y;
	int wheel;

	//�}�E�X��Ԃ̎擾
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

	//��`�̏�Ƀ}�E�X�J�[�\�������邩����
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
