#pragma once


#include "lib/logo.h"
unsigned int White = GetColor(255, 255, 255);
unsigned int Black = GetColor(0, 0, 0);

class Option {
public:
	int strColor = White;
	int musicFlg = 1;
	int soundFlg = 1;
	int likeliFlg = 0;		// 学習機械の出力フラグ 0: 非表示, 1: 色で表示, 2: 数値も表示
	int commentFlg = 0;
};

class Key {
public:
	int state[256];
	int debugFlg = 0;
	int waitConst = 5;		// ゲーム中のキー入力ウェイト
	int waitCnt = 20;		// キー入力ウェイトのカウンタ

	void setWait(int wait) {
		waitConst = wait;
	}

	void initWait() {
		waitCnt = waitConst;
	}

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
		if (waitCnt <= 0 && state[KEY_INPUT_RETURN] == 1) {
			initWait();
			return true;
		}
		return false;
	}

	bool onBack() {
		return (state[KEY_INPUT_Z] == 1 || state[KEY_INPUT_BACK] == 1);
	}

	// ゲーム中のキー入力
	bool onCheck() {
		if (waitCnt <= 0 && (state[KEY_INPUT_RETURN] == 1 || state[KEY_INPUT_SPACE] == 1)) {
			initWait();
			return true;
		}
		return false;
	}
	bool onUp() {
		if (waitCnt <= 0 && (state[KEY_INPUT_UP]== 1 || state[KEY_INPUT_W] == 1)) {
			initWait();
			return true;
		}
		return false;
	}
	bool onDown() {
		if (waitCnt <= 0 && (state[KEY_INPUT_DOWN] == 1 || state[KEY_INPUT_S] == 1)) {
			initWait();
			return true;
		}
		return false;
	}
	bool onLeft() {
		if (waitCnt <= 0 && (state[KEY_INPUT_LEFT] == 1 || state[KEY_INPUT_A] == 1)) {
			initWait();
			return true;
		}
		return false;
	}
	bool onRight() {
		if (waitCnt <= 0 && (state[KEY_INPUT_RIGHT] == 1 || state[KEY_INPUT_D] == 1)) {
			initWait();
			return true;
		}
		return false;
	}

	void toggleSetting(Option& option, Logo& logo) {
		// ロゴを動かす
		if (state[KEY_INPUT_AT] == 1) {
			if (logo.acRate >= 0.5) {
				logo.initialize();
			}
			else {
				logo.acRate += 1.0;
			}
		}

		// 文字色の変更
		if (state[KEY_INPUT_I] == 1) {
			if (option.strColor == Black) {
				option.strColor = White;
			}
			else {
				option.strColor = Black;
			}
		}

		//音楽, SEの有無
		if (state[KEY_INPUT_P] == 1) {
			option.musicFlg = (option.musicFlg + 1) % 2;
			option.soundFlg = (option.soundFlg + 1) % 2;
		}

		// コメントを消す
		if (state[KEY_INPUT_K] == 1) {
			option.commentFlg = (option.commentFlg + 1) % 2;
		}
	}

	void configLearning() {
		// データの保存
		if (state[KEY_INPUT_O] == 1) {
			//save parameters
		}

		// AIのウェイトを変更する
		/*
		if (state[KEY_INPUT_M] == 1) {
			waitOnCOM++;
		}
		else if (state[KEY_INPUT_N] == 1) {
			waitOnCOM--;
		}
		*/
	}

	// 返り値1でゲームを初期化する
	int toggleHighSpeedLearning(int& gameFlg) {
		// 高速自動学習モード
		if (state[KEY_INPUT_H] == 1) {
			gameFlg = 5;
			return 1;
		}
		return 0;
	}

	// 返り値1でゲームを初期化する
	int toggleAutoLearning(int& gameFlg, int& taijin) {
		// 自動学習モード
		if (state[KEY_INPUT_A] == 1) {
			taijin = 2;
			gameFlg = 1;
			return 1;
		}
		return 0;
	}

	void toggleDebug(int& debug) {
		//デバッグモード解除
		if (state[KEY_INPUT_G] == 1) {
			debug = (debug + 1) % 2;
			debugFlg = debug;
		}
	}

	void toggleForDebug(Option& option, int& cutinFlg) {
		if (debugFlg) {
			// カットインを入れる
			if (state[KEY_INPUT_C] == 1) {
				cutinFlg = 1;
			}

			// 学習機械の出力を見る
			if (state[KEY_INPUT_V] == 1) {
				option.likeliFlg = (option.likeliFlg + 1) % 3;
			}

			/*
			// エンディングモード
			if (state[KEY_INPUT_MINUS] == 1) {
				if (game.flg != -4) {
					game.flg = -4;
				}
				else {
					StopMusic();
					game.flg = 0;
				}
				end_cnt = 0;
			}
			*/
		}
	}

	void skipBattle(int& gameFlg, int& sceneFlg) {
		if (debugFlg) {
			if (state[KEY_INPUT_B] == 1) {
				StopMusic();
				sceneFlg++;
				gameFlg = -6;
			}
		}
	}
};
