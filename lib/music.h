#pragma once

#include "DxLib.h"
#include <string>
#include "lib/const.h"
#include "lib/keyboard.h"

class Music {
private:
	int handle[2] = { -1, -1 };			// �Đ����鉹�y�Ƒҋ@���̉��y�̃n���h��
	string musicName[2] = { "", "" };	// ���ꂼ��̃������ɓǂݍ��܂ꂽ�t�@�C����
	int cnt = 0;						// ���[�f�B���O���b�Z�[�W�p�̃J�E���^
	int loadMsgX = 540;
	int loadMsgY = 0;
	int strColorDebug = GetColor(0, 250, 0);
	string loadMsgSync = "Loading...";
	string loadMsgAsyncBase = "Loading";
	int isSwapped = 0;					// ���ł�swap���ꂽ���ۂ�

public:
	int vol = 150;		// 1 - 255

	~Music() {
		unloadAll();
	}

	// �󃁃����Ɏw�肵�����y�t�@�C����ǂݍ���
	// ���[�h�������������ۂ���Ԃ�
	int load(const char* file_name, int async = 1, int strColor = 0) {
		if (strColor == 0) {
			strColor = GetColor(255, 255, 255);
		}

		int loadedHere = 0;		// ���[�h���s��ꂽ���ۂ�

		if (async) {
			SetUseASyncLoadFlag(TRUE);
		}
		else {
			if (isEmpty(0) || isEmpty(1)) {
				// �����I�Ƀ��[�h����ꍇ�̓��b�Z�[�W�𑦔��f
				DrawFormatString(loadMsgX, loadMsgY, strColor, loadMsgSync.c_str());
				ScreenFlip();
				SetUseASyncLoadFlag(FALSE);
			}
		}

		// ����������Ȃ烍�[�h
		if (isEmpty(0)) {
			handle[0] = LoadSoundMem(file_name);
			musicName[0] = file_name;
			loadedHere = 1;
		}
		// ��̉��y�t�@�C���ŗ����̃��������L���Ȃ��悤�ɂ���
		else if (isEmpty(1) && musicName[0] != file_name) {
			handle[1] = LoadSoundMem(file_name);
			musicName[1] = file_name;
			loadedHere = 1;
		}

		// ���[�h���s��ꂽ�ꍇ�̏���
		if (loadedHere) {
			SetUseASyncLoadFlag(FALSE);
			cnt = 0;
			return 1;
		}

		SetUseASyncLoadFlag(FALSE);
		return 0;
	}

	// �w�肵�����������������
	void unload(int hNum = 0) {
		if (hNum == 0 || hNum == 1) {
			DeleteSoundMem(handle[hNum]);
			handle[hNum] = -1;	// unload���ꂽ���
			musicName[hNum] = "";
		}
	}

	// �S�Ẵ��������������
	void unloadAll() {
		unload(0);
		unload(1);
	}

	// �Đ����̉��y��������A�ҋ@���̉��y���Đ�����
	int pop(int strColor = 0) {
		if (swap(strColor)) {
			unload(1);
			return 1;
		}
		return 0;
	}

	// �Đ����Ƒҋ@���̉��y��؂�ւ���
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

	// ���[�v���Ɉ�x����pop����
	void popOnce() {
		if (!isSwapped) {
			swap();
			unload(1);
		}
	}

	// ���[�v���Ɉ�x����swap����
	void swapOnce() {
		if (!isSwapped) {
			swap();
		}
	}

	void enableSwap() {
		isSwapped = 0;
	}

	// fromTop = 1�œ��o���Đ�
	int play(int fromTop = 0, int debug = 0) {
		if (!isLoading()) {
			ChangeVolumeSoundMem(vol, handle[0]);
			PlaySoundMem(handle[0], DX_PLAYTYPE_BACK, fromTop);
			return 1;
		}
		return 0;
	}

	// �ꎞ��~
	int stop() {
		if (!isLoading()) {
			StopSoundMem(handle[0]);
			return 1;
		}
		return 0;
	}

	// �񓯊����[�h���̃��[�f�B���O���b�Z�[�W�`��
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

	// �w�肵���n���h�����󂩂ǂ���
	bool isEmpty(int hNum = 0) {
		if (hNum == 0 || hNum == 1) {
			return handle[hNum] == -1;
		}
		return true;	// �s���Ȏw��ɑ΂��Ă͋�ł���Ƃ���
	}

	// �w�肵���n���h�������[�h�����ǂ���
	bool isLoading(int hNum = 0) {
		if (hNum == 0 || hNum == 1) {
			// ��n���h���ɑ΂��ĕʓr����
			if (isEmpty(hNum)) {
				return false;
			}
			// ���[�h�����ۂ��̔���
			return CheckHandleASyncLoad(handle[hNum]);
		}
		return false;
	}

	// �w�肵���n���h�����Đ��\���ǂ���
	bool isPrepared(int hNum = 0) {
		if (hNum == 0 || hNum == 1) {
			// ��n���h���ɑ΂��ĕʓr����
			if (isEmpty(hNum)) {
				return false;
			}
			// ���[�h���I����Ă��邩�ۂ��̔���
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
