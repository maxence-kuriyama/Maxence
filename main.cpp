#include "DxLib.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <float.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <mbstring.h>
#include <mbctype.h>

using namespace DxLib;
using namespace std;
#include "lib/fireflower.h"
#include "lib/logo.h"
#include "lib/scenario.h"
#include "lib/game.h"
#include "lib/ending.h"
#include "lib/music.h"
#include "lib/com.h"

#pragma comment(lib, "winmm.lib")

const int FLAG_TITLE(0);
const int FLAG_BATTLE(1);
const int FLAG_RESULT(2);
const int FLAG_DEMO_FIRST(-3);
const int FLAG_DEMO_SECOND(-2);
const int FLAG_DEMO_THIRD(-1);
const int FLAG_SCENARIO(-6);
const int FLAG_ENDING(-4);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);
	SetAlwaysRunFlag(TRUE);
	SetMainWindowText("Maxence 0.5.1");
	SetWindowIconID(101);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetBackgroundColor(0, 0, 0);	//�w�i�F
	SetCameraNearFar(100.0, 6000.0);
	SetGlobalAmbientLight(GetColorF(1.0, 0.0, 0.0, 0.0));
	ChangeLightTypePoint(VGet(320.0, 240.0, -300.0), 2000.0, 0.0, 0.001f, 0.0);
	int LightHandle = CreateDirLightHandle(VGet(0.0, 0.0, -1.0));
	srand((unsigned)time(NULL));


	Mouse mouse;
	Key key;
	Logo logo;
	Music bgm;
	Scenario scenario;
	Ending ending;
	fireflower tama[FIRE_FLOWER_NUM];

	Game game(&mouse, &key);
	int flg = FLAG_DEMO_FIRST;
	COM com;


	// ��X�̃n���h��
	unsigned int Green = GetColor(0, 255, 0);
	unsigned int Red = GetColor(255, 0, 0);
	unsigned int Blue = GetColor(0, 0, 255);
	unsigned int White = GetColor(255, 255, 255);
	unsigned int Black = GetColor(0, 0, 0);

	// �摜�ǂݍ���
	int MLogo = LoadGraph("graph/M.png");
	int axence = LoadGraph("graph/axence.png");
	int ClickToStart = LoadGraph("graph/click.png");
	int Logo0 = LoadGraph("graph/Maxence_after.png");
	int Logo1 = LoadGraph("graph/Maxence_after1.png");
	int Logo2 = LoadGraph("graph/Maxence_after2.png");
	int Logo3 = LoadGraph("graph/Maxence_after3.png");
	int Logo4 = LoadGraph("graph/Maxence_after4.png");
	double logoX = 0.0;		// �f����ʗp�ϐ�

	// 3D���f���֌W
	/*
	int ModelHandle = MV1LoadModel("movie/max0.mv1");
	float totalTime, playTime = 0.0;
	MV1SetPosition(ModelHandle, VGet(80.0, 150.0, 100.0));
	MV1SetScale(ModelHandle, VGet(0.1, 0.1, 0.1));
	//MV1SetRotationXYZ(ModelHandle, VGet(0.0, -0.5 * DX_PI_F, 0.0));
	MV1SetRotationXYZ(ModelHandle, VGet(0.0, DX_PI_F, 0.0));
	//int AttachIndex = MV1AttachAnim(ModelHandle, 1, -1, FALSE);
	//totalTime = MV1GetAttachAnimTotalTime(ModelHandle, AttachIndex);
	//int GrHandle = MV1GetTextureGraphHandle(ModelHandle, 0);
	*/
	double theta = 0.3;

	int vict = 0;			// ���s�i�[�p�̈ꎞ�ϐ�
	double reward = 0.0;	// �Ֆʕ]���i�[�p�̈ꎞ�ϐ�


	//���C�����[�v
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
		// ���͏����擾
		key.update();
		mouse.update();

		// �ݒ��؂�ւ���
		game.toggleByKey(bgm);
		if (game.option.soundFlg == 1) {
			tama[0].sound = 1;
		}
		else {
			tama[0].sound = 0;
		}

		// �}�E�X���삩�ۂ��𔻒肷��
		game.toggleMouseOrKeyboard();

		// �G���f�B���O�̃f�o�b�O
		if (game.debugEndingFlg) {
			if (flg != FLAG_ENDING) {
				bgm.unloadAll();
				ending.initialize();
				flg = FLAG_ENDING;
			}
			else {
				bgm.unloadAll();
				flg = FLAG_TITLE;
			}
			game.debugEndingFlg = 0;
		}


		// OP�A�j���[�V���� ClickToStart�܂�
		if (flg == FLAG_DEMO_FIRST){
			SetBackgroundColor(0, 0, 0);	//�w�i�F
			if (logoX <= 120.0 ) {
				DrawExtendGraph(270, 170, 358, 260, MLogo, TRUE);
			}
			else if (logoX <= 200.0) {
				DrawExtendGraph(270 - 1.25*(logoX - 120), 170, 358 - 1.25*(logoX - 120), 260, MLogo, TRUE);
				DrawExtendGraph(350 - 1.25*(logoX - 120), 170, 358 + 1.65*(logoX - 120), 260, axence, TRUE);
			}
			else {
				DrawExtendGraph(170, 170, 258, 260, MLogo, TRUE);
				DrawExtendGraph(250, 170, 490, 260, axence, TRUE);
			}
			if (logoX >= 300.0) {
				DrawExtendGraph(200, 290, 460, 360, ClickToStart, TRUE);
			}
			if (logoX >= 300.0 && logoX <= 480.0) {
				DrawBox(0, 290, 655 - 4.0*(logoX - 300), 360, Black, TRUE);
				DrawBox(650 - 4.0*(logoX - 300), 342, 670 - 4.0*(logoX - 300), 345, White, TRUE);
			}
			logoX += 2.0;

			if (logoX > 480.0 || mouse.click() || key.onReturn()) {
				flg = FLAG_DEMO_SECOND;
				logoX = M_PI_2;
			}
		}
		// OP�A�j���[�V���� ClickToStart�_��
		else if (flg == FLAG_DEMO_SECOND) {
			DrawExtendGraph(170, 170, 258, 260, MLogo, TRUE);
			DrawExtendGraph(250, 170, 490, 260, axence, TRUE);

			logoX = (logoX + 0.05); if (logoX >= 360.0) logoX -= 360.0;
			SetDrawBright(155 + 100*sin(logoX), 155 + 100*sin(logoX), 155 + 100*sin(logoX));
			DrawExtendGraph(200, 290, 460, 360, ClickToStart, TRUE);
			SetDrawBright(255, 255, 255);

			if (mouse.click() || key.onReturn()) {
				flg = FLAG_DEMO_THIRD;
				SetBackgroundColor(0, 128, 128);	//�w�i�F
				SetDrawBright(255, 255, 255);
			}
		}
		// OP�A�j���[�V���� ���C�����S
		else if (flg == FLAG_DEMO_THIRD) {
			if (logoX <= 37.5) {
				DrawExtendGraph(160, 170, 490, 260, Logo0, TRUE);
			}
			else if (logoX <= 45.0) {
				DrawExtendGraph(160, 170, 490, 260, Logo1, TRUE);
			}
			else if (logoX <= 50.0) {
				DrawExtendGraph(160, 170, 490, 260, Logo2, TRUE);
			}
			else if (logoX <= 55.0) {
				DrawExtendGraph(160, 170, 490, 260, Logo3, TRUE);
			}
			else if (logoX <= 90.0) {
				DrawExtendGraph(160, 170, 490, 260, Logo4, TRUE);
			}
			else {
				DrawExtendGraph(160 + (rand() % 11) - 5.0, 170, 485 + (rand() % 11) - 5.0, 260, Logo4, TRUE);
			}
			if(logoX < 1000.0) logoX += 1.0;
			if (logoX > 120 || mouse.click() || key.onReturn()) {
				flg = FLAG_TITLE;
			}
		}
		else if (flg == FLAG_TITLE) {
			bgm.load("sound/bgm03.mp3");
			for (int i = 0; i < FIRE_FLOWER_NUM; ++i) {
				tama[i].draw();
				tama[i].tick();
			}
			DrawExtendGraph(160 + (rand() % 11) - 5.0, 170, 490 + (rand() % 11) - 5.0, 260, Logo4, TRUE);
			//�^�C�g����ʂ��̂P
			if (game.isVsHuman()) {
				int choice = game.menuChoose();
				if (choice == 0) {
					game.mode = "�ڂ�����";
					game.taijin = 1;
					game.setOrderMenu();
				}
				else if (choice == 1) {
					game.mode = "�ׂ̐l��";
					game.initialize();
					flg = FLAG_BATTLE;
					game.goBattle(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_YELLOW);
				}
			}
			//�^�C�g����ʂ��̂Q�i�u�ڂ����Łv�I�����j
			else if (game.isVsCOM()) {
				int choice = game.menuChoose();
				if (choice == 0) {
					flg = FLAG_SCENARIO;
					game.teban = TEBAN_SENKO;
				}
				else if (choice == 1) {
					flg = FLAG_SCENARIO;
					game.teban = TEBAN_KOKO;
				}
			}
		}
		else if (flg == FLAG_BATTLE) {
			// �Ֆʂ̕`��
			//MV1DrawModel(ModelHandle);
			SetBackgroundColor(0, 0, 0);	//�w�i�F
			game.drawBase();
			game.drawGlobalState();
			game.drawHistLast();
			game.drawNextField();
			// �v���C���[�̑���
			if (game.isPlayTurn() && game.playTurn()) {
				reward = game.update();
				if (reward > -10.0) {
					if (game.isVsCOM()) {
						com.setWait();
					}
					game.updateCommentInBattle();
				}
			}
			game.drawLocalState();
			game.drawCurrentCoord();
			game.drawPlayers();
			logo.draw();
			logo.update(key); // ����

			// ���b�Z�[�W�̕`��
			game.drawBattleMessage();

			// �w�K�@�B�̏o�͕`��
			if (game.option.likeliFlg >= 1) {
				com.visualize();
			}

			// COM�̎��
			if (!game.isPlayTurn()) {
				// VectorXd input = game.stateToInput();
				// com.play(input);
				com.playMinMax(game.board, game.currentSide());
				// �Ֆʂ̍X�V
				game.update(com.choice);
			}

			// �R�����g�̕`��
			game.drawComment();

			// �J�b�g�C���A�j���[�V����
			game.cutin.update();

			// ��������
			vict = game.victory();
			if (vict != 0) {
				flg = FLAG_RESULT;
				key.initWait();
			}

			// ����̎�����
			if (key.onBack() && game.goBackHist()) {
				com.setWait();
			}

			// �Z�[�u or ���Z�b�g
			int choice = game.menuChoose();
			if (choice == 0 || choice == 1) {
				flg = FLAG_TITLE;
				game.reset(bgm);
				for (int i = 0; i < FIRE_FLOWER_NUM; ++i) {
					tama[i].initialize();
				}
				scenario.initialize();
				SetBackgroundColor(0, 128, 128);
			}

			// �J��������
			game.camera.set();
			if (mouse.click()) {
				mouse.set();
			}
			if (mouse.onClick()) {
				theta -= (mouse.distDragX()) * 0.05;
				mouse.set();
			}
			//MV1SetRotationXYZ(ModelHandle, VGet(0.0, theta + DX_PI_F, 0.0));

			// �ΐ�X�L�b�v�i��l�p�f�o�b�O�j
			if (game.skipBattle(scenario.flg)) {
				flg = FLAG_SCENARIO;
			}

		}
		else if (flg == FLAG_RESULT) {
			// �Ֆʂ̕`��
			//MV1DrawModel(ModelHandle);
			SetBackgroundColor(0, 0, 0);	//�w�i�F
			game.drawBase();
			game.drawGlobalState();
			game.drawHistLast();
			game.drawNextField();
			game.drawLocalState();
			logo.draw();

			// ���b�Z�[�W�̕`��
			game.drawWinner(vict);
			game.btnAgain.display(mouse, game.option.strColor);
			if (game.btnAgain.isClicked(mouse) || key.onReturn()){
				game.initialize();
				flg = FLAG_BATTLE;
				game.goBattle();
			}

			// ����̎�����
			if (key.onBack() && game.goBackHist()) {
				flg = FLAG_BATTLE;
				game.goBattle();
			}

			// �J��������
			if (mouse.click()) {
				mouse.set();
			}
			else if (mouse.onClick()) {
				game.camera.move(mouse.distDragX(), mouse.distDragY());
				mouse.set();
			}
			game.camera.zoom(mouse.wheel);

			// TODO �V�i���I�̏ꍇ�̏I�� + �J�ڏ���
		}
		else if (flg == FLAG_ENDING) {
			bgm.load("sound/bgm09.mp3");
			SetBackgroundColor(0, 0, 0);	//�w�i�F
			game.drawBase();
			game.drawGlobalState();
			game.drawHistLast();
			game.drawNextField();
			game.drawLocalState();
			game.drawCurrentCoord();
			logo.draw();
			ending.show(bgm, game.fps);
		}
		else if (flg == FLAG_SCENARIO) {
			scenario.getKey(key);
			if (!bgm.drawLoadMsg() && scenario.show(mouse, bgm)) {
				game.initialize();
				flg = FLAG_BATTLE;
				game.goBattle(BATTLE_PLAYER_YELLOW, rand() % 3 + 1);
			}
		}


		// ��������
		game.sync();

		// �f�o�b�O���o��
		game.debugDump();
		bgm.debugDump(game.debugFlg);
		scenario.debugDump(game.debugFlg);
		ending.debugDump(game.debugFlg);
		com.debugDump(game.debugFlg);
	}

	InitGraph();
	//MV1DeleteModel(ModelHandle);
	DxLib_End();

	return 0;
}
