#pragma once

#include "DxLib.h"
#include <string>
#include "lib/const.h"
#include "lib/keyboard.h"

class Music {
private:
	int handle[2] = { -1, -1 };			// 再生する音楽と待機中の音楽のハンドル
	string musicName[2] = { "", "" };	// それぞれのメモリに読み込まれたファイル名
	int cnt = 0;						// ローディングメッセージ用のカウンタ
	int loadMsgX = 540;
	int loadMsgY = 0;
	int strColorDebug = GetColor(0, 250, 0);
	string loadMsgSync = "Loading...";
	string loadMsgAsyncBase = "Loading";
	int isSwapped = 0;					// すでにswapされたか否か

public:
	int vol = 150;		// 1 - 255

	~Music() {
		unloadAll();
	}

	// 空メモリに指定した音楽ファイルを読み込む
	// ロードが成功したか否かを返す
	int load(const char* file_name, int async = 1, int strColor = 0) {
		if (strColor == 0) {
			strColor = GetColor(255, 255, 255);
		}

		int loadedHere = 0;		// ロードが行われたか否か

		if (async) {
			SetUseASyncLoadFlag(TRUE);
		}
		else {
			if (isEmpty(0) || isEmpty(1)) {
				// 同期的にロードする場合はメッセージを即反映
				DrawFormatString(loadMsgX, loadMsgY, strColor, loadMsgSync.c_str());
				ScreenFlip();
				SetUseASyncLoadFlag(FALSE);
			}
		}

		// メモリが空ならロード
		if (isEmpty(0)) {
			handle[0] = LoadSoundMem(file_name);
			musicName[0] = file_name;
			loadedHere = 1;
		}
		// 一つの音楽ファイルで両方のメモリを占有しないようにする
		else if (isEmpty(1) && musicName[0] != file_name) {
			handle[1] = LoadSoundMem(file_name);
			musicName[1] = file_name;
			loadedHere = 1;
		}

		// ロードが行われた場合の処理
		if (loadedHere) {
			SetUseASyncLoadFlag(FALSE);
			cnt = 0;
			return 1;
		}

		SetUseASyncLoadFlag(FALSE);
		return 0;
	}

	// 指定したメモリを解放する
	void unload(int hNum = 0) {
		if (hNum == 0 || hNum == 1) {
			DeleteSoundMem(handle[hNum]);
			handle[hNum] = -1;	// unloadされた状態
			musicName[hNum] = "";
		}
	}

	// 全てのメモリを解放する
	void unloadAll() {
		unload(0);
		unload(1);
	}

	// 再生中の音楽を解放し、待機中の音楽を再生する
	int pop(int strColor = 0) {
		if (swap(strColor)) {
			unload(1);
			return 1;
		}
		return 0;
	}

	// 再生中と待機中の音楽を切り替える
	int swap(int strColor = 0) {
		if (!isLoading(1)) {
			stop();
			int tmp = handle[0];
			handle[0] = handle[1];
			handle[1] = tmp;
			string tmpStr = musicName[0];
			musicName[0] = musicName[1];
			musicName[1] = tmpStr;
			play();
			isSwapped = 1;
			return 1;
		}
		drawLoadMsg(strColor, 1);
		return 0;
	}

	// ループ中に一度だけpopする
	void popOnce() {
		if (!isSwapped) {
			swap();
			unload(1);
		}
	}

	// ループ中に一度だけswapする
	void swapOnce() {
		if (!isSwapped) {
			swap();
		}
	}

	void enableSwap() {
		isSwapped = 0;
	}

	// fromTop = 1で頭出し再生
	int play(int fromTop = 0, int debug = 0) {
		if (!isLoading()) {
			ChangeVolumeSoundMem(vol, handle[0]);
			PlaySoundMem(handle[0], DX_PLAYTYPE_BACK, fromTop);
			return 1;
		}
		return 0;
	}

	// 一時停止
	int stop() {
		if (!isLoading()) {
			StopSoundMem(handle[0]);
			return 1;
		}
		return 0;
	}

	// 非同期ロード時のローディングメッセージ描画
	int drawLoadMsg(int strColor = 0, int hNum = 0) {
		if (isLoading(hNum)) {
			if (strColor == 0) {
				strColor = GetColor(255, 255, 255);
			}

			string loading = loadMsgAsyncBase;
			for (int i = 0; i < (cnt % FPS) / 15; ++i) {
				loading += ".";
			}
			DrawFormatString(loadMsgX, loadMsgY, strColor, loading.c_str());
			cnt++;
			return 1;
		}
		return 0;
	}

	// 指定したハンドルが空かどうか
	bool isEmpty(int hNum = 0) {
		if (hNum == 0 || hNum == 1) {
			return handle[hNum] == -1;
		}
		return true;	// 不正な指定に対しては空であるとする
	}

	// 指定したハンドルがロード中かどうか
	bool isLoading(int hNum = 0) {
		if (hNum == 0 || hNum == 1) {
			// 空ハンドルに対して別途処理
			if (isEmpty(hNum)) {
				return false;
			}
			// ロード中か否かの判定
			return CheckHandleASyncLoad(handle[hNum]);
		}
		return false;
	}

	// 指定したハンドルが再生可能かどうか
	bool isPrepared(int hNum = 0) {
		if (hNum == 0 || hNum == 1) {
			// 空ハンドルに対して別途処理
			if (isEmpty(hNum)) {
				return false;
			}
			// ロードが終わっているか否かの判定
			return !CheckHandleASyncLoad(handle[hNum]);
		}
		return false;
	}

	void debugDump(int debug) {
		if (debug) {
			int strColor = strColorDebug;

			DrawFormatString(5, 265, strColor, "animeCntS: %d", cnt);
			DrawFormatString(5, 285, strColor, "isEmptyS0: %d", isEmpty(0));
			DrawFormatString(5, 305, strColor, "isLoadingS0: %d", isLoading(0));
			DrawFormatString(5, 325, strColor, "isLoadedS0: %s", musicName[0].c_str());
			DrawFormatString(5, 345, strColor, "isEmptyS1: %d", isEmpty(1));
			DrawFormatString(5, 365, strColor, "isLoadingS1: %d", isLoading(1));
			DrawFormatString(5, 385, strColor, "isLoadedS1: %s", musicName[1].c_str());
		}
	}

};
