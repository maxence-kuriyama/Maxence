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
	int cancelCnt = 0; // 何手前までの情報を保持しているか (<= 2)

	void initialize() {
		for (int i = 0; i < 3; i++) {
			last[i] = 0;
			seclast[i] = 0;
		}
		cancelCnt = 0;
	}

	// 履歴に追加
	void add(int global_x, int global_y, int local_x, int local_y, int nextField) {
		for (int m = 0; m < 5; m++) {
			seclast[m] = last[m];
		}
		last[0] = global_x; last[1] = global_y;
		last[2] = local_x; last[3] = local_y;
		last[4] = nextField;
		if (cancelCnt < 2) cancelCnt++;
	}

	// キャンセル可能かどうか判定
	bool canCancel() {
		return (cancelCnt > 0);
	}

	// キャンセル可能なら1、そうでなければ0を返す
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