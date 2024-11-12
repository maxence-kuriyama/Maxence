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

	long start = clock();	// ���������J�n����
	long fpsStart = clock();	// FPS�v���J�n����
	int fpsCnt = 0;			// FPS�v���p
	int fps = 0;			// ����FPS
	int targetFps = FPS;

public:

	// ��������
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
		// ����fps�v��
		fpsCnt++;
		if (clock() - fpsStart > 1000.0) {
			fps = fpsCnt;
			fpsCnt = 0;
			fpsStart = clock();
		}
		// ��������
		while (clock() - start < 1000.0 / targetFps) {
			WaitTimer(1);
		}
		start = clock();
	}
};

Synchronizer* Synchronizer::_instance = NULL;
