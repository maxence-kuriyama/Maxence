#pragma once

#include <string>

using namespace std;

class State {
private:
	string graph = "";		// 画像表示イベントのフラグ
	bool talking = false;	// NPCと会話中か否か

public:

	State() {
		initialize();
	}

	void initialize() {
		graph = "";
		talking = false;
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
};