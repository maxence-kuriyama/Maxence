#pragma once

// シナリオ管理用クラス
class GameOver : public ScenarioBase {

private:
	int mode = MESSAGE_WHO_DESC;

	struct Scene scenes[MAX_SCENE_NUM] = {
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"ゲームーオーバー画面だよ" },
		{ -1,					-1,						"" },
	};

public:

	GameOver() : ScenarioBase() {
		sceneList.initialize(scenes);
	}

	void initialize() {
		ScenarioBase::initialize();
		mode = MESSAGE_WHO_DESC;
	}

	void setMode(int who) {
		mode = who;
	}

	bool isActivated() {
		return (mode != MESSAGE_WHO_DESC);
	}
};
