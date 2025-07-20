#pragma once

#include "lib/mode.h"
#include "lib/utils/flag_store.h"
#include "lib/utils/music.h"
#include "lib/components/sprite.h"
#include "./scenario_base/state.h"
#include "./scenario_base/scene_list.h"
#include "./scenario_base/scenario_battle.h"
#include "./scenario_base/scenario_message.h"

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
const int SCENE_ACTION_STOP(10);

const int SCENE_RES_DEFAULT(-100);

// シナリオ抽象クラス
class ScenarioBase {
protected:
	int strColorDebug = GetColor(150, 0, 0);
	int strColorLoad = GetColor(0, 0, 0);

	Sprite mrK[4];
	Sprite deer;
	State state;
	SceneList sceneList;
	ScenarioMessage msg;
	ScenarioBattle battle;

public:

	ScenarioBase() {
		mrK[0].set(180, 80, "graph/sprite11.png", 1);
		mrK[1].set(420, 80, "graph/sprite12.png", 1);
		mrK[2].set(180, 260, "graph/sprite13.png", 1);
		mrK[3].set(420, 260, "graph/sprite14.png", 1);
		deer.set(300, 160, "graph/sprite15.png", 0);
		initialize();
	}

	void initialize() {
		state.initialize();
		sceneList.initialize();
		msg.initialize();
		initializeDisplay();
		battle.initialize();
	}

	int show() {
		COM dummy_com(false);
		return show(dummy_com);
	}

	int show(COM& com) {
		state.updateKeyboard();

		// 人物の描画
		deer.draw();
		drawMrKs();
		mrK[0].stop();

		showGraph();
		showBattle();

		Scene scene = sceneList.get();

		switch (scene.action) {
		case SCENE_ACTION_TALK:
			readMsg(scene.how, scene.who);
			break;
		case SCENE_ACTION_COCK:
			setTrigger(scene.how);
			break;
		case SCENE_ACTION_MOVE:
			doMove(scene.how);
			break;
		case SCENE_ACTION_EXIBIT:
			doExibit(scene.how, scene.who);
			break;
		case SCENE_ACTION_LOAD:
			Music::load(scene.how);
			goNext();
			break;
		case SCENE_ACTION_MUSIC:
			performMusic(scene.how);
			break;
		case SCENE_ACTION_GRAPH:
			performGraph(scene.how);
			break;
		case SCENE_ACTION_BATTLE:
			setBattle(scene.how);
			break;
		case SCENE_ACTION_PLAY:
			return doBattle(com);
		case SCENE_ACTION_STOP:
		default:
			break;
		}

		if (hasMsg(scene)) msg.draw();

		return SCENE_RES_DEFAULT;
	}

	void debugDump() {
		int strColor = strColorDebug;

		DrawFormatString(245, 225, strColor, "sceneFlg: %d", sceneList.getCurrentId());
		DrawFormatString(245, 325, strColor, "mrK0.vis: %d", mrK[0].visible);
		DrawFormatString(245, 345, strColor, "mrK1.vis: %d", mrK[1].visible);
		DrawFormatString(245, 365, strColor, "mrK2.vis: %d", mrK[2].visible);
		DrawFormatString(245, 385, strColor, "mrK3.vis: %d", mrK[3].visible);
		DrawFormatString(245, 405, strColor, "deer.vis: %d", deer.visible);
		DrawFormatString(245, 425, strColor, "key: %d", state.getKey());
		DrawFormatString(245, 445, strColor, "isTalking: %s", msg.isTalking() ? "true" : "false");
		msg.debugDump(strColor);
	}

protected:

	void initializeDisplay() {
		mrK[0].exhibit();
		mrK[1].exhibit();
		mrK[2].exhibit();
		mrK[3].exhibit();
		deer.hide();
	}

	void waitClick() {
		if (state.isOnReturnOrClicked()) goNext();
	}

	void goNext() {
		sceneList.goNext();
	}

	// メッセージを読む
	void readMsg(string str, int who) {
		if (msg.readNext(str, who)) goNext();

		if (state.isOnReturnOrClicked() && msg.skip()) goNext();
	}

	void doMove(const char how[]) {
		if (msg.isTalking()) {
			int who = checkMrK();
			talkMrK(who, how);
		}
		else {
			if (isTriggered()) goNext();

			int key = state.getKey();
			if (key != SPRITE_KEY_NONE) {
				mrK[0].turn(key);
				mrK[0].move();
			}

			if (state.isOnReturnOrClicked()) {
				int who = checkMrK();
				startTalkMrK(who);
				talkMrK(who, how);
			}
		}
	}

	void doExibit(string how, int who) {
		Sprite* obj = getObject(who);
		if (!obj) return;

		if (how == "exibit") {
			obj->exhibit();
			waitClick();
		}
		else if (how == "hide") {
			obj->hide();
			waitClick();
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

	void performMusic(string how) {
		if (how == "play") {
			Music::play();
		}
		else if (how == "stop") {
			Music::stop();
			Music::enableSwap();
		}
		else if (how == "pop_once") {
			Music::popOnce();
		}
		else if (how == "swap") {
			if (Music::swap(strColorLoad)) {
				Music::enableSwap();
			}
			else {
				return;
			}
		}
		else if (how == "pop") {
			Music::pop(strColorLoad);
		}
		else if (how == "unload") {
			Music::unload(1);
		}
		goNext();
	}

	void performGraph(string how) {
		if (how == "clear") {
			state.setGraph("");
			goNext();
		}
		else {
			state.setGraph(how);
			waitClick();
		}
	}

	virtual void setTrigger(string trigger) {
		if (trigger == "talk_all") {
			setTriggerAllMrK("talk");
			mrK[0].setTrigger("fired");
		}
		else if (trigger == "talk_red") {
			setTriggerAllMrK("fired");
			mrK[MESSAGE_WHO_RED - 1].setTrigger("talk");
		}
		else if (trigger == "talk_green") {
			setTriggerAllMrK("fired");
			mrK[MESSAGE_WHO_GREEN - 1].setTrigger("talk");
		}
		else if (trigger == "talk_blue") {
			setTriggerAllMrK("fired");
			mrK[MESSAGE_WHO_BLUE - 1].setTrigger("talk");
		}
		else if (trigger == "none") {
			setTriggerAllMrK("fired");
		}
		else {
			battle.setTrigger(trigger);
		}
		goNext();
	}

	void setTriggerAllMrK(string trigger) {
		for (int i = 0; i < 4; ++i) {
			mrK[i].setTrigger(trigger);
		}
	}

	virtual bool isTriggered() {
		if (battle.hasNoTrigger()) {
			for (int i = 0; i < 4; ++i) {
				if (!mrK[i].isTriggered()) {
					return false;
				}
			}
			return true;
		}

		return battle.isTriggered();
	}

	virtual void setBattle(string how) {
		if (how == "start") {
			battle.start(BATTLE_PLAYER_NONE, BATTLE_PLAYER_NONE);
		}
		else if (how == "end") {
			battle.initialize();
		}
		goNext();
	}

	virtual void showBattle() {
		battle.show();
	}

	virtual int doBattle(COM& com) {
		battle.showBefore();
		if (playTurn(com)) {
			if (battle.getTrigger() == "play_once") {
				battle.setTrigger("fired");
			}
		}
		battle.showAfter();

		if (isTriggered()) {
			goNext();
		}

		return MODE_TITLE;
	}
	
	bool playTurn(COM& com) {
		if (battle.isPlayTurn()) {
			return playByPlayer();
		}
		else {
			return playByCom(com);
		}
	}

	virtual bool playByPlayer() {
		return battle.playByPlayer();
	}

	virtual bool playByCom(COM& com) {
		return battle.playByComLevel0();
	}

	void talkMrK(int who, const char key[]) {
		Sprite* obj = getObject(who);
		if (!obj) return;

		msg.talkNext(obj, key, state.isOnReturnOrClicked());
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

	virtual void startTalkMrK(int who) {
		if (who) {
			msg.startTalk();
			if (isMrK(who)) {
				mrK[who - 1].talkReset();
			}
		}
	}

	virtual Sprite* getObject(int who) {
		if (who == MESSAGE_WHO_DEER) return &deer;
		if (isMrK(who)) return &mrK[who - 1];
		return NULL;
	}

	virtual bool hasMsg(Scene scene) {
		return (msg.isTalking() || scene.action == SCENE_ACTION_TALK);
	}
};
