#pragma once

#include <time.h>
#include "lib/const.h"


class Synchronizer {
private:
	long start = clock();	// 同期処理開始時刻
	long fpsStart = clock();	// FPS計測開始時刻
	int fpsCnt = 0;			// FPS計測用

public:
	int fps = 0;			// 実効FPS

	// 同期処理
	void execute() {
		// 実効fps計測
		fpsCnt++;
		if (clock() - fpsStart > 1000.0) {
			fps = fpsCnt;
			fpsCnt = 0;
			fpsStart = clock();
		}
		// 同期処理
		while (clock() - start < 1000.0 / FPS) {
			WaitTimer(1);
		}
		start = clock();
	}
};