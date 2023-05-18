#pragma once

#include <regex>
#include "lib/music.h"
#include "lib/user_input.h"
#include "lib/com.h"
#include "lib/modes/common/sprite.h"
#include "lib/modes/common/message.h"
#include "lib/modes/common/game.h"


const int MAX_SCENE_NUM(400);
const double SPRITE_EXPAND_RATE(0.0006);

const int SCENE_ACTION_TALK(1);
const int SCENE_ACTION_MOVE(2);
const int SCENE_ACTION_EXIBIT(3);
const int SCENE_ACTION_LOAD(4);
const int SCENE_ACTION_MUSIC(5);
const int SCENE_ACTION_GRAPH(6);
const int SCENE_ACTION_BATTLE(7);
const int SCENE_ACTION_COCK(8);
const int SCENE_ACTION_PLAY(9);
const int SCENE_ACTION_STOP(-1);

const int SCENE_RES_DEFAULT(-100);

const int SCENE_WHO_DESC(0);
const int SCENE_WHO_YELLOW(1);
const int SCENE_WHO_BLUE(2);
const int SCENE_WHO_RED(3);
const int SCENE_WHO_GREEN(4);
const int SCENE_WHO_DEER(5);
const int SCENE_WHO_PLAYER(-1);


// シナリオ抽象クラス
class ScenarioBase {
protected:

	// シーン + 主人公のアクション
	struct Scene {
		int action;
		int who;
		char how[100];
	};

	int cnt = 0;		// フレームカウンタ
	int textCnt = 0;	// テキストカウンタ
	bool hasMsg = false;	// メッセージがセットされているか
	bool isTalking = false;	// NPCと会話中か否か
	string imgFront;	// フロントサイドに表示する画像
	int key = -1;		// キーボード入力 0: Up, 1: Right, 2: Down, 3: Left
	int onOk = 0;		// キーボード入力（Enter or Space）
	string seName;		// SEのファイル名（デバッグ用）
	int strColorDebug = GetColor(150, 0, 0);
	int strColorLoad = GetColor(0, 0, 0);

	int flg = 0; // シナリオ管理用フラグ
	MrK mrK[4];
	MrK deer;
	Message msg;
	Game game;
	bool onBattle = false;
	string battle_trigger = "";
	
	struct Scene sceneList[MAX_SCENE_NUM] = {
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"Nothing to say...\nPlease define me." },
		{ -1,					-1,					"" },
	};

public:

	ScenarioBase() {
		mrK[0].set(180, 80, "graph/sprite11.png", 1);
		mrK[1].set(420, 80, "graph/sprite12.png", 1);
		mrK[2].set(180, 260, "graph/sprite13.png", 1);
		mrK[3].set(420, 260, "graph/sprite14.png", 1);
		deer.set(300, 160, "graph/sprite15.png", 0);
		initialize();
		msg.initialize();
	}


	void initialize() {
		flg = 0;
		cnt = 0;
		hasMsg = false;
		mrK[0].exhibit();
		mrK[1].exhibit();
		mrK[2].exhibit();
		mrK[3].exhibit();
		deer.hide();
		imgFront = "";
		onOk = 0;
		isTalking = false;
		initializeBattle();
	}

	void initializeBattle() {
		onBattle = false;
		game.initialize();
		game.setVsCOM();
		battle_trigger = "";

	}

	int show(UserInput& ui, Music& music, bool debug = false) {
		getKey(*ui.key);
		Mouse mouse = *ui.mouse;

		// 人物の描画
		deer.draw();
		drawMrKs();
		mrK[0].stop();
		showGraph();

		if (onBattle) showBattle();

		Scene scene = sceneList[flg];

		switch (scene.action) {
		case SCENE_ACTION_TALK:
			readMsg(scene.how, scene.who, mouse);
			break;
		case SCENE_ACTION_COCK:
			setTrigger(scene.how);
			break;
		case SCENE_ACTION_MOVE:
			doMove(scene.how, mouse);
			break;
		case SCENE_ACTION_EXIBIT:
			doExibit(scene.how, scene.who, mouse);
			break;
		case SCENE_ACTION_LOAD:
			music.load(scene.how);
			goNext();
			break;
		case SCENE_ACTION_MUSIC:
			performMusic(scene.how, music);
			break;
		case SCENE_ACTION_GRAPH:
			performGraph(scene.how, mouse);
			break;
		case SCENE_ACTION_BATTLE:
			setBattle(scene.how);
			break;
		case SCENE_ACTION_PLAY:
			return doBattle(ui, debug);
		case SCENE_ACTION_STOP:
		default:
			break;
		}

		if (!onBattle || scene.action == SCENE_ACTION_TALK) msg.draw();
		cnt = (cnt + 1) % 10000;

		return SCENE_RES_DEFAULT;
	}

	// キーボード入力を取得する
	void getKey(Key& keyboard) {
		if (keyboard.onGoingDown()) {
			key = 0;
		}
		else if (keyboard.onGoingRight()) {
			key = 1;
		}
		else if (keyboard.onGoingUp()) {
			key = 2;
		}
		else if (keyboard.onGoingLeft()) {
			key = 3;
		}
		else {
			key = -1;
		}

		if (keyboard.onReturn()) {
			onOk = 1;
		}
		else {
			onOk = 0;
		}
	}

	void debugDump() {
		int strColor = strColorDebug;

		DrawFormatString(245, 185, strColor, "seName: %s", seName.c_str());
		DrawFormatString(245, 205, strColor, "sceneFlg: %d", flg);
		DrawFormatString(245, 225, strColor, "frameCnt: %d", cnt);
		DrawFormatString(245, 265, strColor, "textLen: %d", msg.textLen);
		DrawFormatString(245, 285, strColor, "charCnt: %d", int(msg.cnt * msg.cntPerFrame));
		DrawFormatString(245, 305, strColor, "who: %d", msg.who);
		DrawFormatString(245, 325, strColor, "mrK0.vis: %d", mrK[0].visible);
		DrawFormatString(245, 345, strColor, "mrK1.vis: %d", mrK[1].visible);
		DrawFormatString(245, 365, strColor, "mrK2.vis: %d", mrK[2].visible);
		DrawFormatString(245, 385, strColor, "mrK3.vis: %d", mrK[3].visible);
		DrawFormatString(245, 405, strColor, "deer.vis: %d", deer.visible);
		DrawFormatString(245, 425, strColor, "key: %d", key);
		DrawFormatString(245, 445, strColor, "isTalking: %s", isTalking ? "true" : "false");
		DrawFormatString(245, 465, strColor, "hasMsg: %s", hasMsg ? "true" : "false");
	}


protected:

	void waitClick(Mouse& mouse) {
		if (onOk || mouse.click()) flg++;
	}

	void goNext() {
		flg++;
	}

	Scene getCurrentScene() {
		return sceneList[flg];
	}

	// メッセージを読む
	void readMsg(string str, int who, Mouse& mouse) {
		if (!hasMsg) {
			if (str == "clear") {
				msg.set("", who, false);
				flg++;
			}
			else {
				msg.set(str, who);
				hasMsg = true;
			}
		}
		if ((onOk || mouse.click()) && msg.skip()) {
			flg++;
			hasMsg = false;
		}
	}

	void doMove(const char how[], Mouse& mouse) {
		if (isTalking) {
			int who = checkMrK();
			talkMrK(who, how, mouse);
		}
		else {
			if (key != -1) {
				mrK[0].turn(key);
				mrK[0].move();
			}
			if (onOk || mouse.click()) {
				int who = checkMrK();
				talkResetMrK(who);
			}
			if (isTriggered()) goNext();
		}
	}

	void doExibit(string how, int who, Mouse& mouse) {
		MrK* obj = getObject(who);
		if (!obj) return;

		if (how == "exibit") {
			obj->exhibit();
			waitClick(mouse);
		}
		else if (how == "hide") {
			obj->hide();
			waitClick(mouse);
		}
		else if (how == "exibit_nowait") {
			obj->exhibit();
			goNext();
		}
		else if (how == "hide_nowait") {
			obj->hide();
			goNext();
		}
	}

	void performMusic(string how, Music& music) {
		if (how == "play") {
			music.play();
		}
		else if (how == "stop") {
			music.stop();
			music.enableSwap();
		}
		else if (how == "pop_once") {
			music.popOnce();
		}
		else if (how == "swap") {
			if (music.swap(strColorLoad)) {
				music.enableSwap();
			}
			else {
				return;
			}
		}
		else if (how == "pop") {
			music.pop(strColorLoad);
		}
		else if (how == "unload") {
			music.unload(1);
		}
		goNext();
	}

	void performGraph(string how, Mouse& mouse) {
		imgFront = how;
		if (how == "clear") {
			imgFront = "";
			goNext();
		}
		else {
			waitClick(mouse);
		}
	}

	virtual void setTrigger(string trigger) {
		if (trigger == "talk_all") {
			mrK[0].setTrigger("fired");
			for (int i = 1; i < 4; ++i) {
				mrK[i].setTrigger("talk");
			}
		}
		else if (trigger == "talk_red") {
			for (int i = 0; i < 4; ++i) {
				mrK[i].setTrigger("fired");
			}
			mrK[SCENE_WHO_RED - 1].setTrigger("talk");
		}
		else if (trigger == "talk_green") {
			for (int i = 0; i < 4; ++i) {
				mrK[i].setTrigger("fired");
			}
			mrK[SCENE_WHO_GREEN - 1].setTrigger("talk");
		}
		else if (trigger == "talk_blue") {
			for (int i = 0; i < 4; ++i) {
				mrK[i].setTrigger("fired");
			}
			mrK[SCENE_WHO_BLUE - 1].setTrigger("talk");
		}
		else if (trigger == "none") {
			for (int i = 0; i < 4; ++i) {
				mrK[i].setTrigger("fired");
			}
		}
		else {
			battle_trigger = trigger;
		}
		goNext();
	}

	virtual bool isTriggered() {
		if (battle_trigger == "") {
			for (int i = 0; i < 4; ++i) {
				if (!mrK[i].isTriggered()) {
					return false;
				}
			}
			return true;
		}
		else if (battle_trigger == "play_once") {
			// do nothing here (see doGame)
		}
		else if (battle_trigger == "local_victory") {
			for (int index = 0; index < 9; index++) {
				if (game.board.localVictory(index)) {
					battle_trigger = "fired";
				}
			}
		}
		else if (battle_trigger == "victory") {
			if (game.victory() != 0) {
				battle_trigger = "fired";
			}
		}
		else if (battle_trigger == "fired") {
			battle_trigger = "";
			return true;
		}
		return false;
	}

	virtual void setBattle(string how) {
		if (how == "start") {
			game.prepare(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_RED);
			game.setVsCOM();
			onBattle = true;
		}
		else if (how == "end") {
			initializeBattle();
		}
		goNext();
	}

	virtual void showBattle() {
		game.drawBeforePlay();
		game.drawAfterPlay();
	}

	virtual int doBattle(UserInput& ui, bool debug = false) {
		game.drawBeforePlay();

		if (playTurn(ui)) {
			if (battle_trigger == "play_once") {
				battle_trigger = "fired";
			}
		}

		game.drawAfterPlay();

		// 勝利判定
		if (game.victory() != 0) {
			ui.reset();
		}

		if (isTriggered()) goNext();

		return FLAG_TITLE;
	}

	virtual bool playTurn(UserInput& ui) {
		if (game.isPlayTurn()) {
			return playByPlayer(ui);
		}
		else {
			return playByCom();
		}
	}

	bool playByPlayer(UserInput& ui) {
		if (!game.playTurn(ui)) return false;

		double res = game.update();
		return game.isUpdated(res);
	}

	bool playByCom() {
		MinMaxNode node(game.board, game.currentSide());
		int depth = 2;
		int index = node.search(depth);
		Coordinate choice = Board::coordinates(index);

		double res = game.update(choice);
		return game.isUpdated(res);
	}

	void startMusic(const char* musicName) {
		if (CheckMusic() != 1) {
			PlayMusic(musicName, DX_PLAYTYPE_BACK);
		}
	}

	void talkMrK(int who, const char key[], Mouse& mouse) {
		MrK* obj = getObject(who);
		if (!obj) return;

		// 特殊コマンド
		Saying saying = obj->talk(key);
		if (strcmp(saying.say, "") == 0 || saying.who == -1) {
			isTalking = false;
			msg.set("", SCENE_WHO_DESC, false);
			return;
		}
		else {
			std::cmatch m;
			if (regex_match(saying.say, m, std::regex(R"(SE\[(.+)\])"))) {
				seName = m[1].str(); // デバッグ用
				obj->playSE(seName);
				obj->talkNext();
				hasMsg = false;
				return;
			}
		}

		// メッセージ表示処理
		if (!hasMsg) {
			msg.set(saying.say, saying.who);
			hasMsg = true;
		}
		if ((onOk || mouse.click()) && msg.skip()) {
			obj->talkNext();
			hasMsg = false;
		}
	}

	bool isMrK(int who) {
		return (who >= 1 && who <= 4);
	}

	virtual void showGraph() {}

	virtual void drawMrKs() {
		// 一旦全員描画する
		for (int i = 1; i < 4; ++i) {
			mrK[i].draw();
		}
		mrK[0].draw();

		// 0番より前にいるヤツをもう一度描画
		for (int i = 1; i < 4; ++i) {
			if (mrK[i].y > mrK[0].y) mrK[i].draw();
		}
	}

	virtual int checkMrK() {
		// 0番の近くにいるヤツをpick up
		for (int i = 1; i < 4; ++i) {
			if (abs(mrK[i].y - mrK[0].y) < 45 && abs(mrK[i].x - mrK[0].x) < 45) {
				return i + 1;
			}
		}
		return 0;
	}

	virtual void talkResetMrK(int who) {
		if (who) {
			isTalking = true;
			if (isMrK(who)) {
				mrK[who - 1].talkReset();
			}
		}
	}

	virtual MrK* getObject(int who) {
		if (who == SCENE_WHO_DEER) return &deer;
		if (isMrK(who)) return &mrK[who - 1];
		return NULL;
	}
};
