#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "lib/const.h"
#include "lib/utils/coordinate.h"
#include "lib/components/board.h"
#include "./com/vect.h"
#include "./com/minmax.h"
#include "./com/loader.h"

using namespace Eigen;

const int LAYER_DEPTH(4);

const int COM_THINKING_WAIT(0.8 * FPS);
const float COM_ANNEALING_RATE(0.10); // epsilon-greedy
const float COM_SOFTMAX_ALPHA_DEFAULT(25.0);  // softmaxの係数
const float COM_SOFTMAX_ALPHA_DECREMENT(0.20);
const float COM_HYBRID_THRESHOLD_WIN(0.75); // 複合戦略でMLPを使うしきい値
const float COM_HYBRID_THRESHOLD_LOSE(-0.75); // 複合戦略でMLPを棄却するしきい値

const int COM_LEVEL0(0); // Only MiniMax depth 1
const int COM_LEVEL1(1); // Only MLP
const int COM_LEVEL2(2); // MLP with MiniMax depth 2
const int COM_LEVEL3(3); // MLP with MiniMax depth 3

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

	~COM() {
		if (lastMinMaxNode != NULL) delete lastMinMaxNode;
	}

	static void setChoice(Coordinate c) {
		COM::choice = c;
	}

	static void setChoice(int index) {
		COM::choice = index;
	}

private:
	int wait = 0;
	int maxId = 0;
	double maxVal = 0.0;
	MinMaxNode* lastMinMaxNode = NULL;
	double alpha = COM_SOFTMAX_ALPHA_DEFAULT;
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
	static void setWait(int wait = COM_THINKING_WAIT) {
		COM* com = getInstance();
		com->wait = wait;
	}

	static void resetPlaying() {
		COM* com = getInstance();
		com->alpha = COM_SOFTMAX_ALPHA_DEFAULT;
		COM::setWait();
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

		com->wait--;
		if (com->wait > 0) return;

		int depth = 3;
		switch (level) {
		case COM_LEVEL0:
			depth = 1;
			return com->_playByMinMax(board, side, depth);
		case COM_LEVEL1:
			return com->_playByMachine(input, board, side);
		case COM_LEVEL2:
			depth = 2;
			break;
		case COM_LEVEL3:
			depth = 3;
			break;
		}
		return com->_play(input, board, side, depth);
	}

	static void playAsPlayerCheat(VectorXd input) {
		COM* com = getInstance();

		com->wait--;
		if (com->wait > 0) return;

		return com->_playAsPlayer(input);
	}

	static double evaluateByMinMax(const Board board, int side) {
		MinMaxNode node(board, side);
		MinMaxNode::truncate = false;
		int depth = 0;
		int _index = node.search(depth);
		return node.value;
	}

	static void debugDump() {
		COM* com = getInstance();
		int strColor = com->strColorDebug;

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

			Coordinate c;
			c.set(index);
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

	void _play(VectorXd input, const Board board, int side, int depth = 3) {
		_playByMinMax(board, side, depth);
		if (lastMinMaxNode->value > COM_HYBRID_THRESHOLD_WIN) return;

		_playByMachine(input, board, side);

		int index = COM::choice.getIndex();
		double minMaxValue = lastMinMaxNode->getChildValue(index);
		if (minMaxValue >= COM_HYBRID_THRESHOLD_LOSE) return;

		loggingReject(index, minMaxValue);
		COM::setChoice(lastMinMaxNode->max_child_index);
	}

	void _playByMachine(VectorXd input, const Board board, int side) {
		return _playByMachineWithSoftmax(input, board, side);
	}

	void _playByMachineWithAnnealing(VectorXd input, const Board board, int side) {
		_predict(input);

		double r = unif(mt);
		if (r > COM_ANNEALING_RATE) {
			COM::setChoice(maxId);
			return;
		}

		COM::setChoice(rand() % 81);
		loggingAnnealing(r);
	}

	void _playByMachineWithSoftmax(VectorXd input, const Board board, int side) {
		_predict(input);
		VectorXd distribution = softmax(output, alpha);

		int index = getIndexInProbability(distribution);
		if (index == -1) {
			COM::setChoice(rand() % 81);
			return;
		}

		COM::setChoice(index);
		loggingChoiceBySoftmax(index);
		alpha -= COM_SOFTMAX_ALPHA_DECREMENT;
	}

	void _playByMinMax(const Board board, int side, int depth = 3) {
		MinMaxNode* node = new MinMaxNode(board, side);
		MinMaxNode::truncate = true;
		int index = node->search(depth);
		COM::setChoice(index);
		miniMaxDebugStr = node->debugStr();

		if (lastMinMaxNode != NULL) delete lastMinMaxNode;
		lastMinMaxNode = node;
	}

	void _playAsPlayer(VectorXd input) {
		_predict(input);
		VectorXd invOutput = -output;
		VectorXd distribution = softmax(invOutput, alpha);

		int index = getIndexInProbability(distribution);
		if (index == -1) {
			COM::setChoice(rand() % 81);
			return;
		}

		COM::setChoice(index);
		loggingChoiceBySoftmax(index);
		alpha = max(alpha - COM_SOFTMAX_ALPHA_DECREMENT, 0.0);
	}

	int getIndexInProbability(VectorXd distribution) {
		double r = unif(mt);
		double sliceSum = 0.0;
		for (int i = 0; i < 81; i++) {
			sliceSum += distribution[i];
			if (r >= sliceSum) continue;

			return i;
		}
		return -1;
	}

	VectorXd softmax(const VectorXd& src, double srcAlpha = COM_SOFTMAX_ALPHA_DEFAULT) {
		VectorXd trg;

		trg = (srcAlpha * src).array().exp();
		trg = trg / trg.sum();

		return trg;
	}

	/*===========================*/
	//    Logging
	/*===========================*/
	void loggingReject(int index, double minMaxValue) {
		Logger::ss << "Reject!! ==== "
			<< "chosen index: " << index << ", "
			<< "value: " << minMaxValue << " < "
			<< "threshold: " << COM_HYBRID_THRESHOLD_LOSE;
		Logger::log();
	}

	void loggingAnnealing(double r) {
		Logger::ss << "Annealed!! ==== "
			<< "realized random value: " << r << " < "
			<< "annealing rate: " << COM_ANNEALING_RATE;
		Logger::log();
	}

	void loggingChoiceBySoftmax(int index) {
		Logger::ss << "Choice by softmax: "
			<< "index: " << index << ", "
			<< "alpha: " << alpha;
		Logger::log();
	}
};

COM* COM::_instance = NULL;
Coordinate COM::choice;
