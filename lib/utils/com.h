#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "lib/const.h"
#include "lib/components/board.h"
#include "./com/vect.h"
#include "./com/minmax.h"
#include "./com/loader.h"

using namespace Eigen;

const float SOFTMAX_ALPHA(5.0);  // softmaxの係数
const float ANNEALING_RATE(1.0); // epsilon-greedyの割合
const int LAYER_DEPTH(4);

const int COM_THINKING_WAIT(100);
const int COM_LEVEL0(0); // Only MiniMax depth 1
const int COM_LEVEL1(1); // Only MLP
const int COM_LEVEL2(2); // MPL with MiniMax depth 2
const int COM_LEVEL3(3); // MPL with MiniMax depth 3

// vect.hで定義したMLPを仮想プレイヤー的に扱うためのインターフェース
class COM {
private:
	static COM* _instance; // singleton

	static COM* getInstance() {
		if (!_instance) {
			_instance = new COM();
		}
		return _instance;
	}

public:
	static Coordinate choice; //COMの選ぶ座標

	COM() {
		initialize();
	}

	~COM() {}

private:
	int cnt = 0;
	int maxId = 0;
	double maxVal = 0.0;
	int annealed = 0;
	double annealRate = 1.0;
	string miniMaxDebugStr;
	int strColorDebug = GetColor(255, 100, 0);
	
	//学習機械関連
	int layerSize[LAYER_DEPTH + 1] = { MACHINE_INPUT_SIZE, 1200, 400, 160, MACHINE_OUTPUT_SIZE };
	double mom = 0.9;
	double varc = 0.999;    // adamのパラメータ
	::Affine Q1;
	::Affine Q2;
	::Affine Q3;
	::Affine Q4;
	ActLayer R1;
	ActLayer R2;
	ActLayer R3;
	VectorXd output;
	Machine critic;
	Loader loader;

	void initialize() {
		// MLPの初期化
		MatrixXd P1 = MatrixXd::Random(layerSize[0], layerSize[1]) * sqrt(0.1 / layerSize[0]);
		VectorXd B1 = VectorXd::Random(layerSize[1]) * 0.1;
		MatrixXd P2 = MatrixXd::Random(layerSize[1], layerSize[2]) * sqrt(0.1 / layerSize[1]);
		VectorXd B2 = VectorXd::Random(layerSize[2]) * 0.1;
		MatrixXd P3 = MatrixXd::Random(layerSize[2], layerSize[3]) * sqrt(0.1 / layerSize[2]);
		VectorXd B3 = VectorXd::Random(layerSize[3]) * 0.1;
		MatrixXd P4 = MatrixXd::Random(layerSize[3], layerSize[4]) * sqrt(0.1 / layerSize[3]);
		VectorXd B4 = VectorXd::Random(layerSize[4]) * 0.05;
		Q1.setParam(P1, B1);
		Q1.setCoef(mom, varc);
		Q2.setParam(P2, B2);
		Q2.setCoef(mom, varc);
		Q3.setParam(P3, B3);
		Q3.setCoef(mom, varc);
		Q4.setParam(P4, B4);
		Q4.setCoef(mom, varc);
		R1.setMode("relu");
		R2.setMode("relu");
		R3.setMode("relu");
		critic.set(7, IdentityV, 0.0, "adam");
		critic.setLayer(Q1, 0);
		critic.setLayer(R1, 1);
		critic.setLayer(Q2, 2);
		critic.setLayer(R2, 3);
		critic.setLayer(Q3, 4);
		critic.setLayer(R3, 5);
		critic.setLayer(Q4, 6);
		output = VectorXd::Zero(MACHINE_OUTPUT_SIZE);
		load();
	}
	
	void load() {
		::Affine* layers[LAYER_DEPTH] = { &Q1, &Q2, &Q3, &Q4 };
		loader.read(LAYER_DEPTH, layerSize, layers);
	}

public:
	static void setWait() {
		COM* com = getInstance();
		com->cnt = COM_THINKING_WAIT;
	}

	static void visualize(VectorXd input) {
		COM* com = getInstance();
		return com->_visualize(input);
	}

	static void predict(VectorXd input) {
		COM* com = getInstance();
		return com->_predict(input);
	}

	static void play(VectorXd input, const Board board, int side, int level = COM_LEVEL1) {
		COM* com = getInstance();
		int depth = 4;
		switch (level) {
		case COM_LEVEL0:
			depth = 1;
			return com->_playMinMax(board, side, depth);
		case COM_LEVEL1:
			return com->_play(input, board, side);
		case COM_LEVEL2:
		case COM_LEVEL3:
		default:
			break;
		}
		return com->_play(input, board, side);
	}

	static void debugDump() {
		COM* com = getInstance();
		int strColor = com->strColorDebug;

		DrawFormatString(505, 25, strColor, "anneal: %d", com->annealed);
		DrawFormatString(505, 65, strColor, (com->miniMaxDebugStr).c_str());
	}

private:
	void _visualize(VectorXd input) {
		output = critic.predict(input);
		for (int index = 0; index < 81; index++) {
			int color;
			if (output(index) > 0.0) {
				if (output(index) > 1.0) {
					color = GetColor(155, 155, 0);
				}
				else {
					double tmp = 255.0 - 255.0 * output(index);
					color = GetColor(255, 255, tmp);
				}
			}
			else {
				if (output(index) < -1.0) {
					color = GetColor(0, 0, 155);
				}
				else {
					double tmp = 255.0 + 255.0 * output(index);
					color = GetColor(tmp, tmp, 255);
				}
			}

			Coordinate c = Board::coordinates(index);
			int draw_x = 160 + 100 * c.global_x + 33 * c.x + 16;
			int draw_y = 80 + 100 * c.global_y + 33 * c.y + 16;
			int radius = 15;

			DrawCircle(draw_x, draw_y, radius, color);
		}
	}

	void _predict(VectorXd input) {
		output = critic.predict(input);
		maxVal = output.maxCoeff(&maxId);
	}

	void _play(VectorXd input, const Board board, int side) {
		cnt--;
		if (cnt > 0) return;

		MinMaxNode node(board, side);
		int depth = 3;
		int index = node.search(depth);
		if (node.value > 0.75) {
			COM::choice = Board::coordinates(index);
			annealed = 0;
			return;
		}

		predict(input);
		if (unif(mt) < annealRate) {
			chooseRandom();
		}
		else {
			chooseMax();
		}
	}

	void _playMinMax(const Board board, int side, int depth = 4) {
		cnt--;
		if (cnt > 0) return;

		MinMaxNode node(board, side);
		MinMaxNode::truncate = true;
		int index = node.search(depth);

		COM::choice = Board::coordinates(index);
		annealed = 0;

		miniMaxDebugStr = node.debugStr();
	}
	
	void chooseRandom() {
		COM::choice = { rand() % 3, rand() % 3, rand() % 3, rand() % 3, DUMMY_LAST_FIELD };
		annealed = 1;
	}

	void chooseMax() {
		COM::choice = Board::coordinates(maxId);
		annealed = 0;
	}

	// TODO: softmax使うか要検討
	VectorXd softmax(const VectorXd& src) {
		VectorXd trg;

		trg = (SOFTMAX_ALPHA * src).array().exp();
		trg = trg / trg.sum();

		return trg;
	}
};

COM* COM::_instance = NULL;
Coordinate COM::choice;
