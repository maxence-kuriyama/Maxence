#pragma once

#include <time.h>
#include "lib/const.h"


class Synchronizer {
private:
	long start = clock();	// ���������J�n����
	long fpsStart = clock();	// FPS�v���J�n����
	int fpsCnt = 0;			// FPS�v���p

public:
	int fps = 0;			// ����FPS

	// ��������
	void execute() {
		// ����fps�v��
		fpsCnt++;
		if (clock() - fpsStart > 1000.0) {
			fps = fpsCnt;
			fpsCnt = 0;
			fpsStart = clock();
		}
		// ��������
		while (clock() - start < 1000.0 / FPS) {
			WaitTimer(1);
		}
		start = clock();
	}
};