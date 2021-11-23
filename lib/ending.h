#pragma once

#include "lib/message.h"

void init_ending_text(string* job, string* who);

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
	unsigned int Green = GetColor(0, 255, 0);
	unsigned int Red = GetColor(255, 0, 0);
	unsigned int Blue = GetColor(0, 0, 255);
	unsigned int White = GetColor(255, 255, 255);
	unsigned int Black = GetColor(0, 0, 0);
	unsigned int strColorDebug = GetColor(180, 180, 180);
	int end_pict[20];
	int stripe[15];

public:
	MrK mrK[4];
	MrK deer;

	Ending() {
		for (int i = 1; i <= 20; ++i) {
			string pict_name;
			pict_name = "graph/end_pict" + to_string(i) + ".png";
			end_pict[i - 1] = LoadGraph(pict_name.c_str());
		}
		// ���폜�\��
		for (int i = 1; i <= 15; ++i) {
			string pict_name;
			pict_name = "graph/stripe" + to_string(i) + ".png";
			stripe[i - 1] = LoadGraph(pict_name.c_str());
		}
		// ��
		mrK[0].set(-100, 415, "graph/stripe1.png", 1);
		// mrK[1].set(-100, 425, "graph/stripe2.png", 1);
		// mrK[2].set(-100, 420, "graph/stripe1.png", 1);
		// mrK[3].set(-100, 395, "graph/stripe3.png", 1);
		// deer.set(1480, 200, "graph/stripe6.png", 1);
		initialize();
	}

	void initialize() {
		cnt = 0.0;
		init_ending_text(job, who);
	}

	int show(Music& music) {
		// �t�F�[�h�C��
		fadeinMusic(music);
		fadeinTitle();

		//�X�i�b�v�V���b�g
		drawEndroll();

		//Mr.K
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//1�l��
		if (cnt > 1650.0 && cnt <= 2500.0) {
			mrK[0].set(cnt - 1750.0, 415);
			mrK[0].draw();
		}
		//2�l��
		if (cnt > 2550.0) {
			if (cnt <= 2900.0) {
				DrawGraph((cnt - 2650.0), 425, stripe[1], TRUE);
			}
			else if (cnt <= 2910.0) {
				DrawGraph(220.0 + 0.3 * pow(cnt - 2910.0, 2.0), 425, stripe[1], TRUE);
			}
			else if (cnt <= 2940.0) {
				DrawGraph(220.0 + (cnt - 2910.0), 425, stripe[1], TRUE);
			}
			else if (cnt <= 2950.0) {
				DrawGraph(220.0 + 0.3 * pow(cnt - 2950.0, 2.0), 425, stripe[1], TRUE);
			}
			else if (cnt <= 2980.0) {
				DrawGraph(220.0 + (cnt - 2950.0), 425, stripe[1], TRUE);
			}
			else if (cnt <= 2990.0) {
				DrawGraph(220.0 + 0.3 * pow(cnt - 2990.0, 2.0), 425, stripe[1], TRUE);
			}
			else if (cnt <= 3500.0) {
				DrawGraph(220.0 + (cnt - 2990.0), 425, stripe[1], TRUE);
			}
		}
		//3�l��
		if (cnt > 3600.0) {
			if (cnt <= 3900.0) {
				DrawGraph((cnt - 3700.0), 420, stripe[0], TRUE);
			}
			else if (cnt <= 4100.0) {
				DrawGraph(200, 420, stripe[0], TRUE);
			}
			else if (cnt <= 4600.0) {
				DrawGraph(200, 420.0 - 1.2 * (cnt - 4100.0), stripe[0], TRUE);
			}
		}
		//4�l��
		if (cnt > 4800.0) {
			if (cnt <= 5300.0) {
				DrawGraph((cnt - 4900.0), 395, stripe[2], TRUE);
			}
			else if (cnt <= 5450.0) {
				DrawGraph(400, 395, stripe[2], TRUE);
			}
			else if (cnt <= 5550.0) {
				if (int(cnt) % 10 < 5) {
					DrawGraph(400, 395, stripe[2], TRUE);
				}
				else {
					DrawGraph(400, 395, stripe[3], TRUE);
				}
			}
			else {
				DrawGraph(400, 395, stripe[2], TRUE);
			}
		}

		//��
		if (cnt <= 1200.0) {
			if (int(cnt) % 60 < 15) {
				DrawGraph(650.0 - (cnt - 830.0), 380, stripe[4], TRUE);
			}
			else if (int(cnt) % 60 < 30) {
				DrawGraph(650.0 - (cnt - 830.0), 380, stripe[5], TRUE);
			}
			else if (int(cnt) % 60 < 45) {
				DrawGraph(650.0 - (cnt - 830.0), 380, stripe[6], TRUE);
			}
			else {
				DrawGraph(650.0 - (cnt - 830.0), 380, stripe[7], TRUE);
			}
		}
		else if (cnt <= 3900.0) {
			if (int(cnt) % 60 < 15) {
				DrawGraph(280, 380, stripe[4], TRUE);
			}
			else if (int(cnt) % 60 < 30) {
				DrawGraph(280, 380, stripe[5], TRUE);
			}
			else if (int(cnt) % 60 < 45) {
				DrawGraph(280, 380, stripe[6], TRUE);
			}
			else {
				DrawGraph(280, 380, stripe[7], TRUE);
			}
		}
		//3�l��
		else if (cnt <= 4550.0) {
			if (cnt <= 4200.0) {
				DrawGraph(280, 380, stripe[4], TRUE);
			}
			else {
				DrawGraph(280, 380, stripe[8], TRUE);
			}
		}
		else if (cnt <= 5300.0) {
			if (int(cnt) % 60 < 15) {
				DrawGraph(280, 380, stripe[4], TRUE);
			}
			else if (int(cnt) % 60 < 30) {
				DrawGraph(280, 380, stripe[5], TRUE);
			}
			else if (int(cnt) % 60 < 45) {
				DrawGraph(280, 380, stripe[6], TRUE);
			}
			else {
				DrawGraph(280, 380, stripe[7], TRUE);
			}
		}
		//4�l��
		else if (cnt <= 5600.0) {
			if (cnt <= 5450.0) {
				DrawGraph(280, 380, stripe[4], TRUE);
			}
			else if (cnt <= 5550.0) {
				DrawGraph(280, 380, stripe[9], TRUE);
			}
			else {
				DrawGraph(280, 380, stripe[4], TRUE);
			}
		}
		else  if (cnt <= 6000.0) {
			if (int(cnt) % 40 < 10) {
				DrawGraph(280.0 - 1.2 * (cnt - 5600.0), 380, stripe[4], TRUE);
			}
			else if (int(cnt) % 40 < 20) {
				DrawGraph(280.0 - 1.2 * (cnt - 5600.0), 380, stripe[5], TRUE);
			}
			else if (int(cnt) % 40 < 30) {
				DrawGraph(280.0 - 1.2 * (cnt - 5600.0), 380, stripe[6], TRUE);
			}
			else {
				DrawGraph(280.0 - 1.2 * (cnt - 5600.0), 380, stripe[7], TRUE);
			}
		}

		//Fin
		drawFin();

		if (cnt <= 6500.0) cnt += 30.0 / FPS;

		return 0;
	}

	void fadeinMusic(Music& music) {
		if (cnt > 280.0 && cnt <= 900.0) {
			music.changeVolume(22.0 * pow(cnt - 250.0, 0.3));
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

	void debugDump(int debug) {
		if (debug) {
			int strColor = strColorDebug;

			DrawFormatString(365, 265, strColor, "endingCnt: %4.2f", cnt);
			DrawFormatString(365, 285, strColor, "endingCntInc: %4.2f", cntInc);
		}
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