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

	Scenario() {
		imgRoom = LoadGraph("graph/room.bmp");
		imgCard = LoadGraph("graph/card.bmp");
		mrK[0].set(160, 120);
		mrK[0].setSerialImages(16, "graph/move_test", 1);
		mrK[1].set(480, 120, "graph/sprite12.png", 1);
		mrK[2].set(160, 240, "graph/sprite13.png", 1);
		mrK[3].set(480, 240, "graph/sprite14.png", 1);
		mrK[1].setSayings(sayings1);
		mrK[2].setSayings(sayings2);
		mrK[3].setSayings(sayings3);
		deer.set(270, 200, "graph/sprite15.png", 0);
		initialize();
		msg.initialize();
	}

private:
	int cnt = 0;		// フレームカウンタ
	int textCnt = 0;	// テキストカウンタ
	bool hasMsg = false;	// メッセージがセットされているか
	bool isTalking = false;	// NPCと会話中か否か
	string imgFront;	// フロントサイドに表示する画像
	int imgRoom;
	int imgCard;
	int eqX = 0;
	int eqY = 0;		// eq = earthquake
	int key = -1;		// キーボード入力 0: Up, 1: Right, 2: Down, 3: Left
	int onOk = 0;		// キーボード入力（Enter or Space）
	int strColorDebug = GetColor(150, 0, 0);
	int strColorLoad = GetColor(0, 0, 0);
	struct Scene sceneList[400] = {
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_DESC,		"play" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_DESC,		"sound/bgm04.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"――世界は１つの部屋で出来ている。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_all" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"10" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_DEER,		"exibit" },
		{ SCENE_ACTION_GRAPH,	SCENE_WHO_DESC,		"card" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"「Mr.Kが世界を滅ぼす」" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_DEER,		"hide_nowait" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_GRAPH,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"なにっ！？" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		//{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"Mr.K: あれ、Mr.Kが居ないぞ、何なんだ！" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_all" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"20" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_red" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"30" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_RED,		"swap" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_RED,		"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_RED,		"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_RED,		"sound/bgm05.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"馬鹿な、そんなはずではなかったのに…。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"最後の最期は後悔しないと決めていたのに、こんな死に様とはな。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"だが、俺が死ねば世界の崩壊が止まるというのなら" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_RED,		"俺の死にも意味を持たせられるというものじゃないか……。" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_RED,		"hide" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"hide_nowait" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_green" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"40" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_GREEN,	"swap" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_GREEN,	"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_GREEN,	"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_GREEN,	"sound/bgm06.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"俺は見えたよ。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"残ったお前にもいつか見える時が来るだろう……。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_GREEN,	"先に逝っているよ。" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_GREEN,	"hide" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"exibit_nowait" },
		{ SCENE_ACTION_COCK,	SCENE_WHO_DESC,		"talk_blue" },
		{ SCENE_ACTION_MOVE,	SCENE_WHO_DESC,		"50" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_BLUE,		"pop" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_BLUE,		"" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_BLUE,		"sound/bgm07.mp3" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_BLUE,		"stop" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"馬鹿な、そんな、世界の王に成れるチャンスを逃してしまうなんて、有り得ない。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"……ああ、生きていたこと自体が大きなチャンスだったのか……。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_BLUE,		"……しまったな。" },
		{ SCENE_ACTION_EXIBIT,	SCENE_WHO_BLUE,		"hide" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"clear" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"true" },
		{ SCENE_ACTION_EQ,		SCENE_WHO_DESC,		"false" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_DESC,		"pop_once" },
		{ SCENE_ACTION_LOAD,	SCENE_WHO_YELLOW,	"sound/bgm08.mp3" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.Kは王になろうとしていたのか……。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"王、そうか！" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Kとは king のことだったのか。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"Mr.K。僕も見えたよ。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"さぁ、後はこの世界にいるのは僕だけだ。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"だからこの王である僕を殺して世界を救ってくれ。" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_YELLOW,	"pop" },
		{ SCENE_ACTION_BATTLE,	SCENE_WHO_YELLOW,	"" },
		{ SCENE_ACTION_MUSIC,	SCENE_WHO_YELLOW,	"stop" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_YELLOW,	"ありがとう。" },
		{ SCENE_ACTION_STOP,	SCENE_WHO_DESC,		"" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"しかと見届けたぞ。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"鹿と見届けたぞ。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_PLAYER,	"このゲームの王が世界を滅ぼすというなら…" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_PLAYER,	"王であるプレイヤーが、この世界に居る貴様を滅ぼすのだろう？" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"気づいてしまったか。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"ならば鹿他無い。唯、私が上であることを確認するだけだ。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"かかって来なさい。" },
		{ SCENE_ACTION_TALK,	SCENE_WHO_DEER,		"…………" },
		{ -1,					-1,					"" }
	};
	struct Saying sayings1[20] = {
		{ "10",		SCENE_WHO_BLUE,		"貴方は私にとって、利用価値のある存在です。" },
		{ "10",		SCENE_WHO_BLUE,		"これからも是非お付き合いください。" },
		{ "10",		SCENE_WHO_YELLOW,	"……（こいつ切るか）。" },
		{ "20",		SCENE_WHO_BLUE,		"あのカード…" },
		{ "20",		SCENE_WHO_BLUE,		"本当か分からないが、チャンスかも知れないな。" },
		{ "20",		SCENE_WHO_YELLOW,	"心の声、漏れているよ。" },
		{ "20",		SCENE_WHO_BLUE,		"ホッ！" },
		{ "30",		SCENE_WHO_BLUE,		"ブツブツ…（これは使える！やりましょう！）" },
		{ "30",		SCENE_WHO_YELLOW,	"何言ってんだこいつ。" },
		{ "40",		SCENE_WHO_DESC,		"机: ………。" },
		{ "40",		SCENE_WHO_DESC,		"机: …………………。" },
		{ "40",		SCENE_WHO_DESC,		"私は机だ！まだ話しかけるな！！" },
		{ "50",		SCENE_WHO_BLUE,		"あなた1人になるまで隠れていました。" },
		{ "50",		SCENE_WHO_BLUE,		"他の2人を消してくれてありがとう。こうなることはカードを見た時から分かっていたからね。" },
		{ "50",		SCENE_WHO_BLUE,		"そして連戦の貴方を始末すれば、ここは私の世界になる訳だ。そうだろう？" },
		{ "50",		SCENE_WHO_BLUE,		"私は世界を滅ぼすつもりじゃないのです。" },
		{ "50",		SCENE_WHO_BLUE,		"貴方が邪魔なだけなのですよ。" },
		{ "999",	-1,					"" }
	};
	struct Saying sayings2[20] = {
		{ "10",		SCENE_WHO_RED,		"俺たちはチーム！そして何より正義だ！" },
		{ "10",		SCENE_WHO_YELLOW,	"あぁ、そうだな。これから先もだ。" },
		{ "20",		SCENE_WHO_RED,		"何だこのカードはぁ！俺を馬鹿にしているのかぁ！！" },
		{ "20",		SCENE_WHO_RED,		"ドカッ！（何かを蹴る音）" },
		{ "20",		SCENE_WHO_BLUE,		"！" },
		{ "20",		SCENE_WHO_YELLOW,	"うわぁ、落ち着けよ。怖いなぁ。" },
		{ "30",		SCENE_WHO_RED,		"世界が滅びるというのは本当の事のようだな。"},
		{ "30",		SCENE_WHO_RED,		"しかし、一体我々のうちの誰が滅ぼすというのだ？"},
		{ "30",		SCENE_WHO_RED,		"貴様か？ Mr.K！！！"},
		{ "999",	-1,					"" }
	};
	struct Saying sayings3[20] = {
		{ "10",		SCENE_WHO_GREEN,	"この平和の中、何のために生きているのだろう。" },
		{ "10",		SCENE_WHO_YELLOW,	"お前はいつもそれだな。考えてもしょうがないのに。" },
		{ "10",		SCENE_WHO_BLUE,		"私はこの世界を手に入れたいですねぇ。" },
		{ "20",		SCENE_WHO_GREEN,	"何かが動き出したのですね…。貴方はどう見ますか？" },
		{ "20",		SCENE_WHO_YELLOW,	"さっき突然現れた奴が何者かが気になるなぁ。" },
		{ "20",		SCENE_WHO_GREEN,	"私も同感です。奴の意図は読めませんね。" },
		{ "30",		SCENE_WHO_GREEN,	"この地震と先程のカードの関係はあるのでしょうか？" },
		{ "30",		SCENE_WHO_YELLOW,	"でもこのままでは世界が保たないぞ。" },
		{ "30",		SCENE_WHO_GREEN,	"そうですね。そろそろですねぇ…。" },
		{ "40",		SCENE_WHO_GREEN,	"貴様、Mr.Kを殺したな！" },
		{ "40",		SCENE_WHO_GREEN,	"何故いつも分かり合えないのだろう。でもそれが同じもの同士が集まった時の性なのだろう。" },
		{ "40",		SCENE_WHO_GREEN,	"だからMr.Kよ。殺し合おう。その中で生きた意味を見出そうじゃないか。" },
		{ "40",		SCENE_WHO_GREEN,	"さぁ教えてくれ。" },
		{ "999",	-1,					"" }
	};

public:

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
		onOk = 0;
		isTalking = false;
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
		case SCENE_ACTION_COCK:
			setTrigger(scene.how);
			break;
		case SCENE_ACTION_MOVE:
			doMove(scene.how, mouse);
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
			performEQ(scene.how, mouse);
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

		if (keyboard.onReturn()) {
			onOk = 1;
		}
		else {
			onOk = 0;
		}
	}

	void debugDump(int debug) {
		if (debug) {
			int strColor = strColorDebug;

			DrawFormatString(245, 205, strColor, "sceneFlg: %d", flg);
			DrawFormatString(245, 225, strColor, "frameCnt: %d", cnt);
			DrawFormatString(245, 245, strColor, "eqX: %d", eqX);
			DrawFormatString(245, 265, strColor, "textLen: %d", msg.textLen);
			DrawFormatString(245, 285, strColor, "charCnt: %d", int(msg.cnt * msg.cntPerFrame));
			DrawFormatString(245, 305, strColor, "who: %d", msg.who);
			DrawFormatString(245, 325, strColor, "mrK0.vis: %d", mrK[0].visible);
			DrawFormatString(245, 345, strColor, "mrK1.vis: %d", mrK[1].visible);
			DrawFormatString(245, 365, strColor, "mrK2.vis: %d", mrK[2].visible);
			DrawFormatString(245, 385, strColor, "mrK3.vis: %d", mrK[3].visible);
			DrawFormatString(245, 405, strColor, "deer.vis: %d", deer.visible);
			DrawFormatString(245, 425, strColor, "key: %d", key);
			DrawFormatString(245, 445, strColor, "isTalking: %s", isTalking ? "true" : "false");
			DrawFormatString(245, 465, strColor, "hasMsg: %s", hasMsg ? "true" : "false");
		}
	}


private:

	void waitClick(Mouse& mouse) {
		if (onOk || mouse.click()) {
			flg++;
		}
	}

	void goNext() {
		flg++;
	}

	// メッセージを読む
	void readMsg(string str, int who, Mouse& mouse) {
		if (!hasMsg) {
			if (str == "clear") {
				msg.set("", who, false);
				flg++;
			}
			else {
				msg.set(str, who);
				hasMsg = true;
			}
		}
		if ((onOk || mouse.click()) && msg.skip()) {
			flg++;
			hasMsg = false;
		}
	}

	void doMove(const char how[], Mouse &mouse) {
		if (isTalking) {
			int who = checkMrK();
			talkMrK(who, how, mouse);
		}
		else {
			if (key != -1) {
				mrK[0].turn(key);
				mrK[0].move();
			}
			if (onOk || mouse.click()) {
				int who = checkMrK();
				talkResetMrK(who);
			}
			if (isTriggered()) goNext();
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
		else if (how == "exibit_nowait") {
			if (who == SCENE_WHO_DEER) {
				deer.exhibit();
			}
			else {
				mrK[who - 1].exhibit();
			}
			goNext();
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
			goNext();
		}
		else {
			waitClick(mouse);
		}
	}

	void performEQ(string how, Mouse &mouse) {
		if (how == "true") {
			// happenEQ
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
			waitClick(mouse);
		}
		else {
			// stopEQ
			eqX = 0;
			goNext();
		}
	}

	void setTrigger(string trigger) {
		if (trigger == "talk_all") {
			mrK[0].setTrigger("fired");
			for (int i = 1; i < 4; ++i) {
				mrK[i].setTrigger("talk");
			}
		}
		else if (trigger == "talk_red") {
			for (int i = 0; i < 4; ++i) {
				mrK[i].setTrigger("fired");
			}
			mrK[SCENE_WHO_RED - 1].setTrigger("talk");
		}
		else if (trigger == "talk_green") {
			for (int i = 0; i < 4; ++i) {
				mrK[i].setTrigger("fired");
			}
			mrK[SCENE_WHO_GREEN - 1].setTrigger("talk");
		}
		else if (trigger == "talk_blue") {
			for (int i = 0; i < 4; ++i) {
				mrK[i].setTrigger("fired");
			}
			mrK[SCENE_WHO_BLUE - 1].setTrigger("talk");
		}
		else if (trigger == "none") {
			for (int i = 0; i < 4; ++i) {
				mrK[i].setTrigger("fired");
			}
		}
		goNext();
	}

	bool isTriggered() {
		for (int i = 0; i < 4; ++i) {
			if (!mrK[i].isTriggered()) {
				return false;
			}
		}
		return true;
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

	int checkMrK() {
		// 0番の近くにいるヤツをpick up
		for (int i = 1; i < 4; ++i) {
			if (abs(mrK[i].y - mrK[0].y) < 45 && abs(mrK[i].x - mrK[0].x) < 45) {
				return i;
			}
		}
		return 0;
	}

	void talkResetMrK(int who) {
		if (who) {
			isTalking = true;
			mrK[who].talkReset();
		}
	}

	void talkMrK(int who, const char key[], Mouse &mouse) {
		if (who) {
			Saying saying = mrK[who].talk(key);
			if (strcmp(saying.say, "") == 0 || saying.who == -1) {
				isTalking = false;
				msg.set("", SCENE_WHO_DESC, false);
				return;
			}
			if (!hasMsg) {
				msg.set(saying.say, saying.who);
				hasMsg = true;
			}
			if ((onOk || mouse.click()) && msg.skip()) {
				mrK[who].talkNext();
				hasMsg = false;
			}
		}
	}

};
