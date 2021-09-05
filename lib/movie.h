#pragma once

#include "lib/const.h"
#include "lib/keyboard.h"

class  Movie {
private:
	int handle = -1;
	int cnt = 0;		// ローディングメッセージ用のカウンタ
	int loadMsgX = 540;
	int loadMsgY = 0;
	int loadFlg = 0;	// すでにロードされているかどうか
	int strColorDebug = GetColor(0, 200, 0);

public:

	~Movie() {
		unload();
	}

	int load(const char* file_name, int async = 1, int strColor = 0) {
		if (!isLoading()) {
			if (strColor == 0) {
				strColor = GetColor(255, 255, 255);
			}

			unload();
			if (async) {
				SetUseASyncLoadFlag(TRUE);
			}
			else {
				// 同期的にロードする場合はメッセージを即反映
				DrawFormatString(loadMsgX, loadMsgY, strColor, "Loading...");
				ScreenFlip();
				SetUseASyncLoadFlag(FALSE);
			}
			handle = LoadGraph(file_name);
			SetUseASyncLoadFlag(TRUE);

			loadFlg = 1;
			cnt = 0;
			return 1;
		}
		return 0;
	}

	// 一度ロードしたらplayが成功するまでロードしない
	int loadOnce(const char* file_name, int async = 1, int strColor = 0) {
		if (!loadFlg) {
			return load(file_name, async, strColor);
		}
		return 1;
	}

	void unload() {
		DeleteGraph(handle);
		handle = -1;	// unloadされた状態
	}

	// 動画再生（メインループとは別のループに入る）
	int play(Key& key, int debug = 0) {
		if (!isLoading()) {
			PlayMovieToGraph(handle);
			while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && GetMovieStateToGraph(handle)) {
				key.update();
				if (key.skipMovie(handle)) {
					break;
				}
				DrawExtendGraph(0, 60, 640, 420, handle, FALSE);
				debugDump(debug);
				WaitTimer(10);
			}
			loadFlg = 0;
			return 1;
		}
		return 0;
	}

	// 非同期ロード時のローディングメッセージ描画
	void drawLoadMsg(int strColor = 0) {
		if (isLoading()) {
			if (strColor == 0) {
				strColor = GetColor(255, 255, 255);
			}

			string loading = "Loading";
			for (int i = 0; i < cnt / 15; ++i) {
				loading += ".";
			}
			DrawFormatString(loadMsgX, loadMsgY, strColor, loading.c_str());
			cnt = (cnt + 1) % FPS;
		}
	}

	bool isLoading() {
		// unloadされた状態に対して別途処理
		if (handle < 0) {
			return 0;
		}
		// ロード中か否かの判定
		return CheckHandleASyncLoad(handle);
	}

	bool isPrepared() {
		return (loadFlg == 1 && !CheckHandleASyncLoad(handle));
	}

	void debugDump(int debug) {
		if (debug) {
			int strColor = strColorDebug;

			DrawFormatString(5, 205, strColor, "animeCnt: %d", cnt);
			DrawFormatString(5, 225, strColor, "isLoading: %d", isLoading());
			DrawFormatString(5, 245, strColor, "isLoaded: %d", loadFlg);
		}
	}
};
