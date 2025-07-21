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

	void update() {
		if (!active) return;

		_update();
		cnt++;

		if (cnt > 120) initialize();
	}

protected:

	virtual void initialize() {
		cnt = 0;
		active = false;
	}

	virtual void _update() {}
};
