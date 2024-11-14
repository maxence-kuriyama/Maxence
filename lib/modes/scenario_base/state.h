#pragma once

#include <string>
#include "lib/components/sprite.h"

using namespace std;

class State {
private:

	// 入力を保存する
	struct StickyKeyboard {
		int key;		// 上下左右のみ
		bool onOk;		// Enter or Space
	};

	string graph = "";		// 画像表示イベントのフラグ
	bool talking = false;	// NPCと会話中か否か
	StickyKeyboard keyboard = { SPRITE_KEY_NONE, false };

public:

	State() {
		initialize();
	}

	void initialize() {
		graph = "";
		talking = false;
		resetKeyboard();
	}

	string getGraph() {
		return graph;
	}

	void setGraph(string src) {
		graph = src;
	}

	bool isTalking() {
		return talking;
	}

	void finishTalking() {
		talking = false;
	}

	void talk() {
		talking = true;
	}

	int getKey() {
		return keyboard.key;
	}

	void setKey(int srcKey) {
		keyboard.key = srcKey;
	}

	void pushReturn() {
		keyboard.onOk = true;
	}

	void resetKeyboard() {
		keyboard.key = SPRITE_KEY_NONE;
		keyboard.onOk = false;
	}

	bool isOnReturnOrClicked() {
		Mouse* mouse = UserInput::getMouse();
		return (keyboard.onOk || mouse->click());
	}
};