#pragma once

#include "DxLib.h"
#include <string>
#include "lib/const.h"
#include "lib/utils/logger.h"
#include "lib/music_unlocker.h"

const string MUSIC_LOAD_MSG_SYNC("Loading...");
const string MUSIC_LOAD_MSG_ASYNC_BASE("Loading");
const int MUSIC_START_FROM_TOP(1);

// ���y�̃��[�h�E�Đ�������N���X
// ���ɔ񓯊��ǂݍ��݂�wrapper
// �P�̂ł̎g�p��z��
class Music {
private:
	int handle[2] = { -1, -1 };			// �Đ����鉹�y�Ƒҋ@���̉��y�̃n���h��
	int cnt = 0;						// ���[�f�B���O���b�Z�[�W�p�̃J�E���^
	int loadMsgX = 540;
	int loadMsgY = 0;
	int isSwapped = 0;					// ���ł�swap���ꂽ���ۂ�
	int vol = 150;						// from 1 to 255

	int strColorDebug = GetColor(0, 250, 250);

public:
	string musicName[2] = { "", "" };	// ���ꂼ��̃������ɓǂݍ��܂ꂽ�t�@�C����

	~Music() {
		unloadAll();
	}

	// �󃁃����Ɏw�肵�����y�t�@�C����ǂݍ���
	// ���[�h�������������ۂ���Ԃ�
	int load(const char* file_name, int async = 1, int strColor = 0) {
		if (strColor == 0) {
			strColor = GetColor(255, 255, 255);
		}

		int loadedHere = 0; // ���[�h���s��ꂽ���ۂ�

		if (async) {
			SetUseASyncLoadFlag(TRUE);
		}
		else {
			if (isEmpty(0) || isEmpty(1)) {
				// �����I�Ƀ��[�h����ꍇ�̓��b�Z�[�W�𑦔��f
				DrawFormatString(loadMsgX, loadMsgY, strColor, MUSIC_LOAD_MSG_SYNC.c_str());
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
			loggingLoaded(file_name);
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
			loggingUnloaded(musicName[hNum]);
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
	int swap(int strColor = 0, int noPlay = 0) {
		if (isPrepared(1)) {
			stop();
			int tmp = handle[0];
			handle[0] = handle[1];
			handle[1] = tmp;
			string tmpStr = musicName[0];
			musicName[0] = musicName[1];
			musicName[1] = tmpStr;
			if (!noPlay) play();
			isSwapped = 1;
			return 1;
		}
		drawLoadMsg(strColor, 1);
		return 0;
	}

	int swapWithoutPlay(int strColor = 0) {
		return swap(strColor, 1);
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
	int play(int fromTop = 0) {
		if (isPrepared()) {
			ChangeVolumeSoundMem(vol, handle[0]);
			PlaySoundMem(handle[0], DX_PLAYTYPE_BACK, fromTop);
			MusicUnlocker::unlock(musicName[0]);
			return 1;
		}
		return 0;
	}

	// �ꎞ��~
	int stop() {
		if (isPrepared()) {
			StopSoundMem(handle[0]);
			return 1;
		}
		return 0;
	}

	// ���ʕύX
	void changeVolume(int newVol) {
		vol = newVol;
	}

	// �񓯊����[�h���̃��[�f�B���O���b�Z�[�W�`��
	int drawLoadMsg(int strColor = 0, int hNum = 0) {
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
			return 1;
		}
		return 0;
	}

	// �w�肵���n���h�����󂩂ǂ���
	bool isEmpty(int hNum = 0) {
		if (hNum == 0 || hNum == 1) {
			return handle[hNum] == -1;
		}
		return true; // �s���Ȏw��ɑ΂��Ă͋�ł���Ƃ���
	}

	// �w�肵���n���h�������[�h�����ǂ���
	bool isLoading(int hNum = 0) {
		if (isEmpty(hNum)) return false;
		// ���[�h�����ۂ��̔���
		return CheckHandleASyncLoad(handle[hNum]);
	}

	// �w�肵���n���h�����Đ��\���ǂ���
	bool isPrepared(int hNum = 0) {
		if (isEmpty(hNum)) return false;
		// ���[�h���I����Ă��邩�ۂ��̔���
		return !CheckHandleASyncLoad(handle[hNum]);
	}

	void debugDump() {
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

	void loggingLoaded(const char* file_name) {
		Logger::ss << "Load " << file_name;
		Logger::log();
	}

	void loggingUnloaded(string music_name) {
		Logger::ss << "Unload " << music_name;
		Logger::log();
	}
};
