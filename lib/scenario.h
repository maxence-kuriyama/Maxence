#pragma once

#include "lib/message.h"

void init_scene_text(string* scen_txt, int* scen_who);

class Scenario {
private:
	int strColorDebug = GetColor(150, 0, 0);

public:
	int flg = 0;	// シナリオ管理用フラグ
	int imgRoom;
	int imgCard;
	MrK mrK[4];
	MrK deer;
	Message msg;
	int textCnt = 0;	// テキストカウンタ
	int eqX = 0;
	int eqY = 0;		// eq = earthquake
	string text[50];
	int who[50];
	int cnt = 0;	// フレームカウンタ

	Scenario() {
		init_scene_text(text, who);
		imgRoom = LoadGraph("graph/room.bmp");
		imgCard = LoadGraph("graph/card.bmp");
		mrK[0].set(160, 120, "graph/stripe11.png", 1);
		mrK[1].set(480, 120, "graph/stripe12.png", 1);
		mrK[2].set(160, 240, "graph/stripe13.png", 1);
		mrK[3].set(480, 240, "graph/stripe14.png", 1);
		deer.set(270, 200, "graph/stripe15.png", 0);
		initialize();
		msg.initialize();
	}

	void initialize() {
		flg = 0;
		cnt = 0;
		textCnt = 0;
		msg.set(text[textCnt], who[textCnt]);
		mrK[0].exhibit();
		mrK[1].exhibit();
		mrK[2].exhibit();
		mrK[3].exhibit();
		deer.hide();
	}

	void msgSet() {
		msg.set(text[textCnt], who[textCnt]);
	}

	void msgLoad() {
		textCnt++;
		msgSet();
	}

	void happenEQ() {
		eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
	}

	void stopEQ() {
		eqX = 0;
	}

	// NEXTが出るまでメッセージを読む
	void readMsg(Mouse& mouse) {
		if (mouse.click() && msg.skip()) {
			if (text[textCnt + 1] == "NEXT") {
				flg++;
				textCnt++;
			}
			else {
				textCnt = min(36, textCnt + 1);
				msgSet();
			}
		}
	}

	// 次のメッセージを読みながら場面転換
	void nextMsg(Mouse& mouse) {
		if (mouse.click()) {
			flg++;
			msgLoad();
		}
	}

	// メッセージを送らずに場面転換
	void waitClick(Mouse& mouse){
		if (mouse.click()) {
			flg++;
		}
	}

	void startMusic(const char* musicName) {
		if (CheckMusic() != 1) {
			PlayMusic(musicName, DX_PLAYTYPE_BACK);
		}
	}

	int show(Mouse& mouse) {
		// 背景・人物の描画
		DrawExtendGraph(0 + eqX, -50, 640 + eqX, 380, imgRoom, FALSE);
		for (int i = 0; i < 4; ++i) {
			mrK[i].draw(eqX);
		}
		deer.draw();

		switch (flg) {
		case 0:
			startMusic("sound/bgm03.mp3");
			readMsg(mouse);
			break;
		case 1:
			// 鹿が現れる
			deer.exhibit();
			nextMsg(mouse);
			break;
		case 2:
			// カードを見つける
			DrawExtendGraph(0, 0, 640, 400, imgCard, FALSE);
			nextMsg(mouse);
			break;
		case 3:
			// 青消える
			deer.hide();
			mrK[1].hide();
			readMsg(mouse);
			break;
		case 4:
			// 地震
			happenEQ();
			nextMsg(mouse);
			break;
		case 5:
			stopEQ();
			readMsg(mouse);
			break;
		case 6:
			// 第一戦
			StopMusic();
			startMusic("sound/bgm04.mp3");
			msgLoad();
			return 1;
		case 7:
			startMusic("sound/bgm03.mp3");
			readMsg(mouse);
			break;
		case 8:
			// 赤が死ぬ
			mrK[2].hide();
			waitClick(mouse);
			break;
		case 9:
			// 地震
			happenEQ();
			nextMsg(mouse);
			break;
		case 10:
			stopEQ();
			readMsg(mouse);
			break;
		case 11:
			// 第二戦
			StopMusic();
			startMusic("sound/bgm05.mp3");
			msgLoad();
			return 1;
		case 12:
			startMusic("sound/bgm03.mp3");
			readMsg(mouse);
			break;
		case 13:
			// 緑が死ぬ
			mrK[3].hide();
			waitClick(mouse);
			break;
		case 14:
			// 地震
			happenEQ();
			waitClick(mouse);
			break;
		case 15:
			// 青が出てくる
			stopEQ();
			mrK[1].exhibit();
			nextMsg(mouse);
			break;
		case 17:
			// 第三戦
			StopMusic();
			startMusic("sound/bgm06.mp3");
			msgLoad();
			return 1;
		case 19:
			// 青が死ぬ
			mrK[1].hide();
			waitClick(mouse);
			break;
		case 20:
			// 地震
			happenEQ();
			nextMsg(mouse);
			break;
		case 21:
			stopEQ();
			startMusic("sound/bgm07.mp3");
			readMsg(mouse);
			break;
		case 22:
			// 第四戦
			StopMusic();
			startMusic("sound/bgm08.mp3");
			msgLoad();
			return 1;
		default:
			readMsg(mouse);
			break;
		}

		msg.draw();
		cnt = (cnt + 1) % 10000;

		return 0;
	}

	void debugDump(int debug) {
		if (debug) {
			int strColor = strColorDebug;

			DrawFormatString(245, 205, strColor, "sceneFlg: %d", flg);
			DrawFormatString(245, 225, strColor, "frameCnt: %d", cnt);
			DrawFormatString(245, 245, strColor, "textCnt: %d", textCnt);
			DrawFormatString(245, 265, strColor, "eqX: %d", eqX);
			DrawFormatString(245, 285, strColor, "textLen: %d", msg.textLen);
			DrawFormatString(245, 305, strColor, "charCnt: %d", msg.charCnt);
			DrawFormatString(245, 325, strColor, "who: %d", msg.who);
			DrawFormatString(245, 345, strColor, "mrK0.vis: %d", mrK[0].visible);
			DrawFormatString(245, 365, strColor, "mrK1.vis: %d", mrK[1].visible);
			DrawFormatString(245, 385, strColor, "mrK2.vis: %d", mrK[2].visible);
			DrawFormatString(245, 405, strColor, "mrK3.vis: %d", mrK[3].visible);
			DrawFormatString(245, 425, strColor, "deer.vis: %d", deer.visible);
		}
	}
};

void init_scene_text(string* scen_txt, int* scen_who) {

	scen_who[0] = 0;
	scen_txt[0] = "――世界は１つの部屋で出来ている。";
	scen_who[1] = 0;
	scen_txt[1] = "NEXT";
	scen_who[2] = 0;
	scen_txt[2] = "「Mr.Kが世界を滅ぼす」";
	scen_who[3] = 1;
	scen_txt[3] = "Mr.K: なにっ！？";
	scen_who[4] = 4;
	scen_txt[4] = "Mr.K: あれ、Mr.Kが居ないぞ、何なんだ！";
	scen_who[5] = 0;
	scen_txt[5] = "NEXT";
	scen_who[6] = 3;
	scen_txt[6] = "Mr.K: 世界が滅びるというのは本当の事のようだな。";
	scen_who[7] = 3;
	scen_txt[7] = "Mr.K: しかし、一体我々のうちの誰が滅ぼすというのだ？ 貴様か？ Mr.K！！！";
	scen_who[8] = 0;
	scen_txt[8] = "NEXT";
	scen_who[9] = 3;
	scen_txt[9] = "Mr.K: 馬鹿な、そんなはずではなかったのに…。";
	scen_who[10] = 3;
	scen_txt[10] = "Mr.K: 最後の最期は後悔しないと決めていたのに、こんな死に様とはな。";
	scen_who[11] = 3;
	scen_txt[11] = "Mr.K: だが、俺が死ねば世界の崩壊が止まるというのなら、俺の死にも意味を持たせられるというものじゃないか…………。";
	scen_who[12] = 0;
	scen_txt[12] = "NEXT";
	scen_who[13] = 4;
	scen_txt[13] = "Mr.K: 貴様、Mr.Kを殺したな！";
	scen_who[14] = 4;
	scen_txt[14] = "Mr.K: 何故いつも分かり合えないのだろう。でもそれが同じもの同士が集まった時の性なのだろう。";
	scen_who[15] = 4;
	scen_txt[15] = "Mr.K: だからMr.Kよ。殺し合おう。その中で生きた意味を見出そうじゃないか。";
	scen_who[16] = 4;
	scen_txt[16] = "Mr.K: さぁ教えてくれ。";
	scen_who[17] = 0;
	scen_txt[17] = "NEXT";
	scen_who[18] = 4;
	scen_txt[18] = "Mr.K: 俺は見えたよ。";
	scen_who[19] = 4;
	scen_txt[19] = "Mr.K: 残ったお前にもいつか見える時が来るだろう…………。先に逝っているよ。";
	scen_who[20] = 0;
	scen_txt[20] = "NEXT";
	scen_who[21] = 2;
	scen_txt[21] = "Mr.K: あなた1人になるまで隠れていました。";
	scen_who[22] = 2;
	scen_txt[22] = "Mr.K: 他の2人を消してくれてありがとう。こうなることはカードを見た時から分かっていたからね。";
	scen_who[23] = 2;
	scen_txt[23] = "Mr.K: そして連戦の貴方を始末すれば、ここは私の世界になる訳だ。そうだろう？";
	scen_who[24] = 2;
	scen_txt[24] = "Mr.K: 私は世界を滅ぼすつもりじゃないのです。貴方が邪魔なだけなのですよ。";
	scen_who[25] = 0;
	scen_txt[25] = "NEXT";
	scen_who[26] = 2;
	scen_txt[26] = "Mr.K: 馬鹿な、そんな、世界の王に成れるチャンスを逃してしまうなんて、有り得ない。";
	scen_who[27] = 2;
	scen_txt[27] = "Mr.K: …………ああ、生きていたこと自体が大きなチャンスだったのか…………しまったな。";
	scen_who[28] = 0;
	scen_txt[28] = "NEXT";
	scen_who[29] = 1;
	scen_txt[29] = "Mr.K: Mr.Kは王になろうとしていたのか…………。";
	scen_who[30] = 1;
	scen_txt[30] = "Mr.K: 王、そうか！";
	scen_who[31] = 1;
	scen_txt[31] = "Mr.K: Kとは king のことだったのか。";
	scen_who[32] = 1;
	scen_txt[32] = "Mr.K: Mr.K。僕も見えたよ。";
	scen_who[33] = 1;
	scen_txt[33] = "Mr.K: さぁ、後はこの世界にいるのは僕だけだ。";
	scen_who[34] = 1;
	scen_txt[34] = "Mr.K: だからこの王である僕を殺して世界を救ってくれ。";
	scen_who[35] = 0;
	scen_txt[35] = "NEXT";
	scen_who[36] = 1;
	scen_txt[36] = "Mr.K: ありがとう。";
	scen_who[37] = 5;
	scen_txt[37] = "鹿: しかと見届けたぞ。";
	scen_who[38] = 5;
	scen_txt[38] = "鹿: 鹿と見届けたぞ。";
	scen_who[39] = -1;
	scen_txt[39] = "このゲームの王が世界を滅ぼすというなら…";
	scen_who[40] = -1;
	scen_txt[40] = "王であるプレイヤーが、この世界に居る貴様を滅ぼすのだろう？";
	scen_who[41] = 5;
	scen_txt[41] = "鹿: 気づいてしまったか。";
	scen_who[42] = 5;
	scen_txt[42] = "鹿: ならば鹿他無い。唯、私が上であることを確認するだけだ。";
	scen_who[43] = 5;
	scen_txt[43] = "鹿: かかって来なさい。";
	scen_who[44] = 5;
	scen_txt[44] = "鹿: …………";

}
