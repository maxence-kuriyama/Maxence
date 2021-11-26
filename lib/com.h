#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "lib/vect.h"

using namespace Eigen;

//VectorXd StateToInput(int dim, int side);
//VectorXd Reward1(const VectorXd &out, const VectorXd &in, int side);
//VectorXd softmax(const VectorXd &src, double alpha);

// vect.hで定義したMLPを仮想プレイヤー的に扱うためのインターフェース
class COM {
private:
	int COMGx = 1;
	int COMGy = 1;
	int COMLx = 1;
	int COMLy = 1;						//COMの選ぶ座標
	int COMWait = 0;
	int waitOnCOM = 20;					//COMが手を打つまでのウェイト
	double eps = 0.002;					//学習定数
	//double gamma = 0.95;				//割引率
	double mom = 0.9;
	double varc = 0.999;				// adamのパラメータ
	::Affine Q1;
	::Affine Q2;
	::Affine Q3;
	ActLayer R1;
	ActLayer R2;
	VectorXd output;

public:
	int max_id = 0;
	double max_val = 0.0;

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

	//学習機械関連
	int lay_len = 3;
	int lay_size[4] = { 162, 800, 400, 81 };
	//VectorXd input(lay_size[0]);
	//VectorXd p_output;
	//VectorXd temp_i[100];
	//VectorXd temp_o[100];	//学習用データの一時保存用ベクトル
	//MatrixXd train_i;
	//MatrixXd train_o;		//バッチ学習用のデザイン行列
	double reward2 = 0.0;
	double rwd_tmp = 0.0;
	//double anl_rate = 0.0;	//epsilon-greedyの割合
	//double alpha = 2.1;	//softmaxの係数
	//int anl_flg = 0;


	COM() {
		// MLPの初期化
		MatrixXd P1 = MatrixXd::Random(lay_size[0], lay_size[1]) * sqrt(0.1 / lay_size[0]);
		VectorXd B1 = VectorXd::Random(lay_size[1]) * 0.1;
		MatrixXd P2 = MatrixXd::Random(lay_size[1], lay_size[2]) * sqrt(0.1 / lay_size[1]);
		VectorXd B2 = VectorXd::Random(lay_size[2]) * 0.1;
		MatrixXd P3 = MatrixXd::Random(lay_size[2], lay_size[3]) * sqrt(0.1 / lay_size[2]);
		VectorXd B3 = VectorXd::Random(lay_size[3]) * 0.05;
		Q1.setParam(P1, B1);
		Q1.setCoef(mom, varc);
		Q2.setParam(P2, B2);
		Q2.setCoef(mom, varc);
		Q3.setParam(P3, B3);
		Q3.setCoef(mom, varc);
		R1.setMode("relu");
		R2.setMode("relu");
		Machine critic(5, IdentityV, eps, "adam");
		critic.setLayer(Q1, 0);
		critic.setLayer(R1, 1);
		critic.setLayer(Q2, 2);
		critic.setLayer(R2, 3);
		critic.setLayer(Q3, 4);
		output = VectorXd::Zero(lay_size[3]);
	}

	~COM() {}

	void visualize() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				for (int k = 0; k < 3; ++k) {
					for (int l = 0; l < 3; ++l) {
						double tempColor = min(max(240.0 * (output(27 * i + 9 * j + 3 * k + l) + 0.5), 0.0), 255.0);
						DrawCircle(160 + 100 * i + 33 * k + 16, 80 + 100 * j + 33 * l + 16, 15, GetColor(255, 255, 255 - tempColor));
						//if (game.option.likeliFlg == 2) {
						//	char str_tmp[10];
						//	sprintf_s(str_tmp, "%.4f", output(27 * i + 9 * j + 3 * k + l));
						//	DrawStringToHandle(160 + 100 * i + 33 * k + 2, 80 + 100 * j + 33 * l + 2, str_tmp, Red, Font1);
						//	if (trainCnt >= 1) {
						//		sprintf_s(str_tmp, "%.4f", temp_o[trainCnt - 1](27 * i + 9 * j + 3 * k + l));
						//		DrawStringToHandle(160 + 100 * i + 33 * k + 2, 80 + 100 * j + 33 * l + 12, str_tmp, Blue, Font1);
						//	}
						//}
					}
				}
			}
		}
	}

};


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

//VectorXd softmax(const VectorXd &src, double alpha) {
//	VectorXd trg;
//	
//	trg = (alpha * src).array().exp();
//	trg = trg / trg.sum();
//
//	return trg;
//}