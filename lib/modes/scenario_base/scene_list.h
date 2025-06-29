#pragma once

#include "lib/components/message.h"

const int MAX_SCENE_NUM(400);

const int SCENE_ACTION_NONE(0);
const int SCENE_ACTION_END_OF_LIST(-1);

// シーン + 主人公のアクション
struct Scene {
	int action;
	int who;
	char how[100];
};

class SceneList {
private:
	struct Scene list[MAX_SCENE_NUM] = {
		{ SCENE_ACTION_NONE,		MESSAGE_WHO_DESC,	"" },
		{ SCENE_ACTION_END_OF_LIST,	MESSAGE_WHO_DESC,	"" },
	};

	int currentId = 0;;

public:

	SceneList() {
		initialize();
	}

	void initialize() {
		currentId = 0;
	}

	void initialize(Scene* scenes) {
		currentId = 0;
		for (int i = 0; i < MAX_SCENE_NUM; i++) {
			list[i] = scenes[i];
		}
	}

	int getCurrentId() {
		return currentId;
	}

	void goNext() {
		currentId++;
	}

	Scene get() {
		return list[currentId];
	}
};