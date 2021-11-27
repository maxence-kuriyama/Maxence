#pragma once

#include "DxLib.h"
#include <string>
#include "lib/const.h"
#include "lib/keyboard.h"


// ����̃��[�h�E�Đ�������N���X
// �񓯊��ǂݍ��݂�wrapper
// �P�̂ł̎g�p��z��
class Movie {
private:
	int handle = -1;
	int cnt = 0;		// ���[�f�B���O���b�Z�[�W�p�̃J�E���^
	int loadMsgX = 540;
	int loadMsgY = 0;
	int loadFlg = 0;	// ���łɃ��[�h����Ă��邩�ǂ���
	int strColorDebug = GetColor(0, 200, 0);
	string loadMsgSync = "Loading...";
	string loadMsgAsyncBase = "Loading";

public:
	int vol = 150;		// 1 - 255

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
				// �����I�Ƀ��[�h����ꍇ�̓��b�Z�[�W�𑦔��f
				DrawFormatString(loadMsgX, loadMsgY, strColor, loadMsgSync.c_str());
				ScreenFlip();
				SetUseASyncLoadFlag(FALSE);
			}
			handle = LoadGraph(file_name);
			SetUseASyncLoadFlag(FALSE);

			loadFlg = 1;
			cnt = 0;
			return 1;
		}
		return 0;
	}

	// ��x���[�h������play����������܂Ń��[�h���Ȃ�
	int loadOnce(const char* file_name, int async = 1, int strColor = 0) {
		if (!loadFlg) {
			return load(file_name, async, strColor);
		}
		return 1;
	}

	void unload() {
		DeleteGraph(handle);
		handle = -1;	// unload���ꂽ���
	}

	// ����Đ��i���C�����[�v�Ƃ͕ʂ̃��[�v�ɓ���j
	int play(Key& key, int debug = 0) {
		if (!isLoading()) {
			ChangeMovieVolumeToGraph(vol, handle);
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

	// �񓯊����[�h���̃��[�f�B���O���b�Z�[�W�`��
	void drawLoadMsg(int strColor = 0) {
		if (isLoading()) {
			if (strColor == 0) {
				strColor = GetColor(255, 255, 255);
			}

			string loading = loadMsgAsyncBase;
			for (int i = 0; i < cnt / 15; ++i) {
				loading += ".";
			}
			DrawFormatString(loadMsgX, loadMsgY, strColor, loading.c_str());
			cnt = (cnt + 1) % FPS;
		}
	}

	bool isLoading() {
		// unload���ꂽ��Ԃɑ΂��ĕʓr����
		if (handle < 0) {
			return 0;
		}
		// ���[�h�����ۂ��̔���
		return CheckHandleASyncLoad(handle);
	}

	bool isPrepared() {
		return (loadFlg == 1 && !CheckHandleASyncLoad(handle));
	}

	void debugDump(int debug) {
		if (debug) {
			int strColor = strColorDebug;

			DrawFormatString(5, 205, strColor, "animeCntM: %d", cnt);
			DrawFormatString(5, 225, strColor, "isLoadingM: %d", isLoading());
			DrawFormatString(5, 245, strColor, "isLoadedM: %d", loadFlg);
		}
	}
};
