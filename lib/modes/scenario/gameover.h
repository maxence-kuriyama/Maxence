#pragma once

// シナリオ管理用クラス
class GameOver : public ScenarioBase {

public:

	GameOver() : ScenarioBase() {
		imgBack = LoadGraph("graph/end_pict16.png");

		mrK[0].set(-100, -100, "graph/sprite11.png", 0);
		mrK[1].set(-100, -100, "graph/sprite12.png", 0);
		mrK[2].set(-100, -100, "graph/sprite13.png", 0);
		mrK[3].set(-100, -100, "graph/sprite14.png", 0);

		//mrK[1].setSayings(sayings1);
		//mrK[2].setSayings(sayings2);
		//mrK[3].setSayings(sayings3);
		sceneList.initialize(scenes);

		initialize();
		msg.initialize();
	}

private:
	int mode = MESSAGE_WHO_DESC;
	int imgBack;
	unsigned int Black = GetColor(0, 0, 0);

	struct Scene scenes[MAX_SCENE_NUM] = {
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"ゲームーオーバー画面だよ" },
		{ SCENE_ACTION_STOP,	MESSAGE_WHO_DESC,		"" },
		{ -1,					-1,						"" },
	};

public:

	void initialize() {
		ScenarioBase::initialize();
		mode = MESSAGE_WHO_DESC;
	}

	int show() {
		SetBackgroundColor(0, 0, 0);
		DrawBox(-1, -1, 641, 481, Black, TRUE);
		DrawExtendGraph(0, -50, 640, 380, imgBack, FALSE);

		int res = ScenarioBase::show();
		return res;
	}

	void setMode(int who) {
		mode = who;
	}

	bool isActivated() {
		return (mode != MESSAGE_WHO_DESC);
	}
};
