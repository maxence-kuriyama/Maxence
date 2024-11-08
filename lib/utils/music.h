#pragma once

#include "DxLib.h"
#include <string>
#include "lib/const.h"
#include "lib/utils/logger.h"
#include "lib/utils/music_unlocker.h"

const string MUSIC_LOAD_MSG_SYNC("Loading...");
const string MUSIC_LOAD_MSG_ASYNC_BASE("Loading");
const int MUSIC_START_FROM_TOP(1);

// ���y�̃��[�h�E�Đ�������N���X
// ���ɔ񓯊��ǂݍ��݂�wrapper
class Music {
private:
	static Music* _instance; // singleton
	
	static Music* getInstance() {
		if (!_instance) {
			_instance = new Music();
		}
		return _instance;
	}
	
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
		delete _instance;
	}

	static string getMusicName(int id) {
		Music* music = Music::getInstance();
		return music->musicName[id];
	}

	// �󃁃����Ɏw�肵�����y�t�@�C����ǂݍ���
	// ���[�h�������������ۂ���Ԃ�
	static int load(const char* fileName, int async = 1, int strColor = 0) {
		Music* music = Music::getInstance();
		return music->_load(fileName, async, strColor);
	}

	// �w�肵�����������������
	static void unload(int hNum = 0) {
		Music* music = Music::getInstance();
		music->_unload(hNum);
	}

	// �S�Ẵ��������������
	static void unloadAll() {
		Music* music = Music::getInstance();
		music->_unload(0);
		music->_unload(1);
	}

	// fromTop = 1�œ��o���Đ�
	static int play(int fromTop = 0) {
		Music* music = Music::getInstance();
		return music->_play(fromTop);
	}

	// �ꎞ��~
	static int stop() {
		Music* music = Music::getInstance();
		return music->_stop();
	}

	// �Đ����̉��y��������A�ҋ@���̉��y���Đ�����
	static int pop(int strColor = 0) {
		Music* music = Music::getInstance();
		return music->_pop(strColor);
	}

	// ���[�v���Ɉ�x����pop����
	static void popOnce() {
		Music* music = Music::getInstance();
		music->_popOnce();
	}

	// �Đ����Ƒҋ@���̉��y��؂�ւ���
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

	// ���ʕύX
	static void changeVolume(int newVol) {
		Music* music = Music::getInstance();
		music->vol = newVol;
	}

	// �񓯊����[�h���̃��[�f�B���O���b�Z�[�W�`��
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
			handle[0] = LoadSoundMem(fileName);
			musicName[0] = fileName;
			loadedHere = 1;
		}
		// ��̉��y�t�@�C���ŗ����̃��������L���Ȃ��悤�ɂ���
		else if (isEmpty(1) && musicName[0] != fileName) {
			handle[1] = LoadSoundMem(fileName);
			musicName[1] = fileName;
			loadedHere = 1;
		}

		// ���[�h���s��ꂽ�ꍇ�̏���
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
			handle[hNum] = -1;	// unload���ꂽ���
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
