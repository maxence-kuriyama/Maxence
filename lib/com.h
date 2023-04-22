#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "lib/vect.h"
#include "lib/com/minmax.h"

using namespace Eigen;

const float SOFTMAX_ALPHA(2.1); // softmaxの係数
								// TODO: 完全ランダムやめろ
const float ANNEALING_RATE(1.0); // epsilon-greedyの割合

// vect.hで定義したMLPを仮想プレイヤー的に扱うためのインターフェース
// 単体での使用を想定（Gameオブジェクトから呼び出すべきか？）
class COM {
private:
	int cnt = 0;
	int wait = 20;						//COMが手を打つまでのウェイト
	double eps = 0.002;					//学習定数
	double mom = 0.9;
	double varc = 0.999;				// adamのパラメータ
	double annealRate = 1.0;				
	int annealed = 0;
	::Affine Q1;
	::Affine Q2;
	::Affine Q3;
	ActLayer R1;
	ActLayer R2;
	VectorXd output;
	Machine critic;
	int strColorDebug = GetColor(255, 100, 0);
	string miniMaxDebugStr;

public:
	Coordinate choice;					//COMの選ぶ座標
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

			Coordinate c = Board::coordinates(index);
			int draw_x = 160 + 100 * c.global_x + 33 * c.x + 16;
			int draw_y = 80 + 100 * c.global_y + 33 * c.y + 16;
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
				chooseRandom();
			}
			else {
				chooseMax();
			}
		}
		cnt--;
	}

	void playMinMax(const Board board, int side) {
		cnt--;
		if (cnt > 0) return;

		MinMaxNode node(board, side);
		// MinMaxNode::truncate = true;
		int depth = 3;
		int index = node.search(depth);

		choice = Board::coordinates(index);
		annealed = 0;
		
		miniMaxDebugStr = node.debugStr();
	}

	void chooseRandom() {
		choice = { rand() % 3, rand() % 3, rand() % 3, rand() % 3, DUMMY_LAST_FIELD };
		annealed = 1;
	}

	void chooseMax() {
		choice = Board::coordinates(max_id);
		annealed = 0;
	}

	// TODO: softmax使うか要検討
	VectorXd softmax(const VectorXd& src) {
		VectorXd trg;

		trg = (SOFTMAX_ALPHA * src).array().exp();
		trg = trg / trg.sum();

		return trg;
	}

	void debugDump(int debug) {
		if (debug) {
			int strColor = strColorDebug;

			DrawFormatString(505, 25, strColor, "anneal: %d", annealed);
			DrawFormatString(505, 65, strColor, miniMaxDebugStr.c_str());
		}
	}
};
