#pragma once

#include "DxLib.h"
#include <string>
#include "lib/const.h"
#include "lib/utils/logger.h"
#include "lib/utils/music_unlocker.h"

const string MUSIC_LOAD_MSG_SYNC("Loading...");
const string MUSIC_LOAD_MSG_ASYNC_BASE("Loading");
const int MUSIC_START_FROM_TOP(1);

// 音楽のロード・再生をするクラス
// 特に非同期読み込みのwrapper
class Music {
private:
	static Music* _instance; // singleton
	
	static Music* getInstance() {
		if (!_instance) {
			_instance = new Music();
		}
		return _instance;
	}
	
	int handle[2] = { -1, -1 };			// 再生する音楽と待機中の音楽のハンドル
	int cnt = 0;						// ローディングメッセージ用のカウンタ
	int loadMsgX = 540;
	int loadMsgY = 0;
	int isSwapped = 0;					// すでにswapされたか否か
	int vol = 150;						// from 1 to 255

	int strColorDebug = GetColor(0, 250, 250);

public:
	string musicName[2] = { "", "" };	// それぞれのメモリに読み込まれたファイル名

	~Music() {
		unloadAll();
		delete _instance;
	}

	static string getMusicName(int id) {
		Music* music = Music::getInstance();
		return music->musicName[id];
	}

	// 空メモリに指定した音楽ファイルを読み込む
	// ロードが成功したか否かを返す
	static int load(const char* fileName, int async = 1, int strColor = 0) {
		Music* music = Music::getInstance();
		return music->_load(fileName, async, strColor);
	}

	// 指定したメモリを解放する
	static void unload(int hNum = 0) {
		Music* music = Music::getInstance();
		music->_unload(hNum);
	}

	// 全てのメモリを解放する
	static void unloadAll() {
		Music* music = Music::getInstance();
		music->_unload(0);
		music->_unload(1);
	}

	// fromTop = 1で頭出し再生
	static int play(int fromTop = 0) {
		Music* music = Music::getInstance();
		return music->_play(fromTop);
	}

	// 一時停止
	static int stop() {
		Music* music = Music::getInstance();
		return music->_stop();
	}

	// 再生中の音楽を解放し、待機中の音楽を再生する
	static int pop(int strColor = 0) {
		Music* music = Music::getInstance();
		return music->_pop(strColor);
	}

	// ループ中に一度だけpopする
	static void popOnce() {
		Music* music = Music::getInstance();
		music->_popOnce();
	}

	// 再生中と待機中の音楽を切り替える
	static int swap(int strColor = 0, int noPlay = 0) {
		Music* music = Music::getInstance();
		return music->_swap(strColor, noPlay);
	}

	static int swapWithoutPlay(int strColor = 0) {
		Music* music = Music::getInstance();
		return music->_swap(strColor, 1);
	}

	static void enableSwap() {
		Music* music = Music::getInstance();
		music->isSwapped = 0;
	}

	// 音量変更
	static void changeVolume(int newVol) {
		Music* music = Music::getInstance();
		music->vol = newVol;
	}

	// 非同期ロード時のローディングメッセージ描画
	static bool drawLoadMsg(int strColor = 0, int hNum = 0) {
		Music* music = Music::getInstance();
		return music->_drawLoadMsg(strColor, hNum);
	}

	static void debugDump() {
		Music* music = Music::getInstance();
		music->_debugDump();
	}

private:

	int _load(const char* fileName, int async = 1, int strColor = 0) {
		if (strColor == 0) {
			strColor = GetColor(255, 255, 255);
		}

		int loadedHere = 0; // ロードが行われたか否か

		if (async) {
			SetUseASyncLoadFlag(TRUE);
		}
		else {
			if (isEmpty(0) || isEmpty(1)) {
				// 同期的にロードする場合はメッセージを即反映
				DrawFormatString(loadMsgX, loadMsgY, strColor, MUSIC_LOAD_MSG_SYNC.c_str());
				ScreenFlip();
				SetUseASyncLoadFlag(FALSE);
			}
		}

		// メモリが空ならロード
		if (isEmpty(0)) {
			handle[0] = LoadSoundMem(fileName);
			musicName[0] = fileName;
			loadedHere = 1;
		}
		// 一つの音楽ファイルで両方のメモリを占有しないようにする
		else if (isEmpty(1) && musicName[0] != fileName) {
			handle[1] = LoadSoundMem(fileName);
			musicName[1] = fileName;
			loadedHere = 1;
		}

		// ロードが行われた場合の処理
		if (loadedHere) {
			loggingLoaded(fileName);
			SetUseASyncLoadFlag(FALSE);
			cnt = 0;
			return 1;
		}

		SetUseASyncLoadFlag(FALSE);
		return 0;
	}

	void _unload(int hNum = 0) {
		if (hNum == 0 || hNum == 1) {
			DeleteSoundMem(handle[hNum]);
			handle[hNum] = -1;	// unloadされた状態
			loggingUnloaded(musicName[hNum]);
			musicName[hNum] = "";
		}
	}

	int _play(int fromTop = 0) {
		if (isPrepared()) {
			ChangeVolumeSoundMem(vol, handle[0]);
			PlaySoundMem(handle[0], DX_PLAYTYPE_BACK, fromTop);
			MusicUnlocker::unlock(musicName[0]);
			return 1;
		}
		return 0;
	}

	int _stop() {
		if (isPrepared()) {
			StopSoundMem(handle[0]);
			return 1;
		}
		return 0;
	}

	int _pop(int strColor = 0) {
		if (_swap(strColor)) {
			_unload(1);
			return 1;
		}
		return 0;
	}

	void _popOnce() {
		if (!isSwapped) {
			_swap();
			_unload(1);
		}
	}

	int _swap(int strColor = 0, int noPlay = 0) {
		if (isPrepared(1)) {
			_stop();
			int tmp = handle[0];
			handle[0] = handle[1];
			handle[1] = tmp;
			string tmpStr = musicName[0];
			musicName[0] = musicName[1];
			musicName[1] = tmpStr;
			if (!noPlay) _play();
			isSwapped = 1;
			return 1;
		}
		_drawLoadMsg(strColor, 1);
		return 0;
	}

	bool _drawLoadMsg(int strColor = 0, int hNum = 0) {
		if (isLoading(hNum)) {
			if (strColor == 0) {
				strColor = GetColor(255, 255, 255);
			}

			string loading = MUSIC_LOAD_MSG_ASYNC_BASE;
			for (int i = 0; i < (cnt % FPS) / 15; ++i) {
				loading += ".";
			}
			DrawFormatString(loadMsgX, loadMsgY, strColor, loading.c_str());
			cnt++;
			return true;
		}
		return false;
	}

	// 指定したハンドルが空かどうか
	bool isEmpty(int hNum = 0) {
		if (hNum == 0 || hNum == 1) {
			return handle[hNum] == -1;
		}
		return true; // 不正な指定に対しては空であるとする
	}

	// 指定したハンドルがロード中かどうか
	bool isLoading(int hNum = 0) {
		if (isEmpty(hNum)) return false;
		// ロード中か否かの判定
		return CheckHandleASyncLoad(handle[hNum]);
	}

	// 指定したハンドルが再生可能かどうか
	bool isPrepared(int hNum = 0) {
		if (isEmpty(hNum)) return false;
		// ロードが終わっているか否かの判定
		return !CheckHandleASyncLoad(handle[hNum]);
	}

	void loggingLoaded(const char* file_name) {
		Logger::ss << "Load " << file_name;
		Logger::log();
	}

	void loggingUnloaded(string music_name) {
		Logger::ss << "Unload " << music_name;
		Logger::log();
	}

	void _debugDump() {
		int strColor = strColorDebug;

		DrawFormatString(5, 265, strColor, "animeCntS: %d", cnt);
		DrawFormatString(5, 285, strColor, "volume: %d", vol);
		DrawFormatString(5, 305, strColor, "isEmptyS0: %d", isEmpty(0));
		DrawFormatString(5, 325, strColor, "isLoadingS0: %d", isLoading(0));
		DrawFormatString(5, 345, strColor, "isLoadedS0: %s", musicName[0].c_str());
		DrawFormatString(5, 365, strColor, "isEmptyS1: %d", isEmpty(1));
		DrawFormatString(5, 385, strColor, "isLoadingS1: %d", isLoading(1));
		DrawFormatString(5, 405, strColor, "isLoadedS1: %s", musicName[1].c_str());
	}
};

Music* Music::_instance = NULL;
