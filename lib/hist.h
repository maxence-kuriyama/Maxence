#pragma once

class History {
public:
	int last[5];
	int seclast[5];
	/* ------------------------------- */
	// (last[0], last[1]) : global coord
	// (last[2], last[3]): local coord
	// last[4]: next global coord
	/* ------------------------------- */
	int cancelCnt = 0; // ����O�܂ł̏���ێ����Ă��邩 (<= 2)

	void initialize() {
		for (int i = 0; i < 3; i++) {
			last[i] = 0;
			seclast[i] = 0;
		}
		cancelCnt = 0;
	}

	// �����ɒǉ�
	void add(int global_x, int global_y, int local_x, int local_y, int nextField) {
		for (int m = 0; m < 5; m++) {
			seclast[m] = last[m];
		}
		last[0] = global_x; last[1] = global_y;
		last[2] = local_x; last[3] = local_y;
		last[4] = nextField;
		if (cancelCnt < 2) cancelCnt++;
	}

	// �L�����Z���\���ǂ�������
	bool canCancel() {
		return (cancelCnt > 0);
	}

	// �L�����Z���\�Ȃ�1�A�����łȂ����0��Ԃ�
	int goBack() {
		if (canCancel()) {
			for (int i = 0; i < 5; i++) {
				last[i] = seclast[i];
				seclast[i] = 0;
			}
			cancelCnt--;
			return 1;
		}
		return 0;
	}
};