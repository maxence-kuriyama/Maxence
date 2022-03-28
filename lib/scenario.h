#pragma once

#include "lib/music.h"
#include "lib/message.h"
#include "lib/sprite.h"


// シナリオ管理用クラス
// 単体での使用を想定
class Scenario {

// シーン + 主人公のアクション
struct Scene {
	int action;
	int who;
	char how[100];
};


public:
	int flg = 0;		// シナリオ管理用フラグ
	MrK mrK[4];
	MrK deer;
	Message msg;
	string text[50];
	int who[50];

	Scenario() {
		imgRoom = LoadGraph("graph/room.bmp");
		imgCard = LoadGraph("graph/card.bmp");
		mrK[0].set(160, 120);
		mrK[0].setSerialImages(16, "graph/move_test", 1);
		mrK[1].set(480, 120, "graph/sprite12.png", 1);
		mrK[2].set(160, 240, "graph/sprite13.png", 1);
		mrK[3].set(480, 240, "graph/sprite14.png", 1);
		deer.set(270, 200, "graph/sprite15.png", 0);
		initialize();
		msg.initialize();
	}

	void initialize() {
		flg = 0;
		cnt = 0;
		hasMsg = false;
		mrK[0].exhibit();
		mrK[1].exhibit();
		mrK[2].exhibit();
		mrK[3].exhibit();
		deer.hide();
		imgFront = "";
	}

	int show(Mouse& mouse, Music& music) {
		// 背景・人物の描画
		DrawExtendGraph(0 + eqX, -50, 640 + eqX, 380, imgRoom, FALSE);
		drawMrKs();
		deer.draw();
		mrK[0].stop();
		showGraph();

		Scene scene = sceneList[flg];

		switch (scene.action) {
		case SCENE_ACTION_TALK:
			readMsg(scene.how, scene.who, mouse);
			break;
		case SCENE_ACTION_MOVE:
			if (key != -1) {
				mrK[0].turn(key);
				mrK[0].move();
			}
			mrK[0].walk();
			waitClick(mouse);
			break;
		case SCENE_ACTION_EXIBIT:
			doExibit(scene.how, scene.who, mouse);
			break;
		case SCENE_ACTION_LOAD:
			music.load(scene.how);
			goNext();
			break;
		case SCENE_ACTION_MUSIC:
			performMusic(scene.how, music);
			break;
		case SCENE_ACTION_GRAPH:
			performGraph(scene.how, mouse);
			break;
		case SCENE_ACTION_EQ:
			performEQ(scene.how);
			waitClick(mouse);
			break;
		case SCENE_ACTION_BATTLE:
			return 1;
		case SCENE_ACTION_STOP:
		default:
			break;
		}

		msg.draw();
		cnt = (cnt + 1) % 10000;

		return 0;
	}

	// キーボード入力を取得する
	void getKey(Key& keyboard) {
		if (keyboard.onGoingDown()) {
			key = 0;
		}
		else if (keyboard.onGoingRight()) {
			key = 1;
		}
		else if (keyboard.onGoingUp()) {
			key = 2;
		}
		else if (keyboard.onGoingLeft()) {
			key = 3;
		}
		else {
			key = -1;
		}
	}

	void debugDump(int debug) {
		if (debug) {
			int strColor = strColorDebug;

			DrawFormatString(245, 205, strColor, "sceneFlg: %d", flg);
			DrawFormatString(245, 225, strColor, "frameCnt: %d", cnt);
			//DrawFormatString(245, 245, strColor, "textCnt: %d", textCnt);
			DrawFormatString(245, 265, strColor, "eqX: %d", eqX);
			DrawFormatString(245, 285, strColor, "textLen: %d", msg.textLen);
			DrawFormatString(245, 305, strColor, "charCnt: %d", int(msg.cnt * msg.cntPerFrame));
			DrawFormatString(245, 325, strColor, "who: %d", msg.who);
			DrawFormatString(245, 345, strColor, "mrK0.vis: %d", mrK[0].visible);
			DrawFormatString(245, 365, strColor, "mrK1.vis: %d", mrK[1].visible);
			DrawFormatString(245, 385, strColor, "mrK2.vis: %d", mrK[2].visible);
			DrawFormatString(245, 405, strColor, "mrK3.vis: %d", mrK[3].visible);
			DrawFormatString(245, 425, strColor, "deer.vis: %d", deer.visible);
			DrawFormatString(245, 445, strColor, "key: %d", key);
		}
	}


private:
	int cnt = 0;		// フレームカウンタ
	int textCnt = 0;	// テキストカウンタ
	bool hasMsg = false;	// メッセージがセットされているか
	string imgFront;	// フロントサイドに表示する画像
	int imgRoom;
	int imgCard;
	int eqX = 0;
	int eqY = 0;		// eq = earthquake
	int key = -1;		// キーボード入力 0: Up, 1: Right, 2: Down, 3: Left
	int strColorDebug = GetColor(150, 0, 0);
	int strColorLoad = GetColor(0, 0, 0);
	struct Scene sceneList[400] = {
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_DESC,		"play" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_DESC,		"sound/bgm04.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"――世界は１つの部屋で出来ている。" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_DEER,		"exibit" },
		{ SCENE_ACTION_GRAPH,	SCENE_WHO_DESC,		"card" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"「Mr.Kが世界を滅ぼす」" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_DEER,		"hide_nowait" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"hide_nowait" },
		{ SCENE_ACTION_GRAPH,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: なにっ！？" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: あれ、Mr.Kが居ないぞ、何なんだ！" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"Mr.K: 世界が滅びるというのは本当の事のようだな。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"Mr.K: しかし、一体我々のうちの誰が滅ぼすというのだ？ 貴様か？ Mr.K！！！" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_RED,		"swap" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_RED,		"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_RED,		"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_RED,		"sound/bgm05.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"Mr.K: 馬鹿な、そんなはずではなかったのに…。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"Mr.K: 最後の最期は後悔しないと決めていたのに、こんな死に様とはな。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"Mr.K: だが、俺が死ねば世界の崩壊が止まるというのなら" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"Mr.K: 俺の死にも意味を持たせられるというものじゃないか…………。" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_RED,		"hide" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: 貴様、Mr.Kを殺したな！" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: 何故いつも分かり合えないのだろう。でもそれが同じもの同士が集まった時の性なのだろう。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: だからMr.Kよ。殺し合おう。その中で生きた意味を見出そうじゃないか。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: さぁ教えてくれ。" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_GREEN,	"swap" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_GREEN,	"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_GREEN,	"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_GREEN,	"sound/bgm06.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: 俺は見えたよ。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: 残ったお前にもいつか見える時が来るだろう…………。先に逝っているよ。" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_GREEN,	"hide" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"exibit" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"Mr.K: あなた1人になるまで隠れていました。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"Mr.K: 他の2人を消してくれてありがとう。こうなることはカードを見た時から分かっていたからね。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"Mr.K: そして連戦の貴方を始末すれば、ここは私の世界になる訳だ。そうだろう？" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"Mr.K: 私は世界を滅ぼすつもりじゃないのです。貴方が邪魔なだけなのですよ。" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_BLUE,		"pop" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_BLUE,		"" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_BLUE,		"sound/bgm07.mp3" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_BLUE,		"stop" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"Mr.K: 馬鹿な、そんな、世界の王に成れるチャンスを逃してしまうなんて、有り得ない。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"Mr.K: …………ああ、生きていたこと自体が大きなチャンスだったのか…………しまったな。" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"hide" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_DESC,		"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_YELLOW,	"sound/bgm08.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: Mr.Kは王になろうとしていたのか…………。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: 王、そうか！" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: Kとは king のことだったのか。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: Mr.K。僕も見えたよ。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: さぁ、後はこの世界にいるのは僕だけだ。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: だからこの王である僕を殺して世界を救ってくれ。" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_YELLOW,	"pop" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_YELLOW,	"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_YELLOW,	"stop" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K: ありがとう。" },
		{ SCENE_ACTION_STOP,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"鹿: しかと見届けたぞ。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"鹿: 鹿と見届けたぞ。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_PLAYER,	"このゲームの王が世界を滅ぼすというなら…" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_PLAYER,	"王であるプレイヤーが、この世界に居る貴様を滅ぼすのだろう？" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"鹿: 気づいてしまったか。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"鹿: ならば鹿他無い。唯、私が上であることを確認するだけだ。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"鹿: かかって来なさい。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"鹿: …………" },
		{ -1,					-1,					"" }
	};

	// メッセージを読む
	void readMsg(string str, int who, Mouse& mouse) {
		if (!hasMsg) {
			msg.set(str, who);
			hasMsg = true;
		}
		if (mouse.click() && msg.skip()) {
			flg++;
			hasMsg = false;
		}
	}

	void doExibit(string how, int who, Mouse& mouse) {
		if (how == "exibit") {
			if (who == SCENE_WHO_DEER) {
				deer.exhibit();
			}
			else {
				mrK[who - 1].exhibit();
			}
			waitClick(mouse);
		}
		else if (how == "hide") {
			if (who == SCENE_WHO_DEER) {
				deer.hide();
			}
			else {
				mrK[who - 1].hide();
			}
			waitClick(mouse);
		}
		else if (how == "hide_nowait") {
			if (who == SCENE_WHO_DEER) {
				deer.hide();
			}
			else {
				mrK[who - 1].hide();
			}
			goNext();
		}
	}

	void performMusic(string how, Music &music) {
		if (how == "play") {
			music.play();
		}
		else if (how == "stop") {
			music.stop();
			music.enableSwap();
		}
		else if (how == "pop_once") {
			music.popOnce();
		}
		else if (how == "swap") {
			if (music.swap(strColorLoad)) {
				music.enableSwap();
			}
		}
		else if (how == "pop") {
			music.pop(strColorLoad);
		}
		goNext();
	}

	void performGraph(string how, Mouse &mouse) {
		imgFront = how;
		if (how == "clear") {
			imgFront = "";
		}
		waitClick(mouse);
	}

	void waitClick(Mouse& mouse) {
		if (mouse.click()) {
			flg++;
		}
	}

	void goNext() {
		flg++;
	}

	void performEQ(string how) {
		if (how == "true") {
			// happenEQ
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
		}
		else {
			// stopEQ
			eqX = 0;
		}
	}

	void startMusic(const char* musicName) {
		if (CheckMusic() != 1) {
			PlayMusic(musicName, DX_PLAYTYPE_BACK);
		}
	}

	void showGraph() {
		if (imgFront == "card") {
			DrawExtendGraph(0, 0, 640, 400, imgCard, FALSE);
		}
	}

	void drawMrKs() {
		// 一旦全員描画する
		for (int i = 1; i < 4; ++i) {
			mrK[i].draw(eqX);
		}
		mrK[0].draw(eqX);

		// 0番より前にいるヤツをもう一度描画
		for (int i = 1; i < 4; ++i) {
			if (mrK[i].y > mrK[0].y) mrK[i].draw(eqX);
		}
	}

};
