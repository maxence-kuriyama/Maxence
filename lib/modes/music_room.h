#pragma once

#include <sstream>
#include <iomanip>
#include "lib/const.h"
#include "lib/music.h"
#include "lib/music_unlocker.h"
#include "lib/bg_character.h"
#include "lib/user_input.h"
#include "lib/modes/common/message.h"
#include "lib/modes/common/fireflower.h"

const int MUSIC_NUM(15);
const int MUSIC_MAX_INDEX(MUSIC_NUM);
const int MUSIC_ROOM_FIRE_FLOWER_NUM(2);

const int MUSIC_LEFT_X(50);
const int MUSIC_LEFT_X2(320);
const int MUSIC_DIV_X(10);
const int MUSIC_TOP_Y(40);
const int MUSIC_DIV_Y(40);
const int MUSIC_SPEAKER_ICON_SIZE(36);
const int MUSIC_MESSAGE_INT_X(30);
const int MUSIC_MESSAGE_END_X(610);

// ���y�����[�h
class MusicRoom {
private:
	int strColorMenu = GetColor(255, 255, 255);
	//int strColor = GetColor(255, 255, 255);
	int strColorDebug = GetColor(255, 215, 0);
	int iconSpeaker = LoadGraph("graph/speaker.png");

	struct MusicInfo {
		int id;
		string ext;
		string name;
		int who;
		string desc;
	};

	struct MusicInfo infoList[MUSIC_NUM] = {};
	const struct MusicInfo infoMaster[MUSIC_NUM] = {
		{ 1,	"m4a",	"It's Maxence!!!",					BG_WHO_NONE,			"�I�[�v�j���O�ȁB��ԍŏ��ɍ��܂����B\n�`���ɋ���点��킭�킭�����`���Ί������ł���\n���A�{�҂Ŗ`�����ĂȂ����āH �C�̂����ł���B" },
		{ 2,	"ogg",	"Mr.K is talking to you",			BG_WHO_PL_YELLOW,	"Mr.K�̃e�[�}�B�`���[�g���A���ŗ���܂��B\n�ˑR��肩���Ă���A�C�c�̉�������\�����܂�����\n���Ȃ݂ɔނ͑S���T���O���X�ł��B�C�J���Ă܂��ˁI" },
		{ 3,	"ogg",	"�C�Ǝ��C�Ǝ�",						BG_WHO_NONE,			"���E�i�����j���ݍ��ޕs���ȋ�...���C�I���C�I\n���C�I���C�I���C�I���C�I���C�I���C�I���C�I���C�I\n���Ė��I���܉���u���C�v���Ă������ł��傤���H" },
		{ 4,	"ogg",	"Mr.K is shouting at you",			BG_WHO_RED,			"���V���ďP���������Ă���Ԃ��A�C�c�̃e�[�}�B\n�Ԃ��A�C�c�̓��f���ƂȂ�l������Ƃ����Ȃ��Ƃ��B\n�����ƒN�������悤��..." },
		{ 5,	"ogg",	"Mr.K is smiling on you",			BG_WHO_GREEN,		"���Ԃ̎��𓉂ޗ΂̃A�C�c�̃e�[�}�B\n�΂̃A�C�c�͊��Ƃ�����ȃC���[�W������܂��B\n�ނ͍Ō�ɂ�����������������̂ł��傤���H�H" },
		{ 6,	"ogg",	"Mr.K is lying to you",				BG_WHO_BLUE,			"�d���ɒ��������A�C�c�̃e�[�}�B\n��S���璇�Ԃ�؂�̂ĉ���ڎw���߂����A�C�c�B\n���ς��Ɣ߂������\���ł��Ă���΍K���ł���" },
		{ 7,	"ogg",	"�^���̕��t�_",						BG_WHO_NONE,			"�^������鎞�̋ȁB�Ȗ��͋r�{��A���ɂ�閽���B\n�~�Ɋ����������Ďv���Ȃ��������L��������܂��B\n�^���ɂ����t�_�͂���񂾂�Ȃ�..." },
		{ 8,	"ogg",	"Mr.K is playing with you",			BG_WHO_YELLOW,		"����L�������������F���A�C�c�Ɛ키���̃e�[�}�B\n�`���[�g���A���Ȃ̃A�����W�ɂȂ��Ă��܂��B\nMr.K�炵���Ȃ�ڎw���܂�����" },
		{ 9,	"ogg",	"���͐Â��ɐX�ɋA��",				BG_WHO_NONE,			"�\�G���f�B���O�ȁB�Ȗ��͋r�{��A���ɂ�閽���B\n�Ȃ����Ȃ̂��H�{���Ɏ��Ȃ̂��H�Ȃ��X�ɋA��̂��H\n����͎��݂̂��m���Ă���̂ł���B" },
		{ 10,	"ogg",	"Waterly blue mountain",			BG_WHO_NONE,			"�v���O�����S����T���Ƃ̍���B�{�[�i�X�g���b�N�B\n�Ȗ��̗R���͐���҂̈�l�̖��O�Ƃ����\��...\n����ȏ㏑���Ɠ{��ꂿ�Ⴂ�����Ȃ̂ł��̕ӂŁB" },
		{ 11,	"ogg",	"Dear The Deer",					BG_WHO_DEER,			"�����[�g�˓��I�I��̎��̃e�[�}�ȁB\n�킢�O�̐Â������ǃo�`�o�`�Ƃ������͋C��\n��������ȂɂȂ��Ă���Ί������ł���" },
		{ 12,	"ogg",	"The biggest deer",					BG_WHO_DEER,			"�����X�{�X��̋ȁB���󂷂�ƍł��傫�Ȏ��B\n��Ԃ𑀂�\�͂�����Ƃ��B\n���̂̒m��Ȃ��������\���ł��Ă���Ί������ł���" },
		{ 13,	"ogg",	"�s��������������m�����̃u���[�X",	BG_WHO_NONE,			"�^�G���f�B���O�B�����܂ŗV��ł���Ă��肪�Ƃ��B\n���Ȃ��������{����Mr.K�Ȃ̂�������Ȃ��B\n�u���[�X���đ�������Ȋ�������ˁA���Ԃ�..." },
		{ 14,	"ogg",	"Fading memories",					BG_WHO_NONE,			"���������s��ȕ��͋C�̋ȂɂȂ�܂����B\n������͂������ǎg����ʂ��Ȃ���...\n...������{�[�i�X�g���b�N�Ƃ�����ł��ˁI" },
		{ 15,	"ogg",	"�ނ�D",							BG_WHO_NONE,			"�Q�[���I�[�o�[���ɗ���܂��B\n�����������ɃN���A�ł������Ȃ��̓X�S�C�I�I\n�̂ю�т�\�����邱�Ƃ�ڎw���č��܂����B" },
	};
	const struct MusicInfo lockedMusicInfo = { 0, "wav", "??????????", BG_WHO_NONE, "���̋Ȃ͂܂����������Ƃ��Ȃ���" };
	string defaultMsgStr = "�����͉��y��\n�����̕�������Ȃ��ǂ�������";

	Menu menu;
	Button buttons[MUSIC_NUM + 1]; // �����̗v�f�́u�^�C�g���ցv
	Message msg;
	fireflower tama[MUSIC_ROOM_FIRE_FLOWER_NUM];
	BgCharacter bg;
	int choice = -1;
	bool isLoading = false;

public:

	MusicRoom() {
		initialize();
	}

	void initialize() {
		initializeMusicInfo();
		initializeMessage();
		initializeButtons();
		initializeFireFlower();
		choice = -1;
		isLoading = false;
	}

	int show(UserInput& ui, Music& music) {
		bg.draw();
		showFireFlower();
		msg.draw();
		drawSpeakerIcon();
	
		int currentChoice = choose(ui);

		// �^�C�g���֖߂�
		if (currentChoice == MUSIC_MAX_INDEX) return FLAG_TITLE;

		// �ʂ̋Ȃ�I��
		if (currentChoice != -1) {
			choice = currentChoice;
			music.stop();
			setBgCharacter();
			showMusicDesc();
		}

		if (currentChoice != -1 || isLoading) {
			if (play(music)) {
				isLoading = false;
			}
			else {
				isLoading = true;
				music.drawLoadMsg();
			}
		}
		return FLAG_MUSIC_ROOM;
	}

	void debugDump() {
		int strColor = strColorDebug;

		DrawFormatString(365, 65, strColor, "musicChoice: %d", choice);
	}

private:

	int choose(UserInput& ui) {
		return menu.choose(ui, strColorMenu);
	}

	bool reset(UserInput& ui) {}

	int play(Music& music) {
		if (choice == -1) return 1;

		MusicInfo info = infoList[choice];
		string filename = getMusicFileName(info);
		if (music.musicName[0] == filename) {
			return (music.stop() && music.play(MUSIC_START_FROM_TOP));
		}
		else if (music.musicName[1] == filename) {
			return (music.swapWithoutPlay() && music.play(MUSIC_START_FROM_TOP));
		}
		else {
			music.unload(1); // �K�v?
			music.load(filename.c_str());
			return 0;
		}
	}

	string getMusicFileName(MusicInfo& info) {
		ostringstream ss;
		ss << "./sound/bgm";
		ss << setfill('0') << setw(2) << info.id;
		ss << "." << info.ext;
		return ss.str();
	}

	void setBgCharacter() {
		if (choice == -1) return;

		MusicInfo info = infoList[choice];
		bg.set(info.who);
	}

	/*===========================*/
	//    �`��nprivate�֐�
	/*===========================*/
	void showMusicDesc() {
		MusicInfo info = infoList[choice];
		msg.setWithoutNext(info.desc, MESSAGE_WHO_DESC);
	}

	void showFireFlower() {
		for (int i = 0; i < MUSIC_ROOM_FIRE_FLOWER_NUM; ++i) {
			tama[i].draw();
			tama[i].tick();
		}
	}

	void drawSpeakerIcon() {
		if (choice == -1) return;

		int x, y;
		if (choice < (MUSIC_NUM + 1) / 2) {
			x = choice * MUSIC_DIV_X + MUSIC_LEFT_X - MUSIC_SPEAKER_ICON_SIZE - 5;
			y = choice * MUSIC_DIV_Y + MUSIC_TOP_Y - MUSIC_SPEAKER_ICON_SIZE / 3;
		}
		else if (choice < MUSIC_NUM) {
			x = (choice - (MUSIC_NUM + 1) / 2) * MUSIC_DIV_X + MUSIC_LEFT_X2 - MUSIC_SPEAKER_ICON_SIZE - 5;
			y = (choice - (MUSIC_NUM + 1) / 2) * MUSIC_DIV_Y + MUSIC_TOP_Y - MUSIC_SPEAKER_ICON_SIZE / 3;
		}
		DrawExtendGraph(x, y, x + MUSIC_SPEAKER_ICON_SIZE, y + MUSIC_SPEAKER_ICON_SIZE, iconSpeaker, TRUE);
	}

	/*===========================*/
	//    Initialization
	/*===========================*/
	void initializeMusicInfo() {
		for (int i = 0; i < MUSIC_NUM; ++i) {
			struct MusicInfo info = infoMaster[i];
			if (MusicUnlocker::isUnlocked(info.id)) {
				infoList[i] = info;
			}
			else {
				infoList[i] = lockedMusicInfo;
			}
		}
	}

	void initializeMessage() {
		msg.initialize(MUSIC_MESSAGE_INT_X, MUSIC_MESSAGE_END_X);
		msg.setWithoutNext(defaultMsgStr, MESSAGE_WHO_DESC);
	}

	void initializeButtons() {
		for (int i = 0; i < (MUSIC_NUM + 1) / 2; ++i) {
			int x = i * MUSIC_DIV_X + MUSIC_LEFT_X;
			int y = i * MUSIC_DIV_Y + MUSIC_TOP_Y;
			buttons[i].initializeUsingLabelLen(x, y, infoList[i].name);
		}
		for (int i = (MUSIC_NUM + 1) / 2; i < MUSIC_NUM; ++i) {
			int x = (i - (MUSIC_NUM + 1) / 2) * MUSIC_DIV_X + MUSIC_LEFT_X2;
			int y = (i - (MUSIC_NUM + 1) / 2) * MUSIC_DIV_Y + MUSIC_TOP_Y;
			buttons[i].initializeUsingLabelLen(x, y, infoList[i].name);
		}
		buttons[MUSIC_MAX_INDEX].initializeUsingLabelLen(520, 340, "�^�C�g����");
		menu.set(buttons, MUSIC_NUM + 1);
		menu.setStep((MUSIC_NUM + 1) / 2);
	}

	void initializeFireFlower() {
		for (int i = 0; i < MUSIC_ROOM_FIRE_FLOWER_NUM; ++i) {
			tama[i].initialize(60.0, 600.0, 520.0, 600.0);
		}
	}
};
