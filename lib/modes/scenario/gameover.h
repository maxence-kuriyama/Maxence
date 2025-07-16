#pragma once

// �V�i���I�Ǘ��p�N���X
class GameOver : public ScenarioBase {

public:

	GameOver() : ScenarioBase() {
		imgBack = LoadGraph("graph/end_pict16.png");

		mrK[0].set(-100, -100, "graph/sprite11.png", 0);
		mrK[1].set(-100, -100, "graph/sprite12.png", 0);
		mrK[2].set(-100, -100, "graph/sprite13.png", 0);
		mrK[3].set(-100, -100, "graph/sprite14.png", 0);

		mrK[1].setSayings(sayings1);
		mrK[2].setSayings(sayings2);
		mrK[3].setSayings(sayings3);
		sceneList.initialize(scenes);

		initialize();
		msg.initialize();
	}

private:
	int mode = MESSAGE_WHO_DESC;
	int imgBack;
	unsigned int Black = GetColor(0, 0, 0);

	struct Scene scenes[MAX_SCENE_NUM] = {
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_DESC,		"unload" },
		{ SCENE_ACTION_LOAD,	MESSAGE_WHO_DESC,		"sound/bgm15.ogg" },
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_DESC,		"swap" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"�Q�[���[�I�[�o�[��ʂ���" },
		{ SCENE_ACTION_STOP,	MESSAGE_WHO_DESC,		"" },
		{ -1,					-1,						"" },
	};
	struct Saying sayings1[20] = {
		{ "10",		MESSAGE_WHO_BLUE,		"�M���͎��ɂƂ��ė��p���l�̂��鑶�݂ł�" },
		{ "10",		MESSAGE_WHO_BLUE,		"���ꂩ������񂨕t��������������" },
		{ "10",		MESSAGE_WHO_YELLOW,		"�c�c" },
		{ "999",	-1,						"" },
	};
	struct Saying sayings2[20] = {
		{ "10",		MESSAGE_WHO_RED,		"�������̓`�[���I\n�����ĉ���萳�`���I" },
		{ "10",		MESSAGE_WHO_YELLOW,		"�����A��������\n���ꂩ������" },
		{ "999",	-1,						"" },
	};
	struct Saying sayings3[20] = {
		{ "10",		MESSAGE_WHO_GREEN,	"���̕��a�̒��A���̂��߂ɐ����Ă���̂��낤" },
		{ "10",		MESSAGE_WHO_YELLOW,	"���O�͂������ꂾ��\n�l���Ă����傤���Ȃ��̂�" },
		{ "10",		MESSAGE_WHO_BLUE,		"���͂��̐��E����ɓ��ꂽ���ł��˂�" },
		{ "999",	-1,					"" },
	};

public:

	void initialize() {
		ScenarioBase::initialize();
		mode = MESSAGE_WHO_DESC;
	}

	int show() {
		// �e�̏o�͂��㏑��
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
