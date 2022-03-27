#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "lib/vect.h"

using namespace Eigen;

VectorXd softmax(const VectorXd &src, double alpha);


// vect.hで定義したMLPを仮想プレイヤー的に扱うためのインターフェース
// 単体での使用を想定（Gameオブジェクトから呼び出すべきか？）
class COM {
private:
	int cnt = 0;
	int wait = 20;						//COMが手を打つまでのウェイト
	double eps = 0.002;					//学習定数
	double mom = 0.9;
	double varc = 0.999;				// adamのパラメータ
	//double gamma = 0.95;				//割引率
	//double alpha = 2.1;				//softmaxの係数
	double annealRate = 1.0;				//epsilon-greedyの割合
	int anneal = 0;
	::Affine Q1;
	::Affine Q2;
	::Affine Q3;
	ActLayer R1;
	ActLayer R2;
	VectorXd output;
	Machine critic;
	int strColorDebug = GetColor(255, 100, 0);

public:
	int globalX = 1;
	int globalY = 1;
	int localX = 1;
	int localY = 1;						//COMの選ぶ座標
	int max_id = 0;
	double max_val = 0.0;

	//学習機械関連
	int lay_len = 3;
	int lay_size[4] = { 162, 800, 400, 81 };
	double reward2 = 0.0;
	double rwd_tmp = 0.0;


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
		critic.set(5, IdentityV, eps, "adam");
		critic.setLayer(Q1, 0);
		critic.setLayer(R1, 1);
		critic.setLayer(Q2, 2);
		critic.setLayer(R2, 3);
		critic.setLayer(Q3, 4);
		output = VectorXd::Zero(lay_size[3]);
	}

	~COM() {}

	void setWait() {
		cnt = wait;
	}

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

	void play(VectorXd input) {
		output = critic.predict(input);
		max_val = output.maxCoeff(&max_id);
		// waitを消化したら手を選択する
		if (cnt <= 0) {
			if (unif(mt) < annealRate) {
				choiceRandom();
			}
			else {
				choiceMax();
			}
		}
		cnt--;
	}

	void choiceRandom() {
		globalX = rand() % 3;
		globalY = rand() % 3;
		localX = rand() % 3;
		localY = rand() % 3;
		anneal = 1;
	}

	void choiceMax() {
		globalX = (max_id / 27) % 3;
		globalY = (max_id / 9) % 3;
		localX = (max_id / 3) % 3;
		localY = max_id % 3;
		anneal = 0;
	}

	void debugDump(int debug) {
		if (debug) {
			int strColor = strColorDebug;

			DrawFormatString(505, 25, strColor, "anneal: %d", anneal);
		}
	}

};


VectorXd softmax(const VectorXd &src, double alpha) {
	VectorXd trg;
	
	trg = (alpha * src).array().exp();
	trg = trg / trg.sum();

	return trg;
}
