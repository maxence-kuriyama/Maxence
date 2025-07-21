#pragma once

// 試合中アニメーションのベースクラス
class AnimeBase {
protected:
	int cnt = 0;
	bool active = false;

public:
	
	AnimeBase() {
		initialize();
	}

	void start() {
		active = true;
	}

protected:

	void initialize() {
		cnt = 0;
		active = false;
	}
};
