#pragma once


struct Coordinate {
	int global_x;
	int global_y;
	int x;
	int y;
	int last_field;
};

// 試合中の指し手の履歴を保持するクラス
// Gameオブジェクトのメンバとしての使用を想定
class History {
public:
	Coordinate history[2];
	int cancelCnt = 0; // 何手前までの情報を保持しているか (<= 2)

	void initialize() {
		for (int i = 0; i < 2; i++) {
			clear(history[i]);
		}
		cancelCnt = 0;
	}

	// 履歴に追加
	void add(int global_x, int global_y, int local_x, int local_y, int nextField) {
		history[1] = history[0];
		history[0] = { global_x, global_y, local_x, local_y, nextField };
		if (cancelCnt < 2) cancelCnt++;
	}

	// キャンセル可能かどうか判定
	bool canCancel() {
		return (cancelCnt > 0);
	}

	// キャンセルできたら1、できなかったら0を返す
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