#pragma once

#include <sstream>
#include "lib/mode.h"
#include "lib/utils/encrypter.h"
#include "lib/components/menu.h"
#include "./scenario_base.h"
#include "./scenario/gameover.h"

const int SCENE_ACTION_EQ(11);
const int SCENE_ACTION_WAIT(12);
const int SCENE_ACTION_ENDING(13);
const int SCENE_ACTION_LOSE(14);

const int SCENARIO_ID_BEFORE_RED(17);
const int SCENARIO_ID_AFTER_RED(25);
const int SCENARIO_ID_BEFORE_GREEN(37);
const int SCENARIO_ID_AFTER_GREEN(45);
const int SCENARIO_ID_BEFORE_BLUE(56);
const int SCENARIO_ID_AFTER_BLUE(64);
const int SCENARIO_ID_BEFORE_YELLOW(79);
const int SCENARIO_MAX_EQ_CNT(4);

const int MESSAGE_WHO_CARD(11);

const string SCENARIO_MSG_LOST_BATTLE("�s�k���Ă��܂����c\n�ڂ̑O���Â��Ȃ�c");

// �V�i���I�Ǘ��p�N���X
class Scenario : public ScenarioBase {

public:
	Sprite card;

	Scenario() {
		imgRoom = LoadGraph("graph/room0.png");
		imgCard = LoadGraph("graph/card.bmp");

		mrK[0].set(170, 30);
		mrK[0].setExpand(SPRITE_EXPAND_RATE, 72, 85);
		mrK[0].setSerialImages(16, "graph/move", 1);
		mrK[1].set(320, 80, "graph/sprite12.png", 1);
		mrK[2].set(200, 230, "graph/sprite13.png", 1);
		mrK[3].set(500, 260, "graph/sprite14.png", 1);
		deer.set(300, 160, "graph/sprite15.png", 0);
		card.set(230, 160, "graph/sprite16.png", 0);

		mrK[1].setSayings(sayings1);
		mrK[2].setSayings(sayings2);
		mrK[3].setSayings(sayings3);
		card.setSayings(sayingsCard);
		sceneList.initialize(scenes);

		initialize();

		// �g�p����SE����x�������ɏ悹�Ă���
		SetUseASyncLoadFlag(TRUE);
		seHandleKick = LoadSoundMem("sound/kick01.m4a");
		SetUseASyncLoadFlag(FALSE);
	}

	~Scenario() {
		DeleteSoundMem(seHandleKick);
		DeleteGraph(imgRoom);
		DeleteGraph(imgCard);
	}

private:
	int eqCnt = 0;
	int eqX = 0;
	int eqY = 0; // eq = earthquake
	int endingCnt = 0;
	int numContinue = 0;

	int imgRoom;
	int imgCard;
	int seHandleKick = 0;
	unsigned int White = GetColor(255, 255, 255);
	unsigned int Black = GetColor(0, 0, 0);
	int strColorDebug = GetColor(100, 30, 30);

	Menu menu;
	Button btnSave;
	Button btnReset;
	int strColorMenu = GetColor(255, 255, 255);
	GameOver gameover;

	string music_name[2] = { "", "" };
	string saveFilePath = SAVE_FILE_SCENARIO;
	string saveGameFilePath = SAVE_FILE_SCENARIO_GAME;

	struct Scene scenes[MAX_SCENE_NUM] = {
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_DESC,		"play" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_DESC,		"unload" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_DESC,		"sound/bgm04.ogg" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"�\�\���E�͂P�̕����ŏo���Ă���" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"talk_all" },
		{ SCENE_ACTION_MOVE,	MESSAGE_WHO_DESC,		"10" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_CARD,		"exibit_nowait" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_DEER,		"exibit" },
		{ SCENE_ACTION_GRAPH,	MESSAGE_WHO_DESC,		"card" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"�uMr.K�����E��łڂ��v" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_DEER,		"hide_nowait" },
		{ SCENE_ACTION_GRAPH,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�Ȃɂ��I�H" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"talk_all" },
		{ SCENE_ACTION_MOVE,	MESSAGE_WHO_DESC,		"20" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"false" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"talk_red" },
		{ SCENE_ACTION_MOVE,	MESSAGE_WHO_DESC,		"30" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_RED,		"swap" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"�M�l���I�H\nMr.K�I�I�I"},
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_RED,		"start" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_LOSE,	MESSAGE_WHO_RED,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"�������̂��I�H\n���̉����I�I"},
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_RED,		"end" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_RED,		"pop_once" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_RED,		"sound/bgm05.ogg" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"�n���ȁc\n����Ȃ͂��ł͂Ȃ������̂Ɂc" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"�Ō�̍Ŋ��͌�����Ȃ��ƌ��߂Ă����̂ɁA����Ȏ��ɗl�Ƃ͂�" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"�����A�������˂ΐ��E�̕��󂪎~�܂�Ƃ����̂Ȃ�" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"���̎��ɂ��Ӗ�������������Ƃ������̂���Ȃ����c�c" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_RED,		"hide" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_BLUE,		"hide_nowait" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"false" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"talk_green" },
		{ SCENE_ACTION_MOVE,	MESSAGE_WHO_DESC,		"40" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_GREEN,		"swap" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,		"���������Ă���" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_GREEN,		"start" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_LOSE,	MESSAGE_WHO_GREEN,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,		"�������c\n�����������̂��c" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_GREEN,		"end" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_GREEN,		"pop_once" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_GREEN,		"sound/bgm06.ogg" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,		"���͌�������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,		"�c�������O�ɂ����������鎞�����邾�낤�c�c" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,		"��ɐ����Ă����" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_GREEN,		"hide" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"false" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_BLUE,		"exibit_nowait" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"talk_blue" },
		{ SCENE_ACTION_MOVE,	MESSAGE_WHO_DESC,		"50" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_BLUE,		"pop" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"�M�����ז��Ȃ����Ȃ̂ł���" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_BLUE,		"start" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_LOSE,	MESSAGE_WHO_BLUE,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"You win!!\n�M���̏����ł��I" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_BLUE,		"end" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_BLUE,		"sound/bgm07.ogg" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_BLUE,		"stop" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"�n���ȁc\n����ȁc" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"���E�̉��ɐ����`�����X�𓦂��Ă��܂��Ȃ�āc\n�L�蓾�Ȃ�" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"�c�c����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"�����Ă������Ǝ��̂��傫�ȃ`�����X�������̂��c�c" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"���܂�����" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_BLUE,		"hide" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"false" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_DESC,		"pop_once" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_YELLOW,		"sound/bgm08.ogg" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"Mr.K�͉��ɂȂ낤�Ƃ��Ă����̂��c�c" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"���A�������I" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"K�Ƃ� King �̂��Ƃ������̂�" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"Mr.K�A�l����������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�����A��͂��̐��E�ɂ���͖̂l������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�����炱�̉��ł���l���E���Đ��E���~���Ă���" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"�N�̎�ŁI" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_YELLOW,		"pop" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_YELLOW,		"start" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_LOSE,	MESSAGE_WHO_YELLOW,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"���肪�Ƃ��B" },
		{ SCENE_ACTION_ENDING,	MESSAGE_WHO_DESC,		"prepare" },
		{ SCENE_ACTION_ENDING,	MESSAGE_WHO_DESC,		"go" },
//		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_YELLOW,		"end" },
//		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_YELLOW,		"stop" },
//		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"���肪�Ƃ��B" },
		{ SCENE_ACTION_STOP,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"�����ƌ��͂�����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"���ƌ��͂�����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_PLAYER,		"���̃Q�[���̉������E��łڂ��Ƃ����Ȃ�c" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_PLAYER,		"���ł���v���C���[���A���̐��E�ɋ���M�l��łڂ��̂��낤�H" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"�C�Â��Ă��܂�����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"�Ȃ�Ύ�������\n�B�A������ł��邱�Ƃ��m�F���邾����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"�������ė��Ȃ���" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"�c�c�c�c" },
		{ SCENE_ACTION_END_OF_LIST,	-1,					"" },
	};
	struct Saying sayings1[20] = {
		{ "10",		MESSAGE_WHO_BLUE,		"�M���͎��ɂƂ��ė��p���l�̂��鑶�݂ł�" },
		{ "10",		MESSAGE_WHO_BLUE,		"���ꂩ������񂨕t��������������" },
		{ "10",		MESSAGE_WHO_YELLOW,		"�c�c" },
		{ "20",		MESSAGE_WHO_BLUE,		"���̃J�[�h�c" },
		{ "20",		MESSAGE_WHO_BLUE,		"�{����������Ȃ����A�`�����X�����m��Ȃ���" },
		{ "20",		MESSAGE_WHO_YELLOW,		"�S�̐��A�R��Ă����" },
		{ "20",		MESSAGE_WHO_BLUE,		"�z�b�I" },
		{ "30",		MESSAGE_WHO_BLUE,		"�u�c�u�c�c\n�i����͎g����I���܂��傤�I�j" },
		{ "30",		MESSAGE_WHO_YELLOW,		"�������Ă񂾂�����" },
		{ "40",		MESSAGE_WHO_DESC,		"��: �c�c�c" },
		{ "40",		MESSAGE_WHO_DESC,		"��: �c�c�c�c�c�c�c" },
		{ "40",		MESSAGE_WHO_DESC,		"���͊����I\n�܂��b��������ȁI�I" },
		{ "50",		MESSAGE_WHO_BLUE,		"���Ȃ�1�l�ɂȂ�܂ŉB��Ă��܂���" },
		{ "50",		MESSAGE_WHO_BLUE,		"����2�l�������Ă���Ă��肪�Ƃ�\n�����Ȃ邱�Ƃ̓J�[�h�����������番�����Ă��������" },
		{ "50",		MESSAGE_WHO_BLUE,		"�����ĘA��̋M�����n�������" },
		{ "50",		MESSAGE_WHO_BLUE,		"�����͎��̐��E�ɂȂ��\n�������낤�H" },
		{ "50",		MESSAGE_WHO_BLUE,		"���͐��E��łڂ����肶��Ȃ��̂ł�" },
		{ "999",	-1,						"" },
	};
	struct Saying sayings2[20] = {
		{ "10",		MESSAGE_WHO_RED,		"�������̓`�[���I\n�����ĉ���萳�`���I" },
		{ "10",		MESSAGE_WHO_YELLOW,		"�����A��������\n���ꂩ������" },
		{ "20",		MESSAGE_WHO_RED,		"�������̃J�[�h�͂��I\n����n���ɂ��Ă���̂����I�I" },
		{ "20",		MESSAGE_WHO_RED,		"SE[sound/kick01.m4a]" },
		{ "20",		MESSAGE_WHO_RED,		"�i�������R�鉹�j" },
		{ "20",		MESSAGE_WHO_BLUE,		"�I" },
		{ "20",		MESSAGE_WHO_YELLOW,		"���킟�A����������\n�|���Ȃ�" },
		{ "30",		MESSAGE_WHO_RED,		"���E���łт�Ƃ����͖̂{���̎��̂悤����"},
		{ "30",		MESSAGE_WHO_RED,		"�������A��̉�X�̂����̒N���łڂ��Ƃ����̂��H"},
		{ "999",	-1,						"" },
	};
	struct Saying sayings3[20] = {
		{ "10",		MESSAGE_WHO_GREEN,	"���̕��a�̒��A���̂��߂ɐ����Ă���̂��낤" },
		{ "10",		MESSAGE_WHO_YELLOW,	"���O�͂������ꂾ��\n�l���Ă����傤���Ȃ��̂�" },
		{ "10",		MESSAGE_WHO_BLUE,	"���͂��̐��E����ɓ��ꂽ���ł��˂�" },
		{ "20",		MESSAGE_WHO_GREEN,	"�����������o�����̂ł��ˁc\n�M���͂ǂ����܂����H" },
		{ "20",		MESSAGE_WHO_YELLOW,	"�������ˑR���ꂽ�z�����҂����C�ɂȂ�Ȃ�" },
		{ "20",		MESSAGE_WHO_GREEN,	"���������ł�\n�z�̈Ӑ}�͓ǂ߂܂����" },
		{ "30",		MESSAGE_WHO_GREEN,	"���̒n�k�Ɛ���̃J�[�h�̊֌W�͂���̂ł��傤���H" },
		{ "30",		MESSAGE_WHO_YELLOW,	"�ł����̂܂܂ł͐��E���ۂ��Ȃ���" },
		{ "30",		MESSAGE_WHO_GREEN,	"�����ł���\n���낻��ł��˂��c" },
		{ "40",		MESSAGE_WHO_GREEN,	"�M�l�AMr.K���E�����ȁI" },
		{ "40",		MESSAGE_WHO_GREEN,	"���̂��������荇���Ȃ��̂��낤" },
		{ "40",		MESSAGE_WHO_GREEN,	"�ł��A���ꂪ�������̓��m���W�܂������̐��Ȃ̂��낤�c" },
		{ "40",		MESSAGE_WHO_GREEN,	"������Mr.K��A�E��������\n���̒��Ő������Ӗ������o��������Ȃ���" },
		{ "999",	-1,					"" },
	};
	struct Saying sayingsCard[5] = {
		{ "20",		MESSAGE_WHO_DESC,	"�uMr.K�����E��łڂ��v" },
		{ "30",		MESSAGE_WHO_DESC,	"�uMr.K�����E��łڂ��v" },
		{ "40",		MESSAGE_WHO_DESC,	"�uMr.K�����E��łڂ��v" },
		{ "50",		MESSAGE_WHO_DESC,	"�uMr.K�����E��łڂ��v" },
		{ "999",	-1,					"" },
	};

public:

	void initialize() {
		ScenarioBase::initialize();
		gameover.initialize();
		card.hide();

		// �{�^��������
		btnSave.initialize(TEXT_SAVE_X, TEXT_SAVE_Y, "���f");
		btnReset.initialize(TEXT_RESET_X, TEXT_RESET_Y, "�^�C�g��");
		menu.set(btnSave, btnReset);

		mrK[0].set(170, 30);
		mrK[0].turn(SPRITE_KEY_DOWN);
		endingCnt = 0;
		numContinue = 0;
	}

	int show() {
		SetBackgroundColor(0, 0, 0);

		if (Music::drawLoadMsg()) return MODE_SCENARIO;

		// �w�i�E�l���̕`��
		DrawExtendGraph(0 + eqX, -50, 640 + eqX, 380, imgRoom, FALSE);
		card.draw(eqX);

		Scene scene = sceneList.get();

		bool is_reset = (!battle.isOnGame() && !hasMsg(scene) && saveOrReset());
		
		int res = ScenarioBase::show();
		switch (scene.action) {
		case SCENE_ACTION_EQ:
			performEQ(scene.how);
			break;
		case SCENE_ACTION_WAIT:
			waitClick();
			break;
		case SCENE_ACTION_ENDING:
			res = prepareEnding(scene.how);
			break;
		case SCENE_ACTION_LOSE:
			res = lostBattle(scene.who);
			break;
		default:
			break;
		}

		music_name[0] = Music::getMusicName(0);
		music_name[1] = Music::getMusicName(1);

		if (is_reset) {
			return MODE_TITLE;
		}
		else if (res != SCENE_RES_DEFAULT) {
			return res;
		}
		else {
			return MODE_SCENARIO;
		}
	}

	void route(Mode& mode, int res) {
		if (res == MODE_TITLE) {
			Music::unloadAll();
			mode.goTitle();
			initialize();
		}
		else if (res == MODE_ENDING) {
			Music::unloadAll();
			mode.goEnding();
			initialize();
		}
	}

	void debugDump() {
		int strColor = strColorDebug;

		DrawFormatString(245, 165, strColor, "trigger: %s", battle.getTrigger());
		DrawFormatString(245, 185, strColor, "eqX: %d", eqX);
		DrawFormatString(245, 205, strColor, "continue: %d", numContinue);

		ScenarioBase::debugDump();
	}

	/*===========================*/
	//    Save and Load
	/*===========================*/
	bool hasSaveFile() {
		ifstream file(saveFilePath);
		if (file) {
			file.close();
			return true;
		}
		else {
			file.close();
			return false;
		}
	}

	void load() {
		Encrypter encrypter(saveFilePath);
		nlohmann::json res = encrypter.read();
		Logger::ss << "Scenario loaded: " << res.dump();
		Logger::log();

		initialize();

		loadMusic(res);
		loadScenario(res["flg"]);
		battle.load(res, saveGameFilePath);

		numContinue = res["numContinue"];
		mrK[0].trigger = res["mrk_trigger0"];
		mrK[1].trigger = res["mrk_trigger1"];
		mrK[2].trigger = res["mrk_trigger2"];
		mrK[3].trigger = res["mrk_trigger3"];

		remove(saveFilePath.c_str());
	}

private:

	void save() {
		nlohmann::json content = build_save_content();
		save(content);
	}

	void save(nlohmann::json content) {
		Encrypter encrypter(saveFilePath);
		encrypter.write(content);

		battle.save(saveGameFilePath);
	}

	nlohmann::json build_save_content() {
		nlohmann::json data = {
			{"flg", sceneList.getCurrentId()},
			{"onBattle", battle.isOnGame()},
			{"battle_trigger", battle.getTrigger()},
			{"mrk_trigger0", mrK[0].trigger},
			{"mrk_trigger1", mrK[1].trigger},
			{"mrk_trigger2", mrK[2].trigger},
			{"mrk_trigger3", mrK[3].trigger},
			{"music_name0", music_name[0]},
			{"music_name1", music_name[1]},
			{"numContinue", numContinue},
		};
		return data;
	}

	nlohmann::json build_save_content_for_lost() {
		nlohmann::json data = {
			{"flg", getContinueId()},
			{"onBattle", false},
			{"battle_trigger", battle.getTrigger()},
			{"mrk_trigger0", SPRITE_TRIGGER_FIRED},
			{"mrk_trigger1", SPRITE_TRIGGER_FIRED},
			{"mrk_trigger2", SPRITE_TRIGGER_FIRED},
			{"mrk_trigger3", SPRITE_TRIGGER_FIRED},
			{"music_name0", getContinueMusic()},
			{"music_name1", music_name[0]},
			{"numContinue", numContinue + 1},
		};
		return data;
	}

	int getContinueId() {
		int currentId = sceneList.getCurrentId();
		if (currentId < SCENARIO_ID_AFTER_RED) return SCENARIO_ID_BEFORE_RED;
		if (currentId < SCENARIO_ID_AFTER_GREEN) return SCENARIO_ID_BEFORE_GREEN;
		if (currentId < SCENARIO_ID_AFTER_BLUE) return SCENARIO_ID_BEFORE_BLUE;
		return SCENARIO_ID_BEFORE_YELLOW;
	}

	string getContinueMusic() {
		int currentId = sceneList.getCurrentId();
		if (currentId < SCENARIO_ID_AFTER_BLUE) return "sound/bgm03.ogg";
		return "sound/bgm07.ogg";
	}

	void loadScenario(int flg_saved) {
		int new_flg = 0;
		int old_flg = 0;
		while (sceneList.getCurrentId() < flg_saved) {
			old_flg = sceneList.getCurrentId();
			Scene scene = sceneList.get();
			// if (scene.action != SCENE_ACTION_MUSIC) show(dummy_com);
			if (scene.action != SCENE_ACTION_MUSIC) show();
			new_flg = sceneList.getCurrentId();
			if (new_flg == old_flg) goNext();
		}
		msg.finishTalk();
		state.initialize();
	}

	void loadMusic(nlohmann::json res) {
		music_name[0] = res["music_name0"];
		music_name[1] = res["music_name1"];
		Music::unloadAll();
		Music::loadSync(music_name[0].c_str());
		Music::loadSync(music_name[1].c_str());
		Music::enableSwap();
		Music::play();
	}

	void performEQ(string how) {
		if (how == "true") {
			// happenEQ
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
			waitClick();
		}
		else {
			// stopEQ
			eqX = 0;
			// �w�i�����ւ�
			eqCnt++;
			updateImgRoom();
			goNext();
		}
	}

	void updateImgRoom() {
		if (eqCnt > SCENARIO_MAX_EQ_CNT) eqCnt = SCENARIO_MAX_EQ_CNT;

		ostringstream ss;
		ss << "./graph/room" << eqCnt << ".png";
		string filename = ss.str();

		DeleteGraph(imgRoom);
		imgRoom = LoadGraph(filename.c_str());
	}

	int prepareEnding(string how) {
		if (how == "go") {
			DrawBox(-1, -1, 641, 481, Black, TRUE);
			return MODE_ENDING;
		}

		endingCnt++;
		if (how == "prepare") {
			if (endingCnt > 160.0) {
				goNext();
			}

			// fadeout
			double fade = min(255.0, 0.01 * pow(max(0.0, endingCnt), 2.0));
			SetDrawBlendMode(DX_BLENDMODE_SUB, fade);
			DrawBox(-1, -1, 641, 481, White, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// fadeoutMusic
			Music::changeVolume(7.57 * pow(160.0 - endingCnt, 0.6));
			Music::play();
		}
		return SCENE_RES_DEFAULT;
	}

	// override
	void setBattle(string how) {
		if (how == "start") {
			enemyCom = new Enemy();
			Scene scene = sceneList.get();
			if (scene.who == MESSAGE_WHO_RED) {
				battle.start(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_RED);
			}
			else if (scene.who == MESSAGE_WHO_GREEN) {
				battle.start(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_GREEN);
			}
			else if (scene.who == MESSAGE_WHO_BLUE) {
				battle.start(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_BLUE);
			}
			else if (scene.who == MESSAGE_WHO_YELLOW) {
				battle.start(BATTLE_PLAYER_PLAYER, BATTLE_PLAYER_YELLOW);
			}
			else {
				battle.start(BATTLE_PLAYER_NONE, BATTLE_PLAYER_NONE);
			}
			goNext();
			return;
		}
		return ScenarioBase::setBattle(how);
	}

	// override
	int doBattle() {
		ScenarioBase::doBattle();

		if (saveOrReset()) {
			battle.initialize();
			return MODE_TITLE;
		}

		// �ΐ�X�L�b�v�i��l�p�f�o�b�O�j
		if (FlagStore::isDebug() && UserInput::onKeySkipDebug()) {
			goNext();
		}
			
		return MODE_SCENARIO;
	}

	// override
	bool playByCom() {
		if (FlagStore::isDebug()) return playByPlayer();

		return ScenarioBase::playByCom();
	}

	int lostBattle(int who) {
		if (battle.isLost()) {
			if (gameover.isActivated()) {
				int res = gameover.show();
				if (gameover.goingContinue()) load();
				return res;
			}

			msg.readNext(SCENARIO_MSG_LOST_BATTLE, MESSAGE_WHO_DESC);
			if (state.isOnReturnOrClicked() && msg.skip()) {
				nlohmann::json content = build_save_content_for_lost();
				save(content);

				gameover.activate(who);
			}
		}
		else {
			goNext();
		}
		return MODE_SCENARIO;
	}

	bool saveOrReset() {
		bool no_keyboard = true;
		int choice = menu.choose(strColorMenu, no_keyboard);

		// save
		if (choice == 0) save();

		//reset
		if (choice == 0 || choice == 1) {
			battle.resetGame();
			return true;
		}

		return false;
	}

	// override
	void showGraph() {
		if (state.getGraph() == "card") {
			DrawExtendGraph(0, 0, 640, 400, imgCard, FALSE);
		}
	}

	// override
	void drawMrKs() {
		// ��U�S���`�悷��
		for (int i = 1; i < 4; ++i) {
			mrK[i].draw(eqX);
		}
		mrK[0].draw(eqX);

		// 0�Ԃ��O�ɂ��郄�c��������x�`��
		for (int i = 1; i < 4; ++i) {
			if (mrK[i].y > mrK[0].y) mrK[i].draw(eqX);
		}
	}

	// override
	int checkMrK() {
		int nearest = ScenarioBase::checkMrK();
		if (nearest != 0) return nearest;

		if (abs(card.y - mrK[0].y) < 45 && abs(card.x - mrK[0].x) < 45) {
			return MESSAGE_WHO_CARD;
		}
		return 0;
	}

	// override
	void startTalkMrK(int who) {
		ScenarioBase::startTalkMrK(who);

		if (who == MESSAGE_WHO_CARD) {
			msg.startTalk();
			card.talkReset();
		}
	}

	// override
	Sprite* getObject(int who) {
		if (who == MESSAGE_WHO_CARD) return &card;
		return ScenarioBase::getObject(who);
	}

	// override
	bool hasMsg(Scene scene) {
		if (ScenarioBase::hasMsg(scene)) return true;
		
		return (scene.action == SCENE_ACTION_LOSE);
	}
};
