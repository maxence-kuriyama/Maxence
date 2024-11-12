#pragma once

#include <time.h>
#include "lib/const.h"

class Synchronizer {
private:
	static Synchronizer* _instance; // singleton

	static Synchronizer* getInstance() {
		if (!_instance) {
			_instance = new Synchronizer();
		}
		return _instance;
	}

	long start = clock();	// 同期処理開始時刻
	long fpsStart = clock();	// FPS計測開始時刻
	int fpsCnt = 0;			// FPS計測用
	int fps = 0;			// 実効FPS
	int targetFps = FPS;

public:

	// 同期処理
	static void execute() {
		Synchronizer* sync = getInstance();
		sync->_execute();
	}

	static int actualFps() {
		Synchronizer* sync = getInstance();
		return sync->fps;
	}

private:

	void _execute() {
		// 実効fps計測
		fpsCnt++;
		if (clock() - fpsStart > 1000.0) {
			fps = fpsCnt;
			fpsCnt = 0;
			fpsStart = clock();
		}
		// 同期処理
		while (clock() - start < 1000.0 / targetFps) {
			WaitTimer(1);
		}
		start = clock();
	}
};

Synchronizer* Synchronizer::_instance = NULL;
