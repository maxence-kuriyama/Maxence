#pragma once

#include <iostream>
#include <regex>
#include "lib/mode.h"
#include "lib/components/menu.h"
#include "./scenario_base.h"

using namespace std;

// チュートリアルのシナリオクラス
class Tutorial : public ScenarioBase {

public:

	Tutorial() {
		imgRoom = LoadGraph("graph/room0.png");

		sceneList.initialize(scenes);

		initialize();
	}

private:
	int imgRoom;
	Button btnReset;
	int strColorMenu = GetColor(255, 255, 255);

	int strColor = GetColor(255, 255, 255);

	struct Scene scenes[MAX_SCENE_NUM] = {
		{ SCENE_ACTION_MUSIC,	MESSAGE_WHO_DESC,		"swap" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"――世界は１つの部屋で出来ている" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"おや？君は…\nどこかで会ったような…" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"まあ、それはともかくとして" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"君はこの世界の決闘方法を知る必要がある" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"何故…？\n…それは追々わかるんじゃないかな" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"とにかくやってみようか" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_YELLOW,		"start" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"決闘はこの盤面で行われるよ\n赤い枠が縦に3個、横に3個並んでいるね" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"この赤い枠はそれぞれが3×3の小盤面だよ\n全体は9×9の81マスで構成されているんだ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"まずは一手、指してみよう\nマウスカーソルでマスを選択して、右クリックで決定だ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"play_once" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:LAST]いま君は赤い枠の「[LAST]」に置いたね" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:NEXT]すると、相手が次に置ける場所は全体の「[LAST]」の赤枠内に制限されるんだ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"重要なことを言ったからもう一回言うね" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"君が置いた石の位置によって、次に相手が置ける場所が制限されるんだ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"相手の番→君の番の場合でも、このルールは同じだよ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"ここから相手の番だから、相手がどこに置くか注目するんだ" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"play_once" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"あなたの手番" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"local_victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"終わりまで続けましょう" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"終わり" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_YELLOW,		"end" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_YELLOW,		"exibit_nowait" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"おかえり" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"つづきはまだない" },
		{ -1,					-1,						"" },
	};

public:
	void initialize() {
		ScenarioBase::initialize();

		// ボタン初期化
		btnReset.initialize(260, 440, "タイトル");

		mrK[0].set(300, 140);
		mrK[1].hide();
		mrK[2].hide();
		mrK[3].hide();
	}

	int show() {
		SetBackgroundColor(0, 0, 0);

		if (Music::drawLoadMsg()) return MODE_TUTORIAL;

		// 背景の描画
		DrawExtendGraph(0, -50, 640, 380, imgRoom, FALSE);

		Scene scene = sceneList.get();

		int res = ScenarioBase::show();
		//switch (scene.action) {
		//case SCENE_ACTION_TALK:
		//	moveForTeach(scene.how, scene.who);
		//	break;
		//default:
		//	break;
		//}

		return (res != SCENE_RES_DEFAULT) ? res : MODE_TUTORIAL;
	}

	void route(Mode& mode, int res) {
		if (res == MODE_TITLE) {
			initialize();
			Music::unloadAll();
			mode.goTitle();
		}
	}

	void debugDump() {
		int strColor = strColorDebug;
		Scene scene = sceneList.get();

		DrawFormatString(365, 405, strColor, "tutoTrigger: %s", battle.getTrigger());
		DrawFormatString(365, 425, strColor, "tutoAction: %d", scene.action);

		// game.debugDump();
	}

private:
	void readMsg(string str, int who) {
		cmatch m;
		string displayStr = str;
		int x = 300;
		int y = 140;
		stringstream lastSs;
		if (regex_match(str.c_str(), m, regex(R"(\[MOVE:(?:LAST|NEXT)\](.*))"))) {
			displayStr = m[1].str();
			Coordinate last = battle.last();
			switch (last.x) {
			case 0:
				lastSs << "左";
				break;
			case 2:
				lastSs << "右";
				break;
			}
			switch (last.y) {
			case 0:
				lastSs << "上";
				break;
			case 2:
				lastSs << "下";
				break;
			}
			string lastStr = lastSs.str();
			if (lastStr == "") lastStr = "真ん中";
			displayStr = regex_replace(displayStr, regex(R"(\[LAST\])"), lastStr);
			if (regex_match(str.c_str(), m, regex(R"(\[MOVE:LAST\].*)"))) {
				x = 100 + 100 * last.global_x + 33 * last.x;
				y = 60 + 100 * last.global_y + 33 * last.y;
			}
			else if (regex_match(str.c_str(), m, regex(R"(\[MOVE:NEXT\].*)"))) {
				x = 100 + 100 * last.x;
				y = 60 + 100 * last.y;
			}
			mrK[0].set(x, y);
			mrK[0].draw();
		}

		ScenarioBase::readMsg(displayStr, who);
	}

	// override
	void setBattle(string how) {
		if (how == "start") {
			battle.startTutorial(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_RED);
		}
		else if (how == "end") {
			battle.initialize();
		}
		goNext();
	}

	// override
	int doBattle(COM& com) {
		ScenarioBase::doBattle(com);

		if (reset()) return MODE_TITLE;

		return MODE_TUTORIAL;
	}

	bool reset() {
		btnReset.display(strColorMenu);
		if (btnReset.isClicked()) {
			battle.resetGame();
			return true;
		}
		return false;
	}
};
