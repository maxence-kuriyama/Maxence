#pragma once


struct Coordinate {
	int global_x;
	int global_y;
	int x;
	int y;
	int last_field;
};

// �������̎w����̗�����ێ�����N���X
// Game�I�u�W�F�N�g�̃����o�Ƃ��Ă̎g�p��z��
class History {
public:
	Coordinate history[2];
	int cancelCnt = 0; // ����O�܂ł̏���ێ����Ă��邩 (<= 2)

	void initialize() {
		for (int i = 0; i < 2; i++) {
			clear(history[i]);
		}
		cancelCnt = 0;
	}

	// �����ɒǉ�
	void add(int global_x, int global_y, int local_x, int local_y, int nextField) {
		history[1] = history[0];
		history[0] = { global_x, global_y, local_x, local_y, nextField };
		if (cancelCnt < 2) cancelCnt++;
	}

	// �L�����Z���\���ǂ�������
	bool canCancel() {
		return (cancelCnt > 0);
	}

	// �L�����Z���ł�����1�A�ł��Ȃ�������0��Ԃ�
	int goBack() {
		if (canCancel()) {
			history[0] = history[1];
			clear(history[1]);
			cancelCnt--;
			return 1;
		}
		return 0;
	}

	Coordinate last() {
		return history[0];
	}

private:

	void clear(Coordinate& c) {
		c = { 0, 0, 0, 0, -1 };
	}
};