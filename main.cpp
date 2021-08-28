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
#include "lib/vect.h"

using namespace DxLib;
using namespace std;
#include "lib/const.h"
#include "lib/field.h"
#include "lib/hist.h"
#include "lib/fireflower.h"
#include "lib/scenario.h"
#include "lib/ending.h"
#include "lib/basic.h"
#include "lib/anime.h"
#include "lib/key_input.h"

#pragma comment(lib, "winmm.lib")

int GetTexts(string *text, const char* filename);
int MultiByteLength(const char* String);
//VectorXd StateToInput(int dim, int side);
//VectorXd Reward1(const VectorXd &out, const VectorXd &in, int side);
//VectorXd softmax(const VectorXd &src, double alpha);

//int trainCnt = 0;
//int comHistt[100];
void initializeTrain() {
	//trainCnt = 0;
	//for (int i = 0; i < 100; i++) {
		//comHistt[i] = 0;
	//}
}

int taijin = 0;			// 0: vsHuman, 1: vsCOM, 2: AutoLearning

class Game {
public:
	int flg = -3; 
	// -3,..,-1: Demo
	// 0: Menu, 1: Game, 2: Result
	// -4: Ending
	// -6: Story
	// 5: High-speed Learning
	int musicFlg = 1;
	int soundFlg = 1;
	int debugFlg = 0;
	int strColor = White;
	Field mother;
	Field child[3][3];
	History hist;
	Camera camera;
	Mouse mouse;
	Key key;
	Logo logo;
	Anime cutin;
	int nextField = -1;		// -1: anywhere
	int cnt = 0;			// ターン数
	int drawCnt = 0;		// 引き分け時の強制終了のためのカウント
	string mode = "";

	void initialize(int f = 1) {
		flg = f;
		cnt = 0;
		drawCnt = 0;
		nextField = -1;
		camera.initialize();
		mother.initialize();
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				child[i][j].initialize();
			}
		}
		hist.initialize();
		mouse.set();
		key.initWait();
	}

	double update(int global_x, int global_y, int local_x, int local_y, int side = 0) {
		if (side == 0) {
			side = 1 - 2 * (cnt % 2);
		}
		//盤面の更新
		if (nextField == 3 * global_x + global_y || nextField == -1) {
			if (child[global_x][global_y].update(local_x, local_y, side) == 0) {
				cnt++;
				//履歴を残す
				hist.add(global_x, global_y, local_x, local_y, nextField);
				//全体の更新
				mother.update(global_x, global_y, child[global_x][global_y].victory());
				if (child[local_x][local_y].victory() != 0) {
					nextField = -1;
					return RWD_DOM;
				}
				else {
					nextField = local_x * 3 + local_y;
					return RWD_PUT;
				}
			}
		}
		return -100.0;
	}

	//永遠に勝敗がつかない場合の処理
	void stopDrawGame() {
		if (drawCnt > 10000) {
			flg = 2;
		}
		if (taijin == 2 || flg == 5) {
			drawCnt++;
		}
	}

	void toggleByKey() {
		key.toggleSetting(logo, musicFlg, soundFlg, strColor);
		//key.configLearning();
		key.toggleDebug(debugFlg);
		key.toggleForDebug(cutin.flg);
	}

	void toggleHighSpeedLearning() {
		if (key.toggleHighSpeedLearning(flg) == 1) {
			mode = "オート";
			initialize(flg);
			initializeTrain();
		}
	}

	void toggleAutoLearning() {
		if (key.toggleAutoLearning(flg, taijin) == 1 || key.toggleHighSpeedLearning(flg) == 1) {
			mode = "オート";
			initialize(flg);
			initializeTrain();
		}
	}

	void skipBattle(int& sceneFlg) {
		key.skipBattle(flg, sceneFlg);
	}
};

Game game;


//一時記憶に用いる変数
int mindex[2];

int Font0, Font1, Font2, Font3, Font4;
VECTOR Origin = VGet(320.0, 240.0, 0.0);
VECTOR tmp;
//ゲームの処理に用いる変数
int Soloflg = 0;						// シナリオ管理用フラグ
int Scenflg = 0;						// シナリオ管理用フラグ
int teban = 0;							// 0:senko, 1:koko
int vict = 0;
int keyboardFlg = 0;					// 0:マウス操作, 1:キーボード操作
int corGx = 1;
int corGy = 1;
int corLx = 1;
int corLy = 1;							//キーボード操作時の座標
int COMGx = 1;
int COMGy = 1;
int COMLx = 1;
int COMLy = 1;							//COMの選ぶ座標
int selectMode = 0;
int COMWait = 0;
int waitOnCOM = 20;						//COMが手を打つまでのウェイト
int prex = 0; 
int prey = 0;				//マウス操作か否かを判定するための変数
//ゲームの演出に用いる変数
double logoX = 0.0;
int windowFlg = 1;
int likeliFlg = 0;
int commentFlg = 0;
string text[450]; 
int txtMax = 450;
char txt_name[30] = "data/sayings.txt";
int numTxt = 0; 
int txtX = rnd() % 200; 
int txtY = rnd() % 400;
int txtId = 0; int txtCnt = 0;			//テキストのインデックスとテキスト差し替えのカウンタ
int txtSeq = 0;							//テキストの連番がいくつ継続したかのカウンタ
//char SEname[8][20] = { "sound/se_amb01.wav" ,"sound/se_amb02.wav" ,"sound/se_amb03.wav" ,
		//"sound/se_amb04.wav" ,"sound/se_amb05.wav" ,"sound/se_amb06.wav" ,"sound/se_amb07.wav" ,"sound/se_amb08.wav" };
int max_id = 0;
double max_val = 0.0;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);
	SetAlwaysRunFlag(TRUE);
	SetMainWindowText("Maxence 0.4.0");
	SetWindowIconID(101);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetBackgroundColor(0, 0, 0);	//背景色
	SetCameraNearFar(100.0, 6000.0);
	SetGlobalAmbientLight(GetColorF(1.0, 0.0, 0.0, 0.0));
	ChangeLightTypePoint(VGet(320.0, 240.0, -300.0), 2000.0, 0.0, 0.001f, 0.0);
	int LightHandle = CreateDirLightHandle(VGet(0.0, 0.0, -1.0));

	// 種々のハンドル
	unsigned int Green = GetColor(0, 255, 0);
	unsigned int Red = GetColor(255, 0, 0);
	unsigned int Blue = GetColor(0, 0, 255);
	unsigned int col = Green;

	// フォント
	Font0 = CreateFontToHandle("HG教科書体", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
	Font1 = CreateFontToHandle("Times New Roman", 10, 1, -1);
	Font2 = CreateFontToHandle("HG教科書体", 36, 4, DX_FONTTYPE_ANTIALIASING_EDGE);
	Font3 = CreateFontToHandle("HG教科書体", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
	Font4 = CreateFontToHandle("Times New Roman", 72, 6, DX_FONTTYPE_ANTIALIASING_EDGE);

	// 画像読み込み
	int MLogo = LoadGraph("graph/M.png");
	int axence = LoadGraph("graph/axence.png");
	int ClickToStart = LoadGraph("graph/click.png");
	int Logo0 = LoadGraph("graph/Maxence_after.png");
	int Logo1 = LoadGraph("graph/Maxence_after1.png");
	int Logo2 = LoadGraph("graph/Maxence_after2.png");
	int Logo3 = LoadGraph("graph/Maxence_after3.png");
	int Logo4 = LoadGraph("graph/Maxence_after4.png");
	game.logo.image = Logo4;
	int Room = LoadGraph("graph/room.bmp");
	int Card = LoadGraph("graph/card.bmp");
	int Cutin1 = LoadGraph("graph/cutin1.png");
	int Cutin10 = LoadGraph("graph/cutin10.png");
	game.cutin.image0 = Cutin1;
	game.cutin.image1 = Cutin10;
	GraphBlend(Cutin1, Cutin10, 255, DX_GRAPH_BLEND_MULTIPLE);
	int stone1 = LoadGraph("graph/stone1.png");
	int stone2 = LoadGraph("graph/stone2.png");
	int stone1_t = LoadGraph("graph/stone1.png");
	int stone2_t = LoadGraph("graph/stone2.png");
	int stripe[15];
	for (int i = 1; i <= 15; ++i) {
		string pict_name;
		pict_name = "graph/stripe" + to_string(i) + ".png";
		stripe[i-1] = LoadGraph(pict_name.c_str());
	}
	int end_pict[20];
	for (int i = 1; i <= 20; ++i) {
		string pict_name;
		pict_name = "graph/end_pict" + to_string(i) + ".png";
		end_pict[i-1] = LoadGraph(pict_name.c_str());
	}

	// 動画
	//int MovieGraphHandle = LoadGraph("movie/battle.ogv");

	// 3Dモデル関係
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

	srand((unsigned)time(NULL));
	fireflower tama[3]; tama[0].sound = 1;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			game.child[i][j].stone1 = stone1;
			game.child[i][j].stone2 = stone2;
			game.child[i][j].stone1_t = stone1_t;
			game.child[i][j].stone2_t = stone2_t;
		}
	}
	VECTOR CentK = VGet(150.0, 200.0, 0.0);
	double theta = 0.3;
	numTxt = GetTexts(text, txt_name);

	//時間関連
	long current = clock();
	long start = clock();
	int cur_fps = 0;
	int fps_cnt = 0;
	long fps_start = clock();

	//シナリオ関係
	int scen_char_cnt = 0;
	int scen_txt_cnt = 0;
	int scen_txt_len = 0;
	int eqx = 0; int eqy = 0;
	int visible[5] = { 1,1,1,1,0 };
	string scen_txt[40];
	int scen_who[40];
	init_scene_text(scen_txt, scen_who);

	//エンディング関係
	int end_cnt = 0;
	string job[20];
	string who[20];
	init_ending_text(job, who);

	////学習関連
	//int train_game_cnt = 0;
	//int train_turn_cnt = 0;
	//int train_correct_cnt = 0;
	//int epic = 0;
	//int game_per_epic = 20;
	//int max_epic = 100;
	//double loss_per_epic[100] = { 0.0 };
	//int correct_per_epic[100] = { 0 };
	//int turn_per_epic[100] = { 0 };
	//double tmp_loss = 0.0;
	//int dbg_cnt = 0;

	////学習機械関連
	//int lay_len = 3;//6;
	//int lay_size[4] = { 162, 800, 400, 81 };
	////int lay_size[7] = { 162, 1000, 2000, 2000, 1000, 500, 81 };
	//VectorXd input(lay_size[0]);
	//VectorXd output;
	//VectorXd p_output;
	//double eps = 0.002; //学習定数
	//double gamma = 0.95; //割引率
	//double mom = 0.9;
	//double varc = 0.999;	// adamのパラメータ
	//VectorXd temp_i[100];
	//VectorXd temp_o[100];	//学習用データの一時保存用ベクトル
	//MatrixXd train_i;
	//MatrixXd train_o;		//バッチ学習用のデザイン行列
	double reward2, rwd_tmp;
	//double anl_rate = 0.0;	//epsilon-greedyの割合
	//double alpha = 2.1;	//softmaxの係数
	int anl_flg = 0;

	//// MLPの初期化
	//MatrixXd P1 = MatrixXd::Random(lay_size[0], lay_size[1]);
	//VectorXd B1 = VectorXd::Random(lay_size[1]);
	//MatrixXd P2 = MatrixXd::Random(lay_size[1], lay_size[2]);
	//VectorXd B2 = VectorXd::Random(lay_size[2]);
	//MatrixXd P3 = MatrixXd::Random(lay_size[2], lay_size[3]);
	//VectorXd B3 = VectorXd::Random(lay_size[3]);
	////MatrixXd P4 = MatrixXd::Random(lay_size[3], lay_size[4]);
	////VectorXd B4 = VectorXd::Random(lay_size[4]);
	////MatrixXd P5 = MatrixXd::Random(lay_size[4], lay_size[5]);
	////VectorXd B5 = VectorXd::Random(lay_size[5]);
	////MatrixXd P6 = MatrixXd::Random(lay_size[5], lay_size[6]);
	////VectorXd B6 = VectorXd::Random(lay_size[6]);
	//P1 = P1 * sqrt(0.1 / lay_size[0]);
	//B1 = B1 * 0.1;
	//P2 = P2 * sqrt(0.1 / lay_size[1]);
	//B2 = B2 * 0.1;
	//P3 = P3 * sqrt(0.1 / lay_size[2]);
	//B3 = B3 * 0.05;
	////P4 = P4 * sqrt(0.1 / lay_size[3]);
	////B4 = B4 * 0.05;
	////P5 = P5 * sqrt(0.1 / lay_size[4]);
	////B5 = B5 * 0.05;
	////P6 = P6 * sqrt(0.1 / lay_size[5]);
	////B6 = B6 * 0.05;
	//::Affine Q1(P1, B1, true, mom, varc);
	//::Affine Q2(P2, B2, true, mom, varc);
	//::Affine Q3(P3, B3, true, mom, varc);
	////::Affine Q4(P4, B4, true, mom, varc);
	////::Affine Q5(P5, B5, true, mom, varc);
	////::Affine Q6(P6, B6, true, mom, varc);
	//ActLayer R1("relu");
	//ActLayer R2("relu");
	////ActLayer R3("relu");
	////ActLayer R4("relu");
	////ActLayer R5("relu");
	//Machine critic(5, IdentityV, eps, "adam");
	////Machine critic(11, teacher, eps, "adam");
	//critic.setLayer(Q1, 0);
	//critic.setLayer(R1, 1);
	//critic.setLayer(Q2, 2);
	//critic.setLayer(R2, 3);
	//critic.setLayer(Q3, 4);
	////critic.setLayer(R3, 5);
	////critic.setLayer(Q4, 6);
	////critic.setLayer(R4, 7);
	////critic.setLayer(Q5, 8);
	////critic.setLayer(R5, 9);
	////critic.setLayer(Q6, 10);


	//メインループ
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
		game.key.update();
		game.mouse.update();

		//game.flg > 0 でリセットボタンを表示する
		if (game.flg > 0) {
			if (game.mouse.onButton(game.logo.titleX, game.logo.titleY - 5, game.logo.titleX + 185, game.logo.titleY + 65)) {
				DrawBox(game.logo.titleX, game.logo.titleY - 5, game.logo.titleX + 185, game.logo.titleY + 65, GetColor(20, 150, 150), TRUE);
				if (game.mouse.button[0] == 1) {
					game.mouse.set();
					game.flg = 0;
					taijin = 0;
					for (int i = 0; i < 3; ++i) {
						tama[i].initialize();
					}
					StopMusic();
				}
			}
		}


		game.toggleByKey();
		if (game.soundFlg == 1) {
			tama[0].sound = 1;
		}
		else {
			tama[0].sound = 0;
		}


		//マウス操作か否かを判定する
		if (game.mouse.x != prex || game.mouse.y != prey) {
			keyboardFlg = 0;
		}
		prex = game.mouse.x;
		prey = game.mouse.y;

		//OPアニメーション ClickToStartまで
		if (game.flg == -3){
			SetBackgroundColor(0, 0, 0);	//背景色
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

			if (logoX > 480.0 || game.mouse.button[0] == 1 || game.key.onReturn()) {
				game.flg = -2;
				logoX = M_PI_2;
			}
		}
		//OPアニメーション ClickToStart点滅
		else if (game.flg == -2) {
			DrawExtendGraph(170, 170, 258, 260, MLogo, TRUE);
			DrawExtendGraph(250, 170, 490, 260, axence, TRUE);

			logoX = (logoX + 0.05); if (logoX >= 360.0) logoX -= 360.0;
			SetDrawBright(155 + 100*sin(logoX), 155 + 100*sin(logoX), 155 + 100*sin(logoX));
			DrawExtendGraph(200, 290, 460, 360, ClickToStart, TRUE);
			SetDrawBright(255, 255, 255);

			if (game.mouse.button[0] == 1 || game.key.onReturn()) {
				game.flg = -1;
				SetBackgroundColor(0, 128, 128);	//背景色
				SetDrawBright(255, 255, 255);
			}
		}
		//OPアニメーション メインロゴ
		else if (game.flg == -1) {
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
			if (logoX > 120 || game.mouse.button[0] == 1 || game.key.onReturn()) {
				game.flg = 0;
			}
		}
		//タイトル画面
		else if (game.flg == 0) {
			for (int i = 0; i < 3; ++i) {
				tama[i].draw();
				tama[i].tick();
			}
			DrawExtendGraph(160 + (rand() % 11) - 5.0, 170, 490 + (rand() % 11) - 5.0, 260, Logo4, TRUE);
			//タイトル画面その１
			if (taijin == 0) {
				DrawFormatString(TEXT1_X, TEXT1_Y, game.strColor, "ぼっちで");
				DrawFormatString(TEXT2_X, TEXT2_Y, game.strColor, "隣の人と");
				if ((!keyboardFlg && game.mouse.onButton(TEXT1_X - 16, TEXT1_Y - 16, TEXT1_X + 80, TEXT1_Y + 24))
					|| (keyboardFlg && selectMode == 0)) {
					DrawFormatString(TEXT1_X, TEXT1_Y, Red, "ぼっちで");
					if ((!keyboardFlg && game.mouse.button[0] == 1) || (keyboardFlg && game.key.onReturn())) {
						game.mode = "ぼっちで";
						//InitializeGame();
						taijin = 1;
					}
				}
				else if ((!keyboardFlg && game.mouse.onButton(TEXT2_X - 16, TEXT2_Y - 16, TEXT2_X + 80, TEXT2_Y + 24))
					|| (keyboardFlg && selectMode == 1)) {
					DrawFormatString(TEXT2_X, TEXT2_Y, Red, "隣の人と");
					if ((!keyboardFlg && game.mouse.button[0] == 1) || (keyboardFlg && game.key.onReturn())) {
						game.mode = "隣の人と";
						////PlayMovie("movie/battle.ogv", 1, DX_MOVIEPLAYTYPE_NORMAL);
						//PlayMovieToGraph(MovieGraphHandle);
						//SetBackgroundColor(0, 0, 0);
						//while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() 
						//	&& GetMovieStateToGraph(MovieGraphHandle)){
						//	UpdateKey(Key);
						//	if (Key[KEY_INPUT_W] == 1) {
						//		//PauseMovieToGraph(MovieGraphHandle);
						//		break;
						//	}
						//	//DrawExtendGraph(0, 60, 640, 420, MovieGraphHandle, FALSE);
						//	WaitTimer(10);
						//}
						SetBackgroundColor(0, 128, 128);
						game.initialize();
						initializeTrain();
					}
				}
				if (game.key.onLeft()) {
					selectMode = ((selectMode - 1) + 2) % 2;
					keyboardFlg = 1;
				}
				if (game.key.onRight()) {
					selectMode = (selectMode + 1) % 2;
					keyboardFlg = 1;
				}
				//オートモード
				game.toggleAutoLearning();
			}
			//タイトル画面その２（「ぼっちで」選択時）
			else if (taijin == 1) {
				DrawFormatString(TEXT1_X, TEXT1_Y, game.strColor, "先攻");
				DrawFormatString(TEXT2_X, TEXT2_Y, game.strColor, "後攻");
				if ((!keyboardFlg && game.mouse.onButton(TEXT1_X - 16, TEXT1_Y - 16, TEXT1_X + 80, TEXT1_Y + 24))
					|| (keyboardFlg && selectMode == 0)) {
					DrawFormatString(TEXT1_X, TEXT1_Y, Red, "先攻");
					if ((!keyboardFlg && game.mouse.button[0] == 1) || (keyboardFlg && game.key.onReturn())) {
						SetBackgroundColor(0, 0, 0);
						SetBackgroundColor(0, 128, 128);
						game.flg = -6;
						teban = 0;
					}
				}
				else if ((!keyboardFlg && game.mouse.onButton(TEXT2_X - 16, TEXT2_Y - 16, TEXT2_X + 80, TEXT2_Y + 24))
					|| (keyboardFlg && selectMode == 1)) {
					DrawFormatString(TEXT2_X, TEXT2_Y, Red, "後攻");
					if ((!keyboardFlg && game.mouse.button[0] == 1) || (keyboardFlg && game.key.onReturn())) {
						SetBackgroundColor(0, 0, 0);
						SetBackgroundColor(0, 128, 128);
						game.flg = -6;
						teban = 1;
					}
				}
			}
		}
		//Game Loop
		else if (game.flg == 1) {
			//描画
			//MV1DrawModel(ModelHandle);
			DrawBox(160, 80, 460, 380, GetColor(255, 255, 245), TRUE);
			if (game.hist.canCancel()) {
				DrawBox(160 + 100 * game.hist.last[0] + 33 * game.hist.last[2], 80 + 100 * game.hist.last[1] + 33 * game.hist.last[3],
					160 + 100 * game.hist.last[0] + 33 * (game.hist.last[2] + 1), 80 + 100 * game.hist.last[1] + 33 * (game.hist.last[3] + 1), GetColor(255, 160, 160), TRUE);
			}
			//操作の処理
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					if (game.mother.state[i][j] == 1) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), GetColor(130, 70, 70), TRUE);
					}
					else if (game.mother.state[i][j] == -1) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), GetColor(70, 70, 130), TRUE);
					}
					else if (game.mother.state[i][j] != 0) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), GetColor(70, 130, 70), TRUE);
					}
					DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), Black, FALSE);
					if (game.nextField == 3 * i + j) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), Red, FALSE);
						DrawBox(160 + 100 * i + 1, 80 + 100 * j + 1, 160 + 100 * (i + 1) - 1, 80 + 100 * (j + 1) - 1, Red, FALSE);
						DrawBox(160 + 100 * i + 2, 80 + 100 * j + 2, 160 + 100 * (i + 1) - 2, 80 + 100 * (j + 1) - 2, Red, FALSE);
					}
					else if (game.nextField == -1 && game.child[i][j].victory() == 0) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), Red, FALSE);
					}
					for (int k = 0; k < 3; ++k) {
						for (int l = 0; l < 3; ++l) {
							if (taijin == 0 || (taijin == 1 && game.cnt % 2 == teban)) {
								if (!keyboardFlg && game.mouse.onButton(160 + 100 * i + 33 * k, 80 + 100 * j + 33 * l,
									160 + 100 * i + 33 * (k + 1), 80 + 100 * j + 33 * (l + 1))) {
									corGx = i; corGy = j;
									corLx = k; corLy = l;
									if (game.mouse.button[1] == 1) {
										rwd_tmp = game.update(i, j, k, l);
										if (rwd_tmp > -10.0) {
											if (taijin == 1) {
												COMWait = waitOnCOM;
										//		reward2 = -rwd_tmp;
											}
										}
									}
								}
							}
							if (likeliFlg >= 1) {
								/*double col_tmp = min(max(240.0 * (output(27 * i + 9 * j + 3 * k + l) + 0.5), 0.0), 255.0);
								DrawCircle(160 + 100 * i + 33 * k + 16, 80 + 100 * j + 33 * l + 16, 15, GetColor(255, 255, 255 - col_tmp));
								if (likeliFlg == 2) {
									char str_tmp[10]; 
									sprintf_s(str_tmp, "%.4f", output(27 * i + 9 * j + 3 * k + l));
									DrawStringToHandle(160 + 100 * i + 33 * k + 2, 80 + 100 * j + 33 * l + 2, str_tmp, Red, Font1);
									if (trainCnt >= 1) {
										sprintf_s(str_tmp, "%.4f", temp_o[trainCnt - 1](27 * i + 9 * j + 3 * k + l));
										DrawStringToHandle(160 + 100 * i + 33 * k + 2, 80 + 100 * j + 33 * l + 12, str_tmp, Blue, Font1);
									}
								}*/
							}
							game.child[i][j].draw(176.5 + 100 * i, 96.5 + 100 * j, 33);
						}
					}
				}
			}
			//DrawFormatString(0, 100, StringColor, "m1: %d", meshNum);

			//ゲーム内操作
			if (taijin == 0 || (taijin == 1 && game.cnt % 2 == teban)) {
				if (game.key.onUp()) {
					corLy--;
					if (corLy < 0) {
						if (corGy > 0) {
							corGy--; corLy += 3;
						}
						else {
							corLy = 0;
						}
					}
					keyboardFlg = 1;
				}
				if (game.key.onDown()) {
					corLy++;
					if (corLy > 2) {
						if (corGy < 2) {
							corGy++; corLy -= 3;
						}
						else {
							corLy = 2;
						}
					}
					keyboardFlg = 1;
				}
				if (game.key.onLeft()) {
					corLx--;
					if (corLx < 0) {
						if (corGx > 0) {
							corGx--; corLx += 3;
						}
						else {
							corLx = 0;
						}
					}
					keyboardFlg = 1;
				}
				if (game.key.onRight()) {
					corLx++;
					if (corLx > 2) {
						if (corGx < 2) {
							corGx++; corLx -= 3;
						}
						else {
							corLx = 2;
						}
					}
					keyboardFlg = 1;
				}
				if (keyboardFlg && game.key.onCheck()) {
					rwd_tmp = game.update(corGx, corGy, corLx, corLy);
					if (rwd_tmp > -10.0) {
						if (taijin == 1) {
					//		reward2 = -rwd_tmp;
						}
					}
				}
			}
			DrawBox(160 + 100 * corGx + 33 * corLx, 80 + 100 * corGy + 33 * corLy,
				160 + 100 * corGx + 33 * (corLx + 1), 80 + 100 * corGy + 33 * (corLy + 1), Black, FALSE);

			//COMの手番
			if ((taijin == 1  && game.cnt % 2 != teban) || taijin == 2) {
				//input = StateToInput(lay_size[0], 1 - 2 * (game.cnt % 2));
				//output = critic.predict(input);
				//max_val = output.maxCoeff(&max_id);
				//while (COMWait <= 0) {
				//	if (unif(mt) < anl_rate) {
				//		COMGx = rand() % 3; COMGy = rand() % 3;
				//		COMLx = rand() % 3; COMLy = rand() % 3;
				//		comHistt[trainCnt] = COMGx * 27 + COMGy * 9 + COMLx * 3 + COMLy;
				//		anl_flg = 1;
				//	}
				//	else {
				//		COMGx = (max_id / 27) % 3;
				//		COMGy = (max_id / 9) % 3;
				//		COMLx = (max_id / 3) % 3;
				//		COMLy = max_id % 3;
				//		comHistt[trainCnt] = max_id;
				//		anl_flg = 0;
				//	}
				//	//盤面の更新
				//	rwd_tmp = game.update(COMGx, COMGy, COMLx, COMLy);
				//	if (rwd_tmp > -10.0) {
				//		temp_i[trainCnt] = input;
				//		temp_o[trainCnt] = Reward1(output, input, 1 - 2 * (game.cnt % 2));
				//		temp_o[trainCnt](comHistt[trainCnt]) = rwd_tmp;
				//		if (trainCnt >= 1) {
				//			if (taijin == 1) {
				//				temp_o[trainCnt - 1](comHistt[trainCnt - 1]) += gamma * max_val + reward2;
				//			}
				//			else if (taijin == 2) {
				//				temp_o[trainCnt - 1](comHistt[trainCnt - 1]) -= rwd_tmp;
				//				if (trainCnt >= 2) {
				//					temp_o[trainCnt - 2](comHistt[trainCnt - 2]) += gamma * max_val;
				//				}
				//			}
				//		}
				//		trainCnt++;
				//		if (taijin == 2) COMWait = waitOnCOM;
				//		break;
				//	}
				//}
			}

			//カメラ操作
			if (game.mouse.button[0] == 1) {
				game.mouse.set();
			}
			tmp = VNorm(VSub(game.camera.pos, Origin));
			if (game.mouse.button[0] > 1) {
				theta -= (game.mouse.x - game.mouse.setx) * 0.05;
				game.mouse.set();
			}
			SetCameraPositionAndTarget_UpVecY(game.camera.pos, Origin);
			//MV1SetRotationXYZ(ModelHandle, VGet(0.0, theta + DX_PI_F, 0.0));

			//動作の取り消し
			if (game.key.onBack()) {
				if (game.hist.canCancel() && taijin == 0) {
					game.child[game.hist.last[0]][game.hist.last[1]].state[game.hist.last[2]][game.hist.last[3]] = 0;
					game.mother.state[game.hist.last[0]][game.hist.last[1]] = 0;
					game.mother.update(game.hist.last[0], game.hist.last[1], game.child[game.hist.last[0]][game.hist.last[1]].victory());
					game.nextField = game.hist.last[4];
					game.hist.goBack();
					game.cnt--;
					/*for (int i = 0; i < 3; ++i) {
						for (int j = 0; j < 3; ++j) {
							for (int k = 0; k < 3; ++k) {
								for (int l = 0; l < 3; ++l) {
									input(27 * i + 9 * j + 3 * k + l) = game.child[i][j].state[k][l];

								}
							}
						}
					}
					output = critic.predict(input);*/
					COMWait = waitOnCOM;
				}
			}

			//テキストの描画
			DrawFormatString(470, 80, game.strColor, "右クリック:");
			DrawFormatString(540, 100, game.strColor, "石を置く");
			DrawFormatString(470, 124, game.strColor, "zキー（BkSpキー）:");
			DrawFormatString(540, 144, game.strColor, "一手戻る");
			if (commentFlg) {
				DrawObtainsString2(txtX + 20, txtY + 10, 560, GetFontSize(), text[txtId].c_str(), game.strColor, Font0, GetColor(250, 250, 150));
			}
			if (game.debugFlg) {
				DrawFormatString(620, 0, game.strColor, "%d", waitOnCOM);
			}
			if (anl_flg) {
				DrawFormatString(540, 0, Green, "annealed!");
			}

			//カットインアニメーション
			game.cutin.update();

			// テキストの差し替え
			if (txtCnt > 200) {
				//ある程度連番が続くように設定
				if (txtId < numTxt && (rnd() % 10000) / 10000.0 < pow(0.95,pow(2.0,txtSeq))) {
					txtId++;
					txtSeq++;
				}
				else {
					txtId = rnd() % numTxt;
					txtSeq = 0;
				}
				txtCnt = 0;
				txtX = rnd() % 200;
				txtY = rnd() % 400;
			}
			txtCnt++;

			// 勝利判定
			vict = game.mother.victory();
			if (vict != 0) {
				game.flg = 2;
				game.key.initWait();
				//学習
				/*if (taijin == 1) {
					if (vict == teban * 2 - 1) {
						temp_o[trainCnt - 1](comHistt[trainCnt - 1]) = RWD_VICT;
					}else {
						temp_o[trainCnt - 1](comHistt[trainCnt - 1]) = -RWD_VICT;
					}
				}
				else if (taijin == 2) {
					temp_o[trainCnt - 2](comHistt[trainCnt - 2]) = -RWD_VICT;
					temp_o[trainCnt - 1](comHistt[trainCnt - 1]) = RWD_VICT;
				}
				train_i.setZero(trainCnt, lay_size[0]);
				train_o.setZero(trainCnt, lay_size[lay_len]);
				for (int i = 0; i < trainCnt; ++i) {
					train_i.block(i, 0, 1, lay_size[0]) = temp_i[i].transpose();
					train_o.block(i, 0, 1, lay_size[lay_len]) = temp_o[i].transpose();
				}
				if(taijin == 1 || taijin == 2) critic.backprop(train_i, train_o);*/
			}

			// 永遠に勝敗がつかない場合の処理
			game.stopDrawGame();

			// 高速学習モード
			game.toggleHighSpeedLearning();

			// 対戦スキップ（一人用デバッグ）
			if (taijin == 1) {
				game.skipBattle(Scenflg);
			}

		}
		//勝敗表示
		else if (game.flg == 2) {
			DrawBox(160, 80, 460, 380, GetColor(255, 255, 245), TRUE);
			if (game.hist.canCancel()) {
				/*if (game.child[game.hist.last[0]][game.hist.last[1]].state[game.hist.last[2]][game.hist.last[3]] == 1) {
					DrawExtendGraph(176.5 + 100 * game.hist.last[0] + 33 * game.hist.last[2] - 15, 96.5 + 100 * game.hist.last[1] + 33 * game.hist.last[3] - 15,
						176.5 + 100 * game.hist.last[0] + 33 * game.hist.last[2] + 15, 96.5 + 100 * game.hist.last[1] + 33 * game.hist.last[3] + 15, stone1_t, TRUE);
				}
				else if (game.child[game.hist.last[0]][game.hist.last[1]].state[game.hist.last[2]][game.hist.last[3]] == -1) {
					DrawExtendGraph(176.5 + 100 * game.hist.last[0] + 33 * game.hist.last[2] - 15, 96.5 + 100 * game.hist.last[1] + 33 * game.hist.last[3] - 15,
						176.5 + 100 * game.hist.last[0] + 33 * game.hist.last[2] + 15, 96.5 + 100 * game.hist.last[1] + 33 * game.hist.last[3] + 15, stone2_t, TRUE);
				}*/
				DrawBox(160 + 100 * game.hist.last[0] + 33 * game.hist.last[2], 80 + 100 * game.hist.last[1] + 33 * game.hist.last[3],
					160 + 100 * game.hist.last[0] + 33 * (game.hist.last[2] + 1), 80 + 100 * game.hist.last[1] + 33 * (game.hist.last[3] + 1), GetColor(255, 200, 200), TRUE);
			}
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					if (game.mother.state[i][j] == 1) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), GetColor(130, 70, 70), TRUE);
					}
					else if (game.mother.state[i][j] == -1) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), GetColor(70, 70, 130), TRUE);
					}
					else if (game.mother.state[i][j] != 0) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), GetColor(70, 130, 70), TRUE);
					}
					DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), Black, FALSE);
					if (game.nextField == 3 * i + j) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), Red, FALSE);
						DrawBox(160 + 100 * i + 1, 80 + 100 * j + 1, 160 + 100 * (i + 1) - 1, 80 + 100 * (j + 1) - 1, Red, FALSE);
					}
					else if (game.nextField == -1 && game.child[i][j].victory() == 0) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), Red, FALSE);
					}
					for (int k = 0; k < 3; ++k) {
						for (int l = 0; l < 3; ++l) {
							game.child[i][j].draw(176.5 + 100 * i, 96.5 + 100 * j, 33);
						}
					}
				}
			}
			//if (BkFlg) DrawGraph(versionX, versionY, BkGraphHandle, TRUE);

			//カメラ操作
			if (game.mouse.button[0] == 1) {
				game.mouse.set();
			}
			tmp = VNorm(VSub(game.camera.pos, Origin));
			if (game.mouse.button[0] > 1) {
				game.camera.move(game.mouse.x - game.mouse.setx, game.mouse.y - game.mouse.sety, Origin);
				game.mouse.set();
			}
			game.camera.pos.x -= tmp.x * game.mouse.wheel * 5.0;
			game.camera.pos.z -= tmp.z * game.mouse.wheel * 5.0;
			SetCameraPositionAndTarget_UpVecY(game.camera.pos, Origin);

			//動作の取り消し
			if (game.key.onBack()) {
				if (game.hist.canCancel()) {
					game.child[game.hist.last[0]][game.hist.last[1]].state[game.hist.last[2]][game.hist.last[3]] = 0;
					game.mother.state[game.hist.last[0]][game.hist.last[1]] = 0;
					game.mother.update(game.hist.last[0], game.hist.last[1], game.child[game.hist.last[0]][game.hist.last[1]].victory());
					game.nextField = game.hist.last[4];
					game.hist.goBack();
					game.cnt--;
					game.flg = 1;
				}
			}

			//文字の描画
			if (vict == 1) {
				DrawFormatString(20, 20, Red, "Player 1");
			}
			else if (vict == -1) {
				DrawFormatString(20, 20, Black, "Player 2");
			}
			else {
				DrawFormatString(20, 20, Green, "No one");
			}
			DrawFormatString(20 + 96, 20, Green, "Won");
			DrawFormatString(20 + 24, 20 + 24, game.strColor, "もう一回");
			if (game.mouse.onButton(20 + 24 - 8, 20 + 24 - 8, 20 + 24 + 88, 20 + 24 + 24) || game.key.onReturn()) {
				DrawFormatString(20 + 24, 20 + 24, Red, "もう一回");
				if (game.mouse.button[0] == 1 || game.key.onReturn()) {
					game.initialize();
					initializeTrain();
				}
			}

			if (taijin == 2) {
				game.initialize();
				initializeTrain();
			}
		}
		//Ending
		else if (game.flg == -4) {
			DrawBox(160, 80, 460, 380, GetColor(255, 255, 245), TRUE);
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					if (game.mother.state[i][j] == 1) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), GetColor(130, 70, 70), TRUE);
					}
					else if (game.mother.state[i][j] == -1) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), GetColor(70, 70, 130), TRUE);
					}
					else if (game.mother.state[i][j] != 0) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), GetColor(70, 130, 70), TRUE);
					}
					DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), Black, FALSE);
					if (game.nextField == 3 * i + j) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), Red, FALSE);
						DrawBox(160 + 100 * i + 1, 80 + 100 * j + 1, 160 + 100 * (i + 1) - 1, 80 + 100 * (j + 1) - 1, Red, FALSE);
					}
					else if (game.nextField == -1 && game.child[i][j].victory() == 0) {
						DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), Red, FALSE);
					}
					for (int k = 0; k < 3; ++k) {
						for (int l = 0; l < 3; ++l) {
							game.child[i][j].draw(176.5 + 100 * i, 96.5 + 100 * j, 33);
						}
					}
				}
			}
			game.logo.draw();

			double fade_tmp = min(255.0, 0.01 * pow(max(0.0, end_cnt - 50.0), 2.0));
			SetDrawBlendMode(DX_BLENDMODE_SUB, fade_tmp);
			DrawBox(-1, -1, 641, 481, GetColor(255, 255, 255), TRUE);
			if (end_cnt > 280 && end_cnt <= 900) {
				SetVolumeMusic(26.0 * pow(end_cnt - 250.0, 0.3));
				if (CheckMusic() != 1) PlayMusic("sound/deer.mp3", DX_PLAYTYPE_BACK);
			}
			if (end_cnt <= 300) {
				SetDrawBlendMode(DX_BLENDMODE_ADD, fade_tmp);
				DrawExtendGraph(170, 170, 255, 260, MLogo, TRUE);
				DrawExtendGraph(250, 170, 490, 260, axence, TRUE);
			}
			else if (end_cnt <= 550) {
				SetDrawBlendMode(DX_BLENDMODE_ADD, fade_tmp);
				DrawExtendGraph(170, 170 - 1.2 * (end_cnt - 300.0),
					255, 260 - 1.2 * (end_cnt - 300.0), MLogo, TRUE);
				DrawExtendGraph(250, 170 - 1.2 * (end_cnt - 300.0),
					490, 260 - 1.2 * (end_cnt - 300.0), axence, TRUE);
			}
			//スナップショット
			for (int i = 0; i < 8; ++i){
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				if (end_cnt > 640 * i + 550 && end_cnt <= 640 * (i + 1) + 550) {
					DrawExtendGraph(
						280, 
						480 - 1.2 * (end_cnt - 640 * i - 550),
						600, 
						720 - 1.2 * (end_cnt - 640 * i - 550),
						end_pict[2 * i], TRUE);
					DrawStringToHandle(
						20, 520 - 1.2 * (end_cnt - 640 * i - 550),
						job[2 * i].c_str(), White, Font2);
					DrawStringToHandle(
						20, 620 - 1.2 * (end_cnt - 640 * i - 550),
						who[2 * i].c_str(), White, Font3);
				}
				if (end_cnt > 640 * i + 870 && end_cnt <= 640 * (i + 1) + 870) {
					DrawExtendGraph(
						40, 
						480 - 1.2 * (end_cnt - 640 * i - 870),
						360, 
						720 - 1.2 * (end_cnt - 640 * i - 870),
						end_pict[2 * i + 1], TRUE);
					DrawStringToHandle(
						370, 520 - 1.2 * (end_cnt - 640 * i - 870),
						job[2 * i + 1].c_str(), White, Font2);
					DrawStringToHandle(
						370, 620 - 1.2 * (end_cnt - 640 * i - 870),
						who[2 * i + 1].c_str(), White, Font3);
				}
			}

			//Mr.K
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				//1人目
			if (end_cnt > 1650 && end_cnt <= 2500) {
				DrawGraph((end_cnt - 1750), 415, stripe[0], TRUE);
			}
				//2人目
			if (end_cnt > 2550){
				if (end_cnt <= 2900) {
					DrawGraph((end_cnt - 2650), 425, stripe[1], TRUE);
				}
				else if (end_cnt <= 2910) {
					DrawGraph(220 + 0.3 * pow(end_cnt - 2910, 2.0), 425, stripe[1], TRUE);
				}
				else if (end_cnt <= 2940) {
					DrawGraph(220 + (end_cnt - 2910), 425, stripe[1], TRUE);
				}
				else if (end_cnt <= 2950) {
					DrawGraph(220 + 0.3 * pow(end_cnt - 2950, 2.0), 425, stripe[1], TRUE);
				}
				else if (end_cnt <= 2980) {
					DrawGraph(220 + (end_cnt - 2950), 425, stripe[1], TRUE);
				}
				else if (end_cnt <= 2990) {
					DrawGraph(220 + 0.3 * pow(end_cnt - 2990, 2.0), 425, stripe[1], TRUE);
				}
				else if (end_cnt <= 3500) {
					DrawGraph(220 + (end_cnt - 2990), 425, stripe[1], TRUE);
				}
			}
				//3人目
			if (end_cnt > 3600) {
				if (end_cnt <= 3900) {
					DrawGraph((end_cnt - 3700), 420, stripe[0], TRUE);
				}
				else if (end_cnt <= 4100) {
					DrawGraph(200, 420, stripe[0], TRUE);
				}
				else if (end_cnt <= 4600) {
					DrawGraph(200, 420 - 1.2 * (end_cnt - 4100), stripe[0], TRUE);
				}
			}
				//4人目
			if (end_cnt > 4800) {
				if (end_cnt <= 5300) {
					DrawGraph((end_cnt - 4900), 395, stripe[2], TRUE);
				}
				else if (end_cnt <= 5450) {
					DrawGraph(400, 395, stripe[2], TRUE);
				}
				else if (end_cnt <= 5550) {
					if (end_cnt % 10 < 5) {
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

			//鹿
			if (end_cnt <= 830) {
				//SetDrawBlendMode(DX_BLENDMODE_ADD, fade_tmp);
				//DrawGraph(450, 380, end_str[4], TRUE);
				//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			else if (end_cnt <= 1200) {
				if (end_cnt % 60 < 15) {
					DrawGraph(650 - (end_cnt - 830), 380, stripe[4], TRUE);
				}
				else if (end_cnt % 60 < 30) {
					DrawGraph(650 - (end_cnt - 830), 380, stripe[5], TRUE);
				}
				else if (end_cnt % 60 < 45) {
					DrawGraph(650 - (end_cnt - 830), 380, stripe[6], TRUE);
				}
				else {
					DrawGraph(650 - (end_cnt - 830), 380, stripe[7], TRUE);
				}
			}
			else if (end_cnt <= 3900) {
				if (end_cnt % 60 < 15) {
					DrawGraph(280, 380, stripe[4], TRUE);
				}
				else if(end_cnt % 60 < 30) {
					DrawGraph(280, 380, stripe[5], TRUE);
				}
				else if (end_cnt % 60 < 45) {
					DrawGraph(280, 380, stripe[6], TRUE);
				}
				else {
					DrawGraph(280, 380, stripe[7], TRUE);
				}
			}
				//3人目
			else if (end_cnt <= 4550) {
				if (end_cnt <= 4200) {
					DrawGraph(280, 380, stripe[4], TRUE);
				}
				else {
					DrawGraph(280, 380, stripe[8], TRUE);
				}
			}
			else if (end_cnt <= 5300) {
				if (end_cnt % 60 < 15) {
					DrawGraph(280, 380, stripe[4], TRUE);
				}
				else if (end_cnt % 60 < 30) {
					DrawGraph(280, 380, stripe[5], TRUE);
				}
				else if (end_cnt % 60 < 45) {
					DrawGraph(280, 380, stripe[6], TRUE);
				}
				else {
					DrawGraph(280, 380, stripe[7], TRUE);
				}
			}
				//4人目
			else if (end_cnt <= 5600) {
				if (end_cnt <= 5450) {
					DrawGraph(280, 380, stripe[4], TRUE);
				}
				else if (end_cnt <= 5550) {
					DrawGraph(280, 380, stripe[9], TRUE);
				}
				else {
					DrawGraph(280, 380, stripe[4], TRUE);
				}
			}
			else  if (end_cnt <= 6000) {
				if (end_cnt % 40 < 10) {
					DrawGraph(280 - 1.2 * (end_cnt - 5600), 380, stripe[4], TRUE);
				}
				else if (end_cnt % 40 < 20) {
					DrawGraph(280 - 1.2 * (end_cnt - 5600), 380, stripe[5], TRUE);
				}
				else if (end_cnt % 40 < 30) {
					DrawGraph(280 - 1.2 * (end_cnt - 5600), 380, stripe[6], TRUE);
				}
				else {
					DrawGraph(280 - 1.2 * (end_cnt - 5600), 380, stripe[7], TRUE);
				}
			}
			//Fin
			if (end_cnt > 5680){
				if (end_cnt <= 5950) {
					DrawStringToHandle(
						270, 520 - 1.2 * (end_cnt - 5680),
						"Fin", White, Font4);
				}else{
					DrawStringToHandle(
						270, 520 - 1.2 * 270,
						"Fin", White, Font4);
				}
			}
			
			if(end_cnt <= 6500) end_cnt++;
			DrawFormatString(5, 25, game.strColor, "%d", end_cnt);
		}
		//高速学習モード
		//else if (game.flg == 5) {
		//	if (debugFlg == 1) {
		//		//デバッグ
		//		if (Key[KEY_INPUT_D] == 1) {
		//			if (dbg_cnt == 0) printf("%d\n", trainCnt);
		//			for (int j = 0; j < 9; ++j) {
		//				printf("\n");
		//				for (int k = 0; k < 9; ++k) {
		//					printf("%.3f  ", train_i(dbg_cnt, j * 9 + k));
		//					//printf("%.1f  ", temp_i[1](j));
		//				}
		//			}
		//			for (int j = 0; j < 9; ++j) {
		//				printf("\n");
		//				for (int k = 0; k < 9; ++k) {
		//					printf("%.3f  ", train_o(dbg_cnt, j * 9 + k));
		//					//printf("%.1f  ", temp_o[1](j));
		//				}
		//			}
		//			printf("\n");
		//			dbg_cnt++;
		//		}
		//		if (dbg_cnt >= trainCnt) {
		//			dbg_cnt = 0;
		//			debugFlg = 2;
		//		}
		//	}
		//	else if (debugFlg == 0) {
		//		//操作の処理
		//
		//		while (true) {
		//			//COMの手番
		//			input = StateToInput(lay_size[0], 1 - 2 * (game.cnt % 2));
		//			output = critic.predict(input);
		//			max_val = output.maxCoeff(&max_id);
		//			//COMGx = (max_id / 27) % 3; COMGy = (max_id / 9) % 3;
		//			//COMLx = (max_id / 3) % 3; COMLy = max_id % 3;
		//			p_output = softmax(output, alpha);
		//			double cum_tmp = 0.0;
		//			double ran_tmp = unif(mt);
		//			for (int k = 0; k < 81; ++k) {
		//				cum_tmp += p_output(k);
		//				if (ran_tmp <= cum_tmp) {
		//					COMGx = (k / 27) % 3; COMGy = (k / 9) % 3;
		//					COMLx = (k / 3) % 3; COMLy = k % 3;
		//					break;
		//				}
		//			}
		//			if (unif(mt) >= anl_rate) {
		//				//盤面の更新
		//				rwd_tmp = game.update(COMGx, COMGy, COMLx, COMLy);
		//				if (rwd_tmp > -10.0) {
		//					//comHistt[trainCnt] = max_id;
		//					comHistt[trainCnt] = COMGx * 27 + COMGy * 9 + COMLx * 3 + COMLy;
		//					train_correct_cnt++;
		//					correct_per_epic[epic]++;
		//				}
		//			}
		//			else {
		//				rwd_tmp = -100.0;
		//			}
		//			if (rwd_tmp < -10.0) {
		//				while (true) {
		//					COMGx = rand() % 3; COMGy = rand() % 3;
		//					COMLx = rand() % 3; COMLy = rand() % 3;
		//					//盤面の更新
		//					rwd_tmp = game.update(COMGx, COMGy, COMLx, COMLy);
		//					if (rwd_tmp > -10.0) {
		//						comHistt[trainCnt] = COMGx * 27 + COMGy * 9 + COMLx * 3 + COMLy;
		//						break;
		//					}
		//					//永遠に勝敗がつかない場合の処理
		//					game.stopDrawGame();
		//				}
		//			}
		//			temp_i[trainCnt] = input;
		//			temp_o[trainCnt] = Reward1(output, input, 1 - 2 * (game.cnt % 2));
		//			temp_o[trainCnt](COM_game.hist[trainCnt]) = rwd_tmp;
		//			//if (trainCnt >= 1) temp_o[trainCnt - 1](comHistt[trainCnt - 1]) -= rwd_tmp;
		//			if (trainCnt >= 2) temp_o[trainCnt - 2](comHistt[trainCnt - 2]) += gamma * max_val;
		//			trainCnt++;
		//			game.cnt++;
		//			train_turn_cnt++;
		//			turn_per_epic[epic]++;
		//
		//			//勝利判定
		//			vict = game.mother.victory();
		//			if (vict != 0) break;
		//
		//			//永遠に勝敗がつかない場合の処理
		//			game.stopDrawGame();
		//		}
		//		//学習
		//		temp_o[trainCnt - 2](comHistt[trainCnt - 2]) = -RWD_VICT;
		//		temp_o[trainCnt - 1](comHistt[trainCnt - 1]) = RWD_VICT;
		//		train_i.setZero(trainCnt, lay_size[0]);
		//		train_o.setZero(trainCnt, lay_size[lay_len]);
		//		for (int i = 0; i < trainCnt; ++i) {
		//			train_i.block(i, 0, 1, lay_size[0]) = temp_i[i].transpose();
		//			train_o.block(i, 0, 1, lay_size[lay_len]) = temp_o[i].transpose();
		//		}
		//		critic.backprop(train_i, train_o);
		//		tmp_loss = critic.loss(train_i, train_o);
		//		loss_per_epic[epic] += tmp_loss;
		//
		//		//debugFlg = 1;
		//		InitializeGame(5);
		//
		//		//テキストの描画
		//		DrawFormatString(80, 400, StringColor, "試合数: %d", train_game_cnt);
		//		DrawFormatString(80, 424, StringColor, "試行手数: %d", train_turn_cnt);
		//		DrawFormatString(80, 448, StringColor, "正当率: %.2f%%", 100.0*(double)correct_per_epic[epic] / max(turn_per_epic[epic], 1));
		//		//DrawFormatString(240, 400, StringColor, "損失: %d", (int)tmp_loss);
		//		//DrawFormatString(240, 424, StringColor, "TPE: %d", turn_per_epic[epic]);
		//		DrawFormatString(240, 448, StringColor, "平均損失: %.2f", log(max(loss_per_epic[epic], 1.0)) - log(max(turn_per_epic[epic], 1.0)));
		//		DrawFormatString(40, 50, Red, "5.0");
		//		DrawFormatString(30, 350, Red, "-5.0");
		//		DrawFormatString(570, 50, Blue, "100%%");
		//		DrawFormatString(570, 350, Blue, "0%%");
		//
		//		train_game_cnt++;
		//		if (epic >= max_epic - 1) {
		//			if (train_game_cnt % game_per_epic == 0) {
		//				for (int k = 0; k < max_epic - 1; ++k) {
		//					loss_per_epic[k] = loss_per_epic[k + 1];
		//					correct_per_epic[k] = correct_per_epic[k + 1];
		//					turn_per_epic[k] = turn_per_epic[k + 1];
		//				}
		//				loss_per_epic[max_epic - 1] = 0;
		//				correct_per_epic[max_epic - 1] = 0;
		//				turn_per_epic[max_epic - 1] = 0;
		//				epic = max_epic - 1;
		//			}
		//		}
		//		else {
		//			epic = train_game_cnt / game_per_epic;
		//		}
		//
		//		for (int l = 0; l < 51; ++l) { DrawLine(70, 50 + 6 * l, 570, 50 + 6 * l, Black); }
		//		for (int l = 0; l < 11; ++l) { DrawLine(70, 50 + 30 * l + 1, 570, 50 + 30 * l + 1, Black); }
		//		for (int k = 1; k < max_epic; ++k) {
		//			DrawLine(500.0*(k - 1) / max_epic + 70,
		//				350.0 - 300.0*correct_per_epic[k - 1] / max(turn_per_epic[k - 1], 1),
		//				500.0*k / max_epic + 70,
		//				350.0 - 300.0*correct_per_epic[k] / max(turn_per_epic[k], 1),
		//				Blue);
		//			DrawLine(500.0*(k - 1) / max_epic + 70,
		//				350.0 - 30.0 * (5.0 + log(max(loss_per_epic[k - 1], 1.0)) - log(max(turn_per_epic[k - 1], 1))),
		//				500.0*k / max_epic + 70,
		//				350.0 - 30.0 * (5.0 + log(max(loss_per_epic[k], 1.0)) - log(max(turn_per_epic[k], 1))),
		//				Red);
		//		}
		//
		//		if (Key[KEY_INPUT_A] == 1) {
		//			setx = game.mouse.x; sety = game.mouse.y;
		//			game.flg = 0;
		//			taijin = 0;
		//			for (int i = 0; i < 3; ++i) tama[i].initialize();
		//		}
		//
		//	}
		//	else if(debugFlg == 2){
		//		InitializeGame(5);
		//		debugFlg = 0;
		//	}
		//}

		//シナリオ
		else if (game.flg == -6) {
			if (Soloflg == 0) {
				DrawExtendGraph(0 + eqx, -50, 640 + eqx, 380, Room, FALSE);
				if (visible[0]) DrawGraph(160 + eqx, 120, stripe[10], TRUE);
				if (visible[1]) DrawGraph(480 + eqx, 120, stripe[11], TRUE);
				if (visible[2]) DrawGraph(160 + eqx, 240, stripe[12], TRUE);
				if (visible[3]) DrawGraph(480 + eqx, 240, stripe[13], TRUE);
				if (Scenflg == 2) DrawExtendGraph(0, 0, 640, 400, Card, FALSE);
				DrawRoundBox(15, 380, 10, 609, 89, GetColor(250, 250, 150)); 
				DrawMessage(scen_char_cnt, 110, 390, 600, GetFontSize(), scen_txt[scen_txt_cnt].c_str(), game.strColor, Font0, GetColor(250, 250, 150));
				switch (scen_who[scen_txt_cnt]) {
				case 1:
					DrawGraph(30, 380, stripe[10], TRUE); break;
				case 2:
					DrawGraph(30, 380, stripe[11], TRUE); break;
				case 3:
					DrawGraph(30, 380, stripe[12], TRUE); break;
				case 4:
					DrawGraph(30, 380, stripe[13], TRUE); break;
				case 5:
					DrawGraph(30, 380, stripe[14], TRUE); break;
				}

				if (CheckMusic() != 1) {
					if (Scenflg == 0 || Scenflg == 7 || Scenflg == 12) PlayMusic("sound/bgm03.mp3", DX_PLAYTYPE_BACK);
					if (Scenflg == 21) PlayMusic("sound/bgm07.mp3", DX_PLAYTYPE_BACK);
				}
				switch (Scenflg) {
				case 1:
					//鹿が現れる
					DrawGraph(480, 120, stripe[11], TRUE);
					DrawGraph(270, 200, stripe[14], TRUE);
					if (game.mouse.button[0] == 1) {
						Scenflg++; scen_txt_cnt++; scen_char_cnt = 0;
					}
					break;
				case 2:
					//カードを見つける、青消える
					scen_txt_len = MultiByteLength(scen_txt[scen_txt_cnt].c_str());
					if (fps_cnt % 2 == 0 && scen_char_cnt < scen_txt_len) scen_char_cnt++;
					if (game.mouse.button[0] == 1) {
						if (scen_char_cnt < scen_txt_len) { scen_char_cnt = scen_txt_len; }
						else {
							visible[1] = 0;
							Scenflg++; scen_txt_cnt++; scen_char_cnt = 0;
						}
					}
					break;
				case 4:
					//地震
					eqx = 10 * sin(eqx + M_PI * (rand() % 10) / 10.0); 
					if (game.mouse.button[0] == 1) {
						eqx = 0;
						Scenflg++; scen_txt_cnt++; scen_char_cnt = 0;
					}
					break;
				case 6:
					//第一戦
					PlayMusic("sound/bgm04.mp3", DX_PLAYTYPE_BACK);
					scen_txt_cnt++; scen_char_cnt = 0;
					game.initialize();
					initializeTrain();
					break;
				case 8:
					//赤が死ぬ
					visible[2] = 0; 
					if (game.mouse.button[0] == 1) {
						Scenflg++;
					}
					break;
				case 9:
					//地震
					eqx = 10 * sin(eqx + M_PI * (rand() % 10) / 10.0);
					if (game.mouse.button[0] == 1) {
						eqx = 0;
						Scenflg++; scen_txt_cnt++; scen_char_cnt = 0;
					}
					break;
				case 11:
					//第二戦
					PlayMusic("sound/bgm05.mp3", DX_PLAYTYPE_BACK);
					scen_txt_cnt++; scen_char_cnt = 0;
					game.initialize();
					initializeTrain();
					break;
				case 13:
					//緑が死ぬ
					visible[3] = 0; 
					if (game.mouse.button[0] == 1) {
						Scenflg++; 
					}
					break;
				case 14:
					//地震
					eqx = 10 * sin(eqx + M_PI * (rand() % 10) / 10.0);
					if (game.mouse.button[0] == 1) {
						eqx = 0;
						Scenflg++; scen_txt_cnt++; scen_char_cnt = 0;
					}
					break;
				case 15:
					//青が出てくる
					visible[1] = 1;
					if (game.mouse.button[0] == 1) {
						Scenflg++; ;
					}
					break;
				case 17:
					//第三戦
					PlayMusic("sound/bgm06.mp3", DX_PLAYTYPE_BACK);
					scen_txt_cnt++; scen_char_cnt = 0;
					game.initialize();
					initializeTrain();
					break;
				case 19:
					//青が死ぬ
					visible[1] = 0;
					if (game.mouse.button[0] == 1) {
						Scenflg++; 
					}
					break;
				case 20:
					//地震
					eqx = 10 * sin(eqx + M_PI * (rand() % 10) / 10.0);
					if (game.mouse.button[0] == 1) {
						eqx = 0;
						Scenflg++; scen_txt_cnt++; scen_char_cnt = 0;
					}
					break;
				case 22:
					//第四戦
					PlayMusic("sound/bgm08.mp3", DX_PLAYTYPE_BACK);
					scen_txt_cnt++; scen_char_cnt = 0;
					game.initialize();
					initializeTrain();
					break;
				default:
					scen_txt_len = MultiByteLength(scen_txt[scen_txt_cnt].c_str());
					if (fps_cnt % 2 == 0 && scen_char_cnt < scen_txt_len) scen_char_cnt++;
					if (game.mouse.button[0] == 1) {
						if (scen_char_cnt < scen_txt_len) { scen_char_cnt = scen_txt_len; }
						else { 
							if (scen_txt_cnt == 0 || scen_txt_cnt == 3 || scen_txt_cnt == 5 || scen_txt_cnt == 8 
								|| scen_txt_cnt == 12 || scen_txt_cnt == 14 || scen_txt_cnt == 18 || scen_txt_cnt == 20 || scen_txt_cnt == 26) {
								Scenflg++;
							}
							else {
								scen_txt_cnt = min(27, scen_txt_cnt + 1);
								scen_char_cnt = 0;
							}
						}
					}
					break;
				}
				DrawFormatString(5, 5, game.strColor, "Scenflg:%d", Scenflg);
				//DrawFormatString(5, 25, StringColor, "char_cnt:%d", char_cnt);
			}
		}

		//game.flg > 0でタイトルロゴを表示する
		if (game.flg > 0) {
			game.logo.draw();
			if (game.flg == 1) {
				game.logo.update();
			}
		}

		//同期処理
		fps_cnt++;
		if (clock() - fps_start > 1000.0) {
			cur_fps = fps_cnt;
			fps_cnt = 0;
			fps_start = clock();
		}
		if(likeliFlg) DrawFormatString(5, 5, game.strColor, "fps:%d", cur_fps);
		if (COMWait > 0) COMWait--;
		while (clock() - start < 1000.0 / 30.0 && game.flg != 5) {
			WaitTimer(1);
		}
		start = clock();
	}

	InitGraph();
	//DeleteGraph(MovieGraphHandle); 
	//MV1DeleteModel(ModelHandle);
	DxLib_End();

	return 0;
}


// テキストファイルの読み込み
int GetTexts(string* text, const char* filename) {
	ifstream read(filename);
	string str;
	int length = 30;
	int maxSize = txtMax;
	double tmp = 0.0;
	int k = 0;

	if (!read) {
		cout << "テキストファイルの読み込み失敗" << endl;
	}
	else {
		while (k < maxSize) {
			getline(read, str);
			text[k] = str;
			k++;
		}
	}
	read.close();
	return k;
}


//VectorXd StateToInput(int dim, int side) {
//	VectorXd trg(dim);
//	for (int i1 = 0; i1 < 3; ++i1) {
//		for (int j1 = 0; j1 < 3; ++j1) {
//			for (int k1 = 0; k1 < 3; ++k1) {
//				for (int l1 = 0; l1 < 3; ++l1) {
//					trg(27 * i1 + 9 * j1 + 3 * k1 + l1) = game.child[i1][j1].state[k1][l1] * side;
//					if ((game.nextField == -1 || game.nextField == 3 * i1 + j1)
//						&& game.child[i1][j1].state[k1][l1] == 0 && game.child[i1][j1].victory() == 0) {
//						trg(27 * i1 + 9 * j1 + 3 * k1 + l1 + 81) = 1.0;
//					}
//					else {
//						trg(27 * i1 + 9 * j1 + 3 * k1 + l1 + 81) = -1.0;
//					}
//				}
//			}
//		}
//	}
//	return trg;
//}
//
//VectorXd Reward1(const VectorXd &out, const VectorXd &in, int side) {
//	VectorXd trg; trg = out;
//
//	for (int i1 = 0; i1 < 3; ++i1) {
//		for (int j1 = 0; j1 < 3; ++j1) {
//			for (int k1 = 0; k1 < 3; ++k1) {
//				for (int l1 = 0; l1 < 3; ++l1) {
//					if (in(27 * i1 + 9 * j1 + 3 * k1 + l1 + 81) == -1.0) {
//						trg(27 * i1 + 9 * j1 + 3 * k1 + l1) = RWD_FAULT;
//					}
//					else {
//						//trg(27 * i1 + 9 * j1 + 3 * k1 + l1) += RWD_CANPUT; 
//						trg(27 * i1 + 9 * j1 + 3 * k1 + l1) = RWD_CANPUT;
//					}
//				}
//			}
//		}
//	}
//	return trg;
//}
//
//VectorXd softmax(const VectorXd &src, double alpha) {
//	VectorXd trg;
//	
//	trg = (alpha * src).array().exp();
//	trg = trg / trg.sum();
//
//	return trg;
//}