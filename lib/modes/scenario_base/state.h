#pragma once

#include <string>

using namespace std;

class State {
private:
	string graph = "";		// 画像表示イベントのフラグ

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