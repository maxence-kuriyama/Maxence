#pragma once

#include <string>

using namespace std;

class State {
private:
	string graph = "";		// �摜�\���C�x���g�̃t���O
	bool talking = false;	// NPC�Ɖ�b�����ۂ�

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