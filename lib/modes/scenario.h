#pragma once

#include "lib/mode.h"
#include "lib/utils/music.h"
#include "lib/utils/encrypter.h"
#include "lib/components/menu.h"
#include "./scenario_base.h"

const int SCENE_ACTION_EQ(11);
const int SCENE_ACTION_WAIT(12);

const int MESSAGE_WHO_CARD(11);

// �V�i���I�Ǘ��p�N���X
class Scenario : public ScenarioBase {

public:
	Sprite card;

	Scenario() {
		imgRoom = LoadGraph("graph/room.bmp");
		imgCard = LoadGraph("graph/card.bmp");

		mrK[0].set(170, 30);
		mrK[0].setExpand(SPRITE_EXPAND_RATE, 72, 85);
		mrK[0].setSerialImages(16, "graph/move_test", 1);
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
		msg.initialize();
	}

private:
	int eqX = 0;
	int eqY = 0; // eq = earthquake
	int imgRoom;
	int imgCard;

	Menu menu;
	Button btnSave;
	Button btnReset;

	int strColorMenu = GetColor(255, 255, 255);
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
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,	"�Ȃɂ��I�H" },
//		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,	"Mr.K: ����AMr.K�����Ȃ����A���Ȃ񂾁I" },
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
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"�������̂��I�H\n���̉����I�I"},
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_RED,		"end" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_RED,		"pop_once" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_RED,		"sound/bgm05.ogg" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"�n���ȁc\n����Ȃ͂��ł͂Ȃ������̂Ɂc" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"�Ō�̍Ŋ��͌�����Ȃ��ƌ��߂Ă����̂ɁA����Ȏ��ɗl�Ƃ͂�" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"�����A�������˂ΐ��E�̕��󂪎~�܂�Ƃ����̂Ȃ�" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_RED,		"���̎��ɂ��Ӗ�������������Ƃ������̂���Ȃ����c�c" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_RED,		"hide" },
		{ SCENE_ACTION_WAIT,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_BLUE,		"hide_nowait" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"false" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"talk_green" },
		{ SCENE_ACTION_MOVE,	MESSAGE_WHO_DESC,		"40" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_GREEN,	"swap" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,	"���������Ă���" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_GREEN,	"start" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,	"�����[�I" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_GREEN,	"end" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_GREEN,	"pop_once" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_GREEN,	"sound/bgm06.ogg" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,	"���͌�������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,	"�c�������O�ɂ����������鎞�����邾�낤�c�c" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_GREEN,	"��ɐ����Ă����" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_GREEN,	"hide" },
		{ SCENE_ACTION_WAIT,	MESSAGE_WHO_DESC,		"" },
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
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"You win!" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_BLUE,		"end" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_BLUE,		"sound/bgm07.ogg" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_BLUE,		"stop" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"�n���ȁc\n����ȁc" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"���E�̉��ɐ����`�����X�𓦂��Ă��܂��Ȃ�āc\n�L�蓾�Ȃ�" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"�c�c����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"�����Ă������Ǝ��̂��傫�ȃ`�����X�������̂��c�c" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_BLUE,		"���܂�����" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_BLUE,		"hide" },
		{ SCENE_ACTION_WAIT,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		MESSAGE_WHO_DESC,		"false" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_DESC,		"pop_once" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_YELLOW,	"sound/bgm08.ogg" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,	"Mr.K�͉��ɂȂ낤�Ƃ��Ă����̂��c�c" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,	"���A�������I" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,	"K�Ƃ� King �̂��Ƃ������̂�" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,	"Mr.K�A�l����������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,	"�����A��͂��̐��E�ɂ���͖̂l������" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,	"�����炱�̉��ł���l���E���Đ��E���~���Ă���" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_YELLOW,	"pop" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_YELLOW,	"start" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,	"�c" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_YELLOW,	"end" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_YELLOW,	"stop" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,	"���肪�Ƃ��B" },
		{ SCENE_ACTION_STOP,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"�����ƌ��͂�����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"���ƌ��͂�����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_PLAYER,	"���̃Q�[���̉������E��łڂ��Ƃ����Ȃ�c" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_PLAYER,	"���ł���v���C���[���A���̐��E�ɋ���M�l��łڂ��̂��낤�H" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"�C�Â��Ă��܂�����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"�Ȃ�Ύ�������\n�B�A������ł��邱�Ƃ��m�F���邾����" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"�������ė��Ȃ���" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DEER,		"�c�c�c�c" },
		{ SCENE_ACTION_END_OF_LIST,	-1,					"" },
	};
	struct Saying sayings1[20] = {
		{ "10",		MESSAGE_WHO_BLUE,		"�M���͎��ɂƂ��ė��p���l�̂��鑶�݂ł�" },
		{ "10",		MESSAGE_WHO_BLUE,		"���ꂩ������񂨕t��������������" },
		{ "10",		MESSAGE_WHO_YELLOW,	"�c�c" },
		{ "20",		MESSAGE_WHO_BLUE,		"���̃J�[�h�c" },
		{ "20",		MESSAGE_WHO_BLUE,		"�{����������Ȃ����A�`�����X�����m��Ȃ���" },
		{ "20",		MESSAGE_WHO_YELLOW,	"�S�̐��A�R��Ă����" },
		{ "20",		MESSAGE_WHO_BLUE,		"�z�b�I" },
		{ "30",		MESSAGE_WHO_BLUE,		"�u�c�u�c�c\n�i����͎g����I���܂��傤�I�j" },
		{ "30",		MESSAGE_WHO_YELLOW,	"�������Ă񂾂�����" },
		{ "40",		MESSAGE_WHO_DESC,		"��: �c�c�c" },
		{ "40",		MESSAGE_WHO_DESC,		"��: �c�c�c�c�c�c�c" },
		{ "40",		MESSAGE_WHO_DESC,		"���͊����I\n�܂��b��������ȁI�I" },
		{ "50",		MESSAGE_WHO_BLUE,		"���Ȃ�1�l�ɂȂ�܂ŉB��Ă��܂���" },
		{ "50",		MESSAGE_WHO_BLUE,		"����2�l�������Ă���Ă��肪�Ƃ�\n�����Ȃ邱�Ƃ̓J�[�h�����������番�����Ă��������" },
		{ "50",		MESSAGE_WHO_BLUE,		"�����ĘA��̋M�����n�������" },
		{ "50",		MESSAGE_WHO_BLUE,		"�����͎��̐��E�ɂȂ��\n�������낤�H" },
		{ "50",		MESSAGE_WHO_BLUE,		"���͐��E��łڂ����肶��Ȃ��̂ł�" },
		{ "999",	-1,					"" },
	};
	struct Saying sayings2[20] = {
		{ "10",		MESSAGE_WHO_RED,		"�������̓`�[���I\n�����ĉ���萳�`���I" },
		{ "10",		MESSAGE_WHO_YELLOW,	"�����A��������\n���ꂩ������" },
		{ "20",		MESSAGE_WHO_RED,		"�������̃J�[�h�͂��I\n����n���ɂ��Ă���̂����I�I" },
		{ "20",		MESSAGE_WHO_RED,		"SE[sound/kick01.m4a]" },
		{ "20",		MESSAGE_WHO_RED,		"�i�������R�鉹�j" },
		{ "20",		MESSAGE_WHO_BLUE,		"�I" },
		{ "20",		MESSAGE_WHO_YELLOW,	"���킟�A����������\n�|���Ȃ�" },
		{ "30",		MESSAGE_WHO_RED,		"���E���łт�Ƃ����͖̂{���̎��̂悤����"},
		{ "30",		MESSAGE_WHO_RED,		"�������A��̉�X�̂����̒N���łڂ��Ƃ����̂��H"},
		{ "999",	-1,					"" },
	};
	struct Saying sayings3[20] = {
		{ "10",		MESSAGE_WHO_GREEN,	"���̕��a�̒��A���̂��߂ɐ����Ă���̂��낤" },
		{ "10",		MESSAGE_WHO_YELLOW,	"���O�͂������ꂾ��\n�l���Ă����傤���Ȃ��̂�" },
		{ "10",		MESSAGE_WHO_BLUE,		"���͂��̐��E����ɓ��ꂽ���ł��˂�" },
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
		card.hide();
		// �{�^��������
		btnSave.initialize(TEXT_SAVE_X, TEXT_SAVE_Y, "���f");
		btnReset.initialize(TEXT_RESET_X, TEXT_RESET_Y, "�^�C�g��");
		menu.set(btnSave, btnReset);
		mrK[0].set(170, 30);
		mrK[0].turn(SPRITE_KEY_DOWN);
	}

	int show(COM& com) {
		SetBackgroundColor(0, 0, 0);

		if (Music::drawLoadMsg()) return MODE_SCENARIO;

		// �w�i�E�l���̕`��
		DrawExtendGraph(0 + eqX, -50, 640 + eqX, 380, imgRoom, FALSE);
		card.draw(eqX);

		bool is_reset = (!battle.isOnGame() && !state.isTalking() && saveOrReset());
		
		int res = ScenarioBase::show(com);

		Scene scene = sceneList.get();
		switch (scene.action) {
		case SCENE_ACTION_EQ:
			performEQ(scene.how);
			break;
		case SCENE_ACTION_WAIT:
			waitClick();
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
	}

	void debugDump() {
		int strColor = strColorDebug;

		DrawFormatString(245, 165, strColor, "trigger: %s", battle.getTrigger());
		DrawFormatString(245, 245, strColor, "eqX: %d", eqX);

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

		mrK[0].trigger = res["mrk_trigger0"];
		mrK[1].trigger = res["mrk_trigger1"];
		mrK[2].trigger = res["mrk_trigger2"];
		mrK[3].trigger = res["mrk_trigger3"];

		remove(saveFilePath.c_str());
	}

private:

	void save() {
		Encrypter encrypter(saveFilePath);
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
		};
		encrypter.write(data);

		battle.save(saveGameFilePath);
	}

	void loadScenario(int flg_saved) {
		COM dummy_com(false);
		int new_flg = 0;
		int old_flg = 0;
		while (sceneList.getCurrentId() < flg_saved) {
			old_flg = sceneList.getCurrentId();
			Scene scene = sceneList.get();
			if (scene.action != SCENE_ACTION_MUSIC) show(dummy_com);
			new_flg = sceneList.getCurrentId();
			if (new_flg == old_flg) goNext();
		}
		state.initialize();
	}

	void loadMusic(nlohmann::json res) {
		music_name[0] = res["music_name0"];
		music_name[1] = res["music_name1"];
		Music::unloadAll();
		Music::load(music_name[0].c_str(), 0);
		Music::load(music_name[1].c_str(), 0);
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
			goNext();
		}
	}

	// override
	void setBattle(string how) {
		if (how == "start") {
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
		}
		else if (how == "end") {
			battle.initialize();
		}
		goNext();
	}

	// override
	int doBattle(COM& com) {
		ScenarioBase::doBattle(com);

		if (saveOrReset()) {
			battle.initialize();
			return MODE_TITLE;
		}

		// �ΐ�X�L�b�v�i��l�p�f�o�b�O�j
		if (FlagStore::isDebug()) {
			if (skipBattle()) goNext();
		}
			
		return MODE_SCENARIO;
	}

	// override
	// TODO: getCurrentScene()�ɉ����Ďg��COM��ς��邱��
	// com.play�̒��ŕς���ׂ����H
	bool playByCom(COM& com) {
		return battle.playByComLevel1(com);
	}

	// �f�o�b�O�p
	bool skipBattle() {
		if (UserInput::onKeySkipDebug()) {
			battle.initialize();
			return true;
		}
		return false;
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
	void talkResetMrK(int who) {
		ScenarioBase::talkResetMrK(who);

		if (who == MESSAGE_WHO_CARD) {
			state.talk();
			card.talkReset();
		}
	}

	// override
	Sprite* getObject(int who) {
		if (who == MESSAGE_WHO_CARD) return &card;
		return ScenarioBase::getObject(who);
	}
};
