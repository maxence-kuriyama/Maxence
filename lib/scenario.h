#pragma once

#include "lib/message.h"

void init_scene_text(string* scen_txt, int* scen_who);

class Scenario {
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
	string text[40];
	int who[40];
	int cnt = 0;	// フレームカウンタ

	Scenario() {
		init_scene_text(text, who);
		imgRoom = LoadGraph("graph/room.bmp");
		imgCard = LoadGraph("graph/card.bmp");
		mrK[0].set(160, 120, "graph/stripe11.png", 1);
		mrK[1].set(480, 120, "graph/stripe12.png", 1);
		mrK[2].set(160, 240, "graph/stripe13.png", 1);
		mrK[3].set(480, 240, "graph/stripe14.png", 1);
		deer.set(0, 0, "graph/stripe15.png", 0);
		initialize();
		msg.initialize();
	}

	void initialize() {
		flg = 0;
		textCnt = 0;
		msg.set(text[textCnt], who[textCnt]);
	}

	int show(Mouse& mouse, int strColor) {
		DrawExtendGraph(0 + eqX, -50, 640 + eqX, 380, imgRoom, FALSE);
		for (int i = 0; i < 4; ++i) {
			if (mrK[i].visible) {
				DrawGraph(mrK[i].x + eqX, mrK[i].y, mrK[i].img, TRUE);
			}
		}
		if (flg == 2) DrawExtendGraph(0, 0, 640, 400, imgCard, FALSE);

		msg.draw();

		if (CheckMusic() != 1) {
			if (flg == 0 || flg == 7 || flg == 12) {
				PlayMusic("sound/bgm03.mp3", DX_PLAYTYPE_BACK);
			}
			if (flg == 21) {
				PlayMusic("sound/bgm07.mp3", DX_PLAYTYPE_BACK);
			}
		}

		cnt++;

		DrawFormatString(5, 5, strColor, "flg:%d", flg);
		//DrawFormatString(5, 25, StringColor, "char_cnt:%d", char_cnt);

		switch (flg) {
		case 1:
			//鹿が現れる
			DrawGraph(480, 120, mrK[1].img, TRUE);
			DrawGraph(270, 200, deer.img, TRUE);
			if (mouse.click()) {
				flg++;
				textCnt++;
				msg.set(text[textCnt], who[textCnt]);
			}
			break;
		case 2:
			//カードを見つける、青消える
			msg.textLen = MultiByteLength(text[textCnt].c_str());
			if (mouse.click() && msg.skip()) {
				mrK[1].hide();
				flg++;
				textCnt++;
				msg.set(text[textCnt], who[textCnt]);
			}
			break;
		case 4:
			//地震
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
			if (mouse.click()) {
				eqX = 0;
				flg++;
				textCnt++;
				msg.set(text[textCnt], who[textCnt]);
			}
			break;
		case 6:
			//第一戦
			PlayMusic("sound/bgm04.mp3", DX_PLAYTYPE_BACK);
			textCnt++;
			msg.set(text[textCnt], who[textCnt]);
			return 1;
		case 8:
			//赤が死ぬ
			mrK[2].hide();
			if (mouse.click()) {
				flg++;
			}
			break;
		case 9:
			//地震
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
			if (mouse.click()) {
				eqX = 0;
				flg++;
				textCnt++;
				msg.set(text[textCnt], who[textCnt]);
			}
			break;
		case 11:
			//第二戦
			PlayMusic("sound/bgm05.mp3", DX_PLAYTYPE_BACK);
			textCnt++;
			msg.set(text[textCnt], who[textCnt]);
			return 1;
		case 13:
			//緑が死ぬ
			mrK[3].hide();
			if (mouse.click()) {
				flg++;
			}
			break;
		case 14:
			//地震
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
			if (mouse.click()) {
				eqX = 0;
				flg++;
				textCnt++;
				msg.set(text[textCnt], who[textCnt]);
			}
			break;
		case 15:
			//青が出てくる
			mrK[1].exhibit();
			if (mouse.click()) {
				flg++;
			}
			break;
		case 17:
			//第三戦
			PlayMusic("sound/bgm06.mp3", DX_PLAYTYPE_BACK);
			textCnt++;
			msg.set(text[textCnt], who[textCnt]);
			return 1;
		case 19:
			//青が死ぬ
			mrK[1].hide();
			if (mouse.click()) {
				flg++;
			}
			break;
		case 20:
			//地震
			eqX = 10 * sin(eqX + M_PI * (rand() % 10) / 10.0);
			if (mouse.click()) {
				eqX = 0;
				flg++;
				textCnt++;
				msg.set(text[textCnt], who[textCnt]);
			}
			break;
		case 22:
			//第四戦
			PlayMusic("sound/bgm08.mp3", DX_PLAYTYPE_BACK);
			textCnt++;
			msg.set(text[textCnt], who[textCnt]);
			return 1;
		default:
			msg.textLen = MultiByteLength(text[textCnt].c_str());
			if (mouse.click() && msg.skip()) {
				if (textCnt == 0 || textCnt == 3 || textCnt == 5 || textCnt == 8 || textCnt == 12 || textCnt == 14 || textCnt == 18 || textCnt == 20 || textCnt == 26) {
					flg++;
				}
				else {
					textCnt = min(27, textCnt + 1);
					msg.set(text[textCnt], who[textCnt]);
				}
			}
			break;
		}

		return 0;
	}
};

void init_scene_text(string* scen_txt, int* scen_who) {

	scen_who[0] = 0;
	scen_txt[0] = "――世界は１つの部屋で出来ている。";
	scen_who[1] = 0;
	scen_txt[1] = "「Mr.Kが世界を滅ぼす」";
	scen_who[2] = 1;
	scen_txt[2] = "Mr.K: なにっ！？";
	scen_who[3] = 4;
	scen_txt[3] = "Mr.K: あれ、Mr.Kが居ないぞ、何なんだ！";
	scen_who[4] = 3;
	scen_txt[4] = "Mr.K: 世界が滅びるというのは本当の事のようだな。";
	scen_who[5] = 3;
	scen_txt[5] = "Mr.K: しかし、一体我々のうちの誰が滅ぼすというのだ？ 貴様か？ Mr.K！！！";
	scen_who[6] = 3;
	scen_txt[6] = "Mr.K: 馬鹿な、そんなはずではなかったのに…。";
	scen_who[7] = 3;
	scen_txt[7] = "Mr.K: 最後の最期は後悔しないと決めていたのに、こんな死に様とはな。";
	scen_who[8] = 3;
	scen_txt[8] = "Mr.K: だが、俺が死ねば世界の崩壊が止まるというのなら、俺の死にも意味を持たせられるというものじゃないか…………。";
	scen_who[9] = 4;
	scen_txt[9] = "Mr.K: 貴様、Mr.Kを殺したな！";
	scen_who[10] = 4;
	scen_txt[10] = "Mr.K: 何故いつも分かり合えないのだろう。でもそれが同じもの同士が集まった時の性なのだろう。";
	scen_who[11] = 4;
	scen_txt[11] = "Mr.K: だからMr.Kよ。殺し合おう。その中で生きた意味を見出そうじゃないか。";
	scen_who[12] = 4;
	scen_txt[12] = "Mr.K: さぁ教えてくれ。";
	scen_who[13] = 4;
	scen_txt[13] = "Mr.K: 俺は見えたよ。";
	scen_who[14] = 4;
	scen_txt[14] = "Mr.K: 残ったお前にもいつか見える時が来るだろう…………。先に逝っているよ。";
	scen_who[15] = 2;
	scen_txt[15] = "Mr.K: あなた1人になるまで隠れていました。";
	scen_who[16] = 2;
	scen_txt[16] = "Mr.K: 他の2人を消してくれてありがとう。こうなることはカードを見た時から分かっていたからね。";
	scen_who[17] = 2;
	scen_txt[17] = "Mr.K: そして連戦の貴方を始末すれば、ここは私の世界になる訳だ。そうだろう？";
	scen_who[18] = 2;
	scen_txt[18] = "Mr.K: 私は世界を滅ぼすつもりじゃないのです。貴方が邪魔なだけなのですよ。";
	scen_who[19] = 2;
	scen_txt[19] = "Mr.K: 馬鹿な、そんな、世界の王に成れるチャンスを逃してしまうなんて、有り得ない。";
	scen_who[20] = 2;
	scen_txt[20] = "Mr.K: …………ああ、生きていたこと自体が大きなチャンスだったのか…………しまったな。";
	scen_who[21] = 1;
	scen_txt[21] = "Mr.K: Mr.Kは王になろうとしていたのか…………。";
	scen_who[22] = 1;
	scen_txt[22] = "Mr.K: 王、そうか！";
	scen_who[23] = 1;
	scen_txt[23] = "Mr.K: Kとは king のことだったのか。";
	scen_who[24] = 1;
	scen_txt[24] = "Mr.K: Mr.K。僕も見えたよ。";
	scen_who[25] = 1;
	scen_txt[25] = "Mr.K: さぁ、後はこの世界にいるのは僕だけだ。";
	scen_who[26] = 1;
	scen_txt[26] = "Mr.K: だからこの王である僕を殺して世界を救ってくれ。";
	scen_who[27] = 1;
	scen_txt[27] = "Mr.K: ありがとう。";
	scen_who[28] = 5;
	scen_txt[28] = "鹿: しかと見届けたぞ。";
	scen_who[29] = 5;
	scen_txt[29] = "鹿: 鹿と見届けたぞ。";
	scen_who[30] = -1;
	scen_txt[30] = "このゲームの王が世界を滅ぼすというなら…";
	scen_who[31] = -1;
	scen_txt[31] = "王であるプレイヤーが、この世界に居る貴様を滅ぼすのだろう？";
	scen_who[32] = 5;
	scen_txt[32] = "鹿: 気づいてしまったか。";
	scen_who[33] = 5;
	scen_txt[33] = "鹿: ならば鹿他無い。唯、私が上であることを確認するだけだ。";
	scen_who[34] = 5;
	scen_txt[34] = "鹿: かかって来なさい。";
	scen_who[35] = 5;
	scen_txt[35] = "鹿: …………";

}
