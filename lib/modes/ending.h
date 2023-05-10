#pragma once

#include "lib/sprite.h"
#include "lib/modes/common/game.h"

void init_ending_text(string* job, string* who);


//エンディング管理用クラス
// 単体での使用を想定
class Ending {
private:
	double cnt = 0.0;
	double cntInc = 30.0 / FPS;		// 1Fあたりのcntの増分
	string job[20];
	string who[20];
	int Font2 = CreateFontToHandle("HG教科書体", 36, 4, DX_FONTTYPE_ANTIALIASING_EDGE);
	int Font3 = CreateFontToHandle("HG教科書体", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
	int Font4 = CreateFontToHandle("Times New Roman", 72, 6, DX_FONTTYPE_ANTIALIASING_EDGE);
	int MLogo = LoadGraph("graph/M.png");
	int axence = LoadGraph("graph/axence.png");
	unsigned int White = GetColor(255, 255, 255);
	unsigned int strColorDebug = GetColor(180, 180, 180);
	int end_pict[20];
	int sprite[15];

public:
	MrK mrK[4];
	MrK deer;

	Ending() {
		for (int i = 1; i <= 20; ++i) {
			string pict_name;
			pict_name = "graph/end_pict" + to_string(i) + ".png";
			end_pict[i - 1] = LoadGraph(pict_name.c_str());
		}
		mrK[0].set(-100, 415, "graph/sprite1.png");
		mrK[1].set(-100, 425, "graph/sprite2.png");
		mrK[2].set(-100, 420, "graph/sprite1.png");
		mrK[3].img[0] = LoadGraph("graph/sprite3.png");
		mrK[3].img[1] = LoadGraph("graph/sprite4.png");
		mrK[3].img[2] = LoadGraph("graph/sprite3.png");
		mrK[3].img[3] = LoadGraph("graph/sprite4.png");
		deer.setSerialImages(4, "graph/sprite", 5);
		deer.spImg[0] = LoadGraph("graph/sprite9.png");
		deer.spImg[1] = LoadGraph("graph/sprite10.png");
		mrK[3].setLoopSpeed(5 * FPS / 30);
		deer.setLoopSpeed(15 * FPS / 30);
		initialize();
	}

	void initialize() {
		cnt = 0.0;
		init_ending_text(job, who);
	}

	int show(Music& music, int fps) {
		// 実効FPSを元にアニメーション速度を調整
		cntInc = 30.2 / fps;

		// フェードイン
		fadeinMusic(music);
		fadeinTitle();

		// スナップショット
		drawEndroll();

		// Mr.K
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// 1人目 死んでいる
		if (cnt > 1650.0 && cnt <= 2500.0) {
			mrK[0].set(cnt - 1750.0, 415);
			mrK[0].draw();
		}
		// 2人目 蹴られる
		if (cnt > 2550.0 && cnt <= 3500.0) {
			double tempx = -200.0;
			if (cnt <= 2900.0) {
				tempx = cnt - 2650.0;
			}
			else {
				tempx = 220.0 + (cnt - 2990.0);
				for (int i = 0; i < 3; ++i) {
					double baseCnt = 2910.0 + i * 40.0;
					if (cnt <= baseCnt + 30.0) {
						if (cnt <= baseCnt) {
							tempx = 220.0 + 0.3 * pow(cnt - baseCnt, 2.0);
						}
						else {
							tempx = 220.0 + (cnt - baseCnt);
						}
						break;
					}
				}
			}
			mrK[1].set(tempx, 425);
			mrK[1].draw();
		}
		// 3人目 昇天する
		if (cnt > 3600.0 && cnt <= 4600.0) {
			double tempx = 200.0;
			double tempy = 420.0;
			if (cnt <= 3900.0) {
				tempx = cnt - 3700.0;
			}
			else if (cnt > 4100.0) {
				tempy = 420.0 - 1.2 * (cnt - 4100.0);
			}
			mrK[2].set(tempx, tempy);
			mrK[2].draw();
		}
		// 4人目 見送る
		if (cnt > 4800.0) {
			double tempx = 400.0;
			if (cnt <= 5300.0) {
				tempx = cnt - 4900.0;
			}
			else if (cnt > 5450.0 && cnt <= 5550.0) {
				mrK[3].walk();
			}
			else {
				mrK[3].stop();
			}
			mrK[3].set(tempx, 395);
			mrK[3].draw();
		}

		// 鹿
		// 左へ歩く
		if (cnt <= 1200.0) {
			deer.walk();
			deer.set(650.0 - (cnt - 830.0), 380);
		}
		// その場歩き
		else if (cnt <= 3900.0) {
			deer.walk();
			deer.set(280, 380);
		}
		// 3人目
		else if (cnt <= 4550.0) {
			deer.stop();
			if (cnt > 4200.0) {
				deer.setSpecialImg(0);
			}
			deer.set(280, 380);
		}
		// その場歩き
		else if (cnt <= 5300.0) {
			deer.walk();
			deer.set(280, 380);
		}
		// 4人目
		else if (cnt <= 5600.0) {
			deer.stop();
			if (cnt > 5450.0 && cnt <= 5550.0) {
				deer.setSpecialImg(1);
			}
			deer.set(280, 380);
		}
		// 退場
		else  if (cnt <= 6000.0) {
			deer.walk(10 * FPS / 30);
			deer.set(280.0 - 1.2 * (cnt - 5600.0), 380);
		}

		if (cnt <= 6500.0) {
			cnt += cntInc;
			deer.draw();
		}

		//Fin
		drawFin();
		fadeoutMusic(music);

		return 0;
	}

	void fadeinMusic(Music& music) {
		if (cnt > 280.0 && cnt <= 900.0) {
			music.changeVolume(22.0 * pow(cnt - 250.0, 0.3));
			music.play();
		}
	}

	void fadeoutMusic(Music& music) {
		if (cnt > 6100.0 && cnt <= 6200.0) {
			music.changeVolume(7.57 * pow(6250.0 - cnt, 0.6));
			music.play();
		}
	}

	void fadeinTitle() {
		double fade = min(255.0, 0.01 * pow(max(0.0, cnt - 50.0), 2.0));
		SetDrawBlendMode(DX_BLENDMODE_SUB, fade);
		DrawBox(-1, -1, 641, 481, White, TRUE);
		if (cnt <= 300.0) {
			SetDrawBlendMode(DX_BLENDMODE_ADD, fade);
			DrawExtendGraph(170, 170, 255, 260, MLogo, TRUE);
			DrawExtendGraph(250, 170, 490, 260, axence, TRUE);
		}
		else if (cnt <= 550.0) {
			SetDrawBlendMode(DX_BLENDMODE_ADD, fade);
			DrawExtendGraph(170, 170 - 1.2 * (cnt - 300.0), 255, 260 - 1.2 * (cnt - 300.0), MLogo, TRUE);
			DrawExtendGraph(250, 170 - 1.2 * (cnt - 300.0), 490, 260 - 1.2 * (cnt - 300.0), axence, TRUE);
		}
	}

	void drawGameBoard(Game& game) {
		game.drawBase();
		game.drawGlobalState();
		game.drawHistLast();
		game.drawNextField();
		game.drawLocalState();
		game.drawCurrentCoord();
	}

	void drawEndroll() {
		for (int i = 0; i < 8; ++i) {
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			if (cnt > 640.0 * i + 550.0 && cnt <= 640.0 * (i + 1) + 550.0) {
				DrawExtendGraph(
					280,
					480 - 1.2 * (cnt - 640.0 * i - 550.0),
					600,
					720 - 1.2 * (cnt - 640.0 * i - 550.0),
					end_pict[2 * i], TRUE);
				DrawStringToHandle(20, 520.0 - 1.2 * (cnt - 640.0 * i - 550.0), job[2 * i].c_str(), White, Font2);
				DrawStringToHandle(20, 620.0 - 1.2 * (cnt - 640.0 * i - 550.0), who[2 * i].c_str(), White, Font3);
			}
			if (cnt > 640.0 * i + 870.0 && cnt <= 640.0 * (i + 1) + 870.0) {
				DrawExtendGraph(
					40,
					480.0 - 1.2 * (cnt - 640.0 * i - 870.0),
					360,
					720.0 - 1.2 * (cnt - 640.0 * i - 870.0),
					end_pict[2 * i + 1], TRUE);
				DrawStringToHandle(370, 520.0 - 1.2 * (cnt - 640.0 * i - 870.0), job[2 * i + 1].c_str(), White, Font2);
				DrawStringToHandle(370, 620.0 - 1.2 * (cnt - 640.0 * i - 870.0), who[2 * i + 1].c_str(), White, Font3);
			}
		}
	}

	void drawFin() {
		if (cnt > 5680.0) {
			if (cnt <= 5950.0) {
				DrawStringToHandle(270, 520.0 - 1.2 * (cnt - 5680.0), "Fin", White, Font4);
			}
			else {
				DrawStringToHandle(270, 520.0 - 1.2 * 270.0, "Fin", White, Font4);
			}
		}
	}

	void debugDump() {
		int strColor = strColorDebug;

		DrawFormatString(365, 265, strColor, "endingCnt: %4.2f", cnt);
		DrawFormatString(365, 285, strColor, "endingCntInc: %4.2f", cntInc);
		DrawFormatString(365, 305, strColor, "mrK0.walkCnt: %d", mrK[0].walkCnt);
		DrawFormatString(365, 325, strColor, "mrK1.walkCnt: %d", mrK[1].walkCnt);
		DrawFormatString(365, 345, strColor, "mrK2.walkCnt: %d", mrK[2].walkCnt);
		DrawFormatString(365, 365, strColor, "mrK3.walkCnt: %d", mrK[3].walkCnt);
		DrawFormatString(365, 385, strColor, "deer.walkCnt: %d", deer.walkCnt);
	}

};

void init_ending_text(string* job, string* who) {

	job[0] = "企画";
	who[0] = "兼 総監督 Mr.A";
	job[1] = "原案・脚本";
	who[1] = "想像力の権化\n Mr.A & Mr.O";
	job[2] = "キャラクター\n デザイン";
	who[2] = "未来の世界の\n クリエイター\n  Mr.O & Mr.A";
	job[3] = "グラフィック";
	who[3] = "既成概念など知らぬ\n Mr.O";
	job[4] = "ドット絵";
	who[4] = "激震のマウスカーソル\n Mr.T";
	job[5] = "3Dモデル";
	who[5] = "インテリジェンス\n デザイナー\n  Mr.O";
	job[6] = "動画";
	who[6] = "踊り狂う探究心\n Mr.O";
	job[7] = "システム\n デザイン";
	who[7] = "目先の仕事は\n 投げるもの\n  Mr.T";
	job[8] = "プログラム";
	who[8] = "量産型九龍城砦\n Mr.T";
	job[9] = "音楽";
	who[9] = "破壊と調和の境界線\n Mr.O & Mr.A";
	job[10] = "セリフ";
	who[10] = "唯一無二の有頂天\n Mr.A";
	job[11] = "ゲーム\n デザイン";
	who[11] = "悪乗りの玉手箱\n Mr.T & Mr.O";
	job[12] = "タイトルロゴ\n デザイン";
	who[12] = "すみませんでした\n Mr.T";
	job[13] = "デバッグ\n テストプレイ";
	who[13] = "Densuke Shiraishi";
	job[14] = "スペシャル\n サンクス";
	who[14] = "Maxence";
	job[15] = "And YOU";
	who[15] = "Thank you\n for playing!!";

}