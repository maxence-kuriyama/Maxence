#pragma once

#include <string>

using namespace std;

class State {
private:
	string graph = "";		// �摜�\���C�x���g�̃t���O

public:

	State() {
		initialize();
	}

	void initialize() {
		graph = "";
	}

	string getGraph() {
		return graph;
	}

	void setGraph(string src) {
		graph = src;
	}
};