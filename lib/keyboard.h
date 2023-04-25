#pragma once

// キー入力に対する処理を行うクラス
// Gameオブジェクトのメンバとして使用（単体で使用すべきか？）
class Key {
public:
	int state[256];
	int waitConst = 3;		// ゲーム中のキー入力ウェイト
	int waitCnt = 3;		// キー入力ウェイトのカウンタ
	int use = 0;

	void setWait(int wait) {
		waitConst = wait;
	}

	void initWait() {
		waitCnt = waitConst;
	}

public:

	int update() {
		char temp[256];
		GetHitKeyStateAll(temp);
		for (int i = 0; i < 256; i++) {
			if (temp[i] != 0) {
				state[i]++;
			}
			else {
				state[i] = 0;
			}
		}
		if (waitCnt > 0) {
			waitCnt--;
		}
		return 0;
	}

	bool onReturn() {
		return (state[KEY_INPUT_RETURN] == 1 || state[KEY_INPUT_SPACE] == 1);
	}

	bool onBack() {
		return (state[KEY_INPUT_Z] == 1 || state[KEY_INPUT_BACK] == 1);
	}

	bool onAt() {
		return (state[KEY_INPUT_AT] == 1);
	}

	// ゲーム中のキー入力
	bool onCheck() {
		if (waitCnt <= 0 && (state[KEY_INPUT_RETURN] == 1 || state[KEY_INPUT_SPACE] == 1)) {
			initWait();
			use++;
			return true;
		}
		return false;
	}

	bool onUp() {
		if (waitCnt <= 0 && (state[KEY_INPUT_UP]== 1 || state[KEY_INPUT_W] == 1)) {
			initWait();
			use++;
			return true;
		}
		return false;
	}
	bool onGoingUp() {
		return (state[KEY_INPUT_UP] > 0 || state[KEY_INPUT_W] > 0);
	}

	bool onDown() {
		if (waitCnt <= 0 && (state[KEY_INPUT_DOWN] == 1 || state[KEY_INPUT_S] == 1)) {
			initWait();
			use++;
			return true;
		}
		return false;
	}
	bool onGoingDown() {
		return (state[KEY_INPUT_DOWN] > 0 || state[KEY_INPUT_S] > 0);
	}

	bool onLeft() {
		if (waitCnt <= 0 && (state[KEY_INPUT_LEFT] == 1 || state[KEY_INPUT_A] == 1)) {
			initWait();
			use++;
			return true;
		}
		return false;
	}
	bool onGoingLeft() {
		return (state[KEY_INPUT_LEFT] > 0 || state[KEY_INPUT_A] > 0);
	}

	bool onRight() {
		if (waitCnt <= 0 && (state[KEY_INPUT_RIGHT] == 1 || state[KEY_INPUT_D] == 1)) {
			initWait();
			use++;
			return true;
		}
		return false;
	}
	bool onGoingRight() {
		return (state[KEY_INPUT_RIGHT] > 0 || state[KEY_INPUT_D] > 0);
	}

	bool isUsed() {
		if (use > 0) {
			use = 0;
			return true;
		}
		return false;
	}

	int skipMovie(int movieHandle) {
		if (state[KEY_INPUT_W] == 1) {
			PauseMovieToGraph(movieHandle);
			return 1;
		}
		return 0;
	}
};
