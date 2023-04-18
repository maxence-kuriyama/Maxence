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
		for (int index = 0; index < 81; index++) {
			double temp_color = min(max(240.0 * (output(index) + 0.5), 0.0), 255.0);
			int color = GetColor(255, 255, 255 - temp_color);

			int* coord_info = Board::coordinates(index);
			int global_x = coord_info[0];
			int global_y = coord_info[1];
			int local_x = coord_info[2];
			int local_y = coord_info[3];

			int draw_x = 160 + 100 * global_x + 33 * local_x + 16;
			int draw_y = 80 + 100 * global_y + 33 * local_y + 16;
			int radius = 15;
			DrawCircle(draw_x, draw_y, radius, color);
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
		int* coord_info = Board::coordinates(max_id);
		globalX = coord_info[0];
		globalY = coord_info[1];
		localX = coord_info[2];
		localY = coord_info[3];
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
