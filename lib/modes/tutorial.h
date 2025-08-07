#pragma once

#include <iostream>
#include <regex>
#include "lib/mode.h"
#include "lib/components/menu.h"
#include "./scenario_base.h"

using namespace std;

const int SCENE_ACTION_TUTO_WIN(21);
const int SCENE_ACTION_TUTO_LOSE(22);
const int SCENE_ACTION_TUTO_DRAW(23);

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
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"決闘はこの『盤面』で行われるよ\n赤い枠が縦に3個、横に3個並んでいるね" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"これらの赤い枠は、それぞれが3×3マスで構成された『小盤面』になっているよ\n全体は9×9の81マスで構成されているんだ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"まずは一手、指してみよう\nマウスカーソルでマスを選択して\n『右クリック』で決定だ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"play_once" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:LAST]いま君は[PREV]の枠の中の「[LAST]」に石を置いたね" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:NEXT]すると、相手が次に石を置ける場所は\n『全体の中で「[LAST]」』に位置するこの赤枠内に制限されるんだ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"重要なことを言ったからもう一回言うね" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"『君が置いた石の位置によって、次に相手が置ける場所が制限される』んだ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"相手の番→君の番の場合でも、このルールは同じだよ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"ここから相手の番だから、相手がどこに置くか注目するんだ" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"play_once" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:LAST]相手は[PREV]の枠内の「[LAST]」に置いたようだ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:NEXT]君が次に置ける場所はここ\n『全体の中で「[LAST]」』の赤枠内だよ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"じゃあ、同じ要領である程度のところまで盤面を進めてみよう\n僕はしばらく黙っているよ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"『どれかの枠の中で縦か、横か、斜めの三目を並べること』\nを目指してみたらいいんじゃないかな" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"local_victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:LAST][PREV]の枠で三目並んだようだね" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"[MOVE:LAST]決着がついた小盤面は色がつくよ\n君が勝つと赤に、相手が勝つと青に" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"そろそろ勝利条件を教えよう\n『君の色の小盤面を三目並べること』\nこれが君の勝利条件だ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"いかに相手を誘導して、君が置きたい場所に石を置けるようにするかが重要になるね" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"逆に『相手の色の小盤面を三目並べ』られると君の負けだからね\n誘導されないよう注意するんだ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"それじゃあ、勝敗が決まるまで進めてみよう\nちなみに、色のついた小盤面にさらに石を置くことはできないよ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	MESSAGE_WHO_DESC,		"victory" },
		{ SCENE_ACTION_PLAY,	MESSAGE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"…決着がついたようだね" },
		{ SCENE_ACTION_TUTO_LOSE,	MESSAGE_WHO_YELLOW,		"今回は相手の勝ちだ\n残念だったね" },
		{ SCENE_ACTION_TUTO_DRAW,	MESSAGE_WHO_YELLOW,		"おっと引き分けか\nこういうこともある" },
		{ SCENE_ACTION_TUTO_WIN,	MESSAGE_WHO_YELLOW,		"今回は君の勝ちだ\nやったね" },
		{ SCENE_ACTION_BATTLE,	MESSAGE_WHO_YELLOW,		"end" },
		{ SCENE_ACTION_EXIBIT,	MESSAGE_WHO_YELLOW,		"exibit_nowait" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"この世界の決闘方法はだいたいわかったかな\nそれじゃあ健闘を祈っているよ" },
		{ SCENE_ACTION_TALK,	MESSAGE_WHO_YELLOW,		"また会おう" },
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
		switch (scene.action) {
		case SCENE_ACTION_TUTO_WIN:
		case SCENE_ACTION_TUTO_LOSE:
		case SCENE_ACTION_TUTO_DRAW:
			readMsgAfterBattle(scene.action, scene.how, scene.who);
			break;
		default:
			break;
		}

		int res = ScenarioBase::show();

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
		if (regex_match(str.c_str(), m, regex(R"(\[MOVE:(?:LAST|NEXT)\]([\s\S]*))"))) {
			int x = 300;
			int y = 140;
			Coordinate last = battle.last();
			displayStr = replaceLastToken(m[1].str(), last);
			displayStr = replacePrevtToken(displayStr, last);
			if (regex_match(str.c_str(), m, regex(R"(\[MOVE:LAST\][\s\S]*)"))) {
				x = 100 + 100 * last.global_x + 33 * last.x;
				y = 60 + 100 * last.global_y + 33 * last.y;
			}
			else if (regex_match(str.c_str(), m, regex(R"(\[MOVE:NEXT\][\s\S]*)"))) {
				x = 100 + 100 * last.x;
				y = 60 + 100 * last.y;
			}
			mrK[0].set(x, y);
			mrK[0].draw();
		}
		else if (regex_match(str.c_str(), m, regex(R"([\s\S]*\[VICT\][\s\S]*)"))) {
			displayStr = replaceVictToken(str);
		}

		ScenarioBase::readMsg(displayStr, who);
	}

	void readMsgAfterBattle(int action, string str, int who) {
		if ((action == SCENE_ACTION_TUTO_WIN && !battle.isWon()) 
			|| (action == SCENE_ACTION_TUTO_LOSE && !battle.isLost())
			|| (action == SCENE_ACTION_TUTO_DRAW && !battle.isDraw())) {
			goNext();
			return;
		}
		readMsg(str, who);
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

	// override
	bool hasMsg(Scene scene) {
		if (ScenarioBase::hasMsg(scene)) return true;

		return (scene.action == SCENE_ACTION_TUTO_WIN
			|| scene.action == SCENE_ACTION_TUTO_LOSE
			|| scene.action == SCENE_ACTION_TUTO_DRAW);
	}

	string getPositionStr(int x, int y) {
		stringstream lastSs;
		switch (x) {
		case 0:
			lastSs << "左";
			break;
		case 2:
			lastSs << "右";
			break;
		}
		switch (y) {
		case 0:
			lastSs << "上";
			break;
		case 2:
			lastSs << "下";
			break;
		}
		return (lastSs.str() == "") ? "真ん中" : lastSs.str();
	}

	string replaceLastToken(string srcStr, Coordinate last) {
		string lastStr = getPositionStr(last.x, last.y);
		return regex_replace(srcStr, regex(R"(\[LAST\])"), lastStr);
	}

	string replacePrevtToken(string srcStr, Coordinate last) {
		string prevStr = getPositionStr(last.global_x, last.global_y);
		return regex_replace(srcStr, regex(R"(\[PREV\])"), prevStr);
	}

	string replaceVictToken(string srcStr) {
		string displayStr = srcStr;
		if (battle.isLost()) {
			displayStr = regex_replace(displayStr, regex(R"(\[VICT\])"), "相手");
			displayStr = regex_replace(displayStr, regex(R"(\[MSG:VICT\])"), "残念だったね");
		}
		else {
			displayStr = regex_replace(displayStr, regex(R"(\[VICT\])"), "君");
			displayStr = regex_replace(displayStr, regex(R"(\[MSG:VICT\])"), "やったね");
		}
		return displayStr;
	}
};
