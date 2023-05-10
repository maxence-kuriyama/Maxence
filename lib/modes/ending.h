#pragma once

#include "lib/sprite.h"
#include "lib/modes/common/game.h"

void init_ending_text(string* job, string* who);


//�G���f�B���O�Ǘ��p�N���X
// �P�̂ł̎g�p��z��
class Ending {
private:
	double cnt = 0.0;
	double cntInc = 30.0 / FPS;		// 1F�������cnt�̑���
	string job[20];
	string who[20];
	int Font2 = CreateFontToHandle("HG���ȏ���", 36, 4, DX_FONTTYPE_ANTIALIASING_EDGE);
	int Font3 = CreateFontToHandle("HG���ȏ���", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
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
		// ����FPS�����ɃA�j���[�V�������x�𒲐�
		cntInc = 30.2 / fps;

		// �t�F�[�h�C��
		fadeinMusic(music);
		fadeinTitle();

		// �X�i�b�v�V���b�g
		drawEndroll();

		// Mr.K
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// 1�l�� ����ł���
		if (cnt > 1650.0 && cnt <= 2500.0) {
			mrK[0].set(cnt - 1750.0, 415);
			mrK[0].draw();
		}
		// 2�l�� �R����
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
		// 3�l�� ���V����
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
		// 4�l�� ������
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

		// ��
		// ���֕���
		if (cnt <= 1200.0) {
			deer.walk();
			deer.set(650.0 - (cnt - 830.0), 380);
		}
		// ���̏����
		else if (cnt <= 3900.0) {
			deer.walk();
			deer.set(280, 380);
		}
		// 3�l��
		else if (cnt <= 4550.0) {
			deer.stop();
			if (cnt > 4200.0) {
				deer.setSpecialImg(0);
			}
			deer.set(280, 380);
		}
		// ���̏����
		else if (cnt <= 5300.0) {
			deer.walk();
			deer.set(280, 380);
		}
		// 4�l��
		else if (cnt <= 5600.0) {
			deer.stop();
			if (cnt > 5450.0 && cnt <= 5550.0) {
				deer.setSpecialImg(1);
			}
			deer.set(280, 380);
		}
		// �ޏ�
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

	job[0] = "���";
	who[0] = "�� ���ē� Mr.A";
	job[1] = "���āE�r�{";
	who[1] = "�z���͂̌���\n Mr.A & Mr.O";
	job[2] = "�L�����N�^�[\n �f�U�C��";
	who[2] = "�����̐��E��\n �N���G�C�^�[\n  Mr.O & Mr.A";
	job[3] = "�O���t�B�b�N";
	who[3] = "�����T�O�Ȃǒm���\n Mr.O";
	job[4] = "�h�b�g�G";
	who[4] = "���k�̃}�E�X�J�[�\��\n Mr.T";
	job[5] = "3D���f��";
	who[5] = "�C���e���W�F���X\n �f�U�C�i�[\n  Mr.O";
	job[6] = "����";
	who[6] = "�x�苶���T���S\n Mr.O";
	job[7] = "�V�X�e��\n �f�U�C��";
	who[7] = "�ڐ�̎d����\n ���������\n  Mr.T";
	job[8] = "�v���O����";
	who[8] = "�ʎY�^�㗴���\n Mr.T";
	job[9] = "���y";
	who[9] = "�j��ƒ��a�̋��E��\n Mr.O & Mr.A";
	job[10] = "�Z���t";
	who[10] = "�B�ꖳ��̗L���V\n Mr.A";
	job[11] = "�Q�[��\n �f�U�C��";
	who[11] = "�����̋ʎ蔠\n Mr.T & Mr.O";
	job[12] = "�^�C�g�����S\n �f�U�C��";
	who[12] = "���݂܂���ł���\n Mr.T";
	job[13] = "�f�o�b�O\n �e�X�g�v���C";
	who[13] = "Densuke Shiraishi";
	job[14] = "�X�y�V����\n �T���N�X";
	who[14] = "Maxence";
	job[15] = "And YOU";
	who[15] = "Thank you\n for playing!!";

}