#pragma once

#include "lib/board.h"
#include "lib/logger.h"

const float EVALUATION_WIN(1.0);
const float EVALUATION_LOSE(-1.0);
const float EVALUATION_DRAW(0.0);
const float EVALUATION_WAITING(0.1);
const float EVALUATION_WAITED(-0.1);
const float EVALUATION_LOCAL_WIN(0.1);
const float EVALUATION_LOCAL_LOSE(-0.1);
const float EVALUATION_LOCAL_WAITING(0.01);
const float EVALUATION_LOCAL_WAITED(-0.01);

class MinMaxNode;

class ChildLink {
public:
	ChildLink* next;
	MinMaxNode* node;

	ChildLink();
	ChildLink(MinMaxNode* src);
	~ChildLink();

	bool hasNext() {
		return (next != nullptr);
	}

	void setNext(ChildLink* src) {
		next = src;
	}

	void setNode(MinMaxNode* src) {
		node = src;
	}
};

class MinMaxNode {
private:
	ChildLink* head;
	ChildLink* last;
	
	void createChildren();
	void searchRecursive(int depth, bool may_skip = false);
	bool terminated();
	void evaluate();
	void evaluateChildren();
	int optimalIndex();
	MinMaxNode* childNodeByIndex(int index);
	void loggingSearchForward(int depth);
	void loggingSearchBackward(int depth);
	void loggingSearchTerminal();
	void loggingSearchSkip();

	void setLast(ChildLink* src) {
		last = src;
	}
	
public:
	Board board;
	int side;
	int index = 0;
	int max_child_index = 0;
	double value = 0.0;

	static bool truncate;

	MinMaxNode(const Board& src_board, int src_side, int src_index = 0) {
		board = src_board;
		side = src_side;
		index = src_index;

		head = new ChildLink();
		last = head;
	}

	~MinMaxNode() {
		delete head;
		truncate = false;
	}

	int search(int depth) {
		searchRecursive(depth);
		return max_child_index;
	}

	string debugStr() {
		stringstream ss;
		int local_x = (max_child_index % 9) / 3;
		int local_y = (max_child_index % 9) % 3;
		ss << local_x << "," << local_y;

		MinMaxNode* node = this;
		while (!node->terminated()) {
			node = node->childNodeByIndex(node->max_child_index);
			if (!node) break;
			
			int local_x = (node->max_child_index % 9) / 3;
			int local_y = (node->max_child_index % 9) % 3;
			ss << "->" << local_x << "," << local_y;
		}
		return ss.str();
	}
};

bool MinMaxNode::truncate = false;

void MinMaxNode::searchRecursive(int depth, bool may_skip) {
	if (depth == 0 || terminated() || (may_skip && board.isNextAny())) {
		if (depth == 0 || terminated()) {
			loggingSearchTerminal();
		}
		else {
			loggingSearchSkip();
		}
		evaluate();
		return;
	}

	loggingSearchForward(depth);
	createChildren();

	ChildLink* child = head;
	while (child->hasNext()) {
		child = child->next;
		if (truncate && depth == 2) {
			child->node->searchRecursive(depth - 1, board.isNextAny());
		}
		else {
			child->node->searchRecursive(depth - 1);
		}
	}
	evaluateChildren();
	max_child_index = optimalIndex();
	loggingSearchBackward(depth);
}

bool MinMaxNode::terminated() {
	return (board.victory() != VICTORY_NONE);
}

// 子を持つノードの評価
void MinMaxNode::createChildren() {
	for (int new_index = 0; new_index < 81; new_index++) {
		Board board_clone;
		board_clone = board;
		
		//盤面の更新
		double res = board_clone.update(new_index, side, false);
		if (res < -10.0) continue;

		MinMaxNode* new_node = new MinMaxNode(board_clone, -side, new_index);
		ChildLink* child = new ChildLink(new_node);
		last->setNext(child);
		setLast(child);
	}

}

// 末端ノードの評価
void MinMaxNode::evaluate() {
	int global_victory = board.victory();
	if (global_victory != 0) {
		if (global_victory == side) {
			value = EVALUATION_WIN;
		}
		else if (global_victory == -side) {
			value = EVALUATION_LOSE;
		}
		else {
			value = EVALUATION_DRAW;
		}
		return;
	}
	
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			int local_victory = board.localVictory(x, y);
			if (local_victory == side) {
				value += EVALUATION_LOCAL_WIN;
			}
			else if (local_victory == -side) {
				value += EVALUATION_LOCAL_LOSE;
			}
		}
	}

	int waiting_count = board.globalWaiting(side);
	value += EVALUATION_WAITING * waiting_count;
	int waited_count = board.globalWaiting(-side);
	value += EVALUATION_WAITED * waited_count;
	
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			int local_waiting = board.localWaiting(x, y, side);
			value += EVALUATION_LOCAL_WAITING * local_waiting;
			int local_waited = board.localWaiting(x, y, -side);
			value += EVALUATION_LOCAL_WAITED * local_waited;
		}
	}
}

void MinMaxNode::evaluateChildren() {
	value = -100.0;
	ChildLink* child = head;
	while (child->hasNext()) {
		child = child->next;
		double child_value = -child->node->value; // 子ノードは相手視点
		if (value < child_value) value = child_value;
	}
}

int MinMaxNode::optimalIndex() {
	int max_index[81] = { -1 };
	int count = 0;
	
	ChildLink* child = head;
	while (child->hasNext()) {
		child = child->next;
		double child_value = -child->node->value; // 子ノードは相手視点
		// valueにはすでに最大値が入っている
		if (child_value > value - 0.001) {
			max_index[count++] = child->node->index;
		}
	}

	if (count == 0) return rand() % 81;
	if (count == 1) return max_index[0];

	return max_index[rand() % count];
}

MinMaxNode* MinMaxNode::childNodeByIndex(int src_index) {
	ChildLink* child = head;
	while (child->hasNext()) {
		child = child->next;
		if (child->node->index == src_index) return child->node;
	}
	return NULL;
}

/*===========================*/
//    Logging
/*===========================*/
void MinMaxNode::loggingSearchForward(int depth) {
	Logger::ss << "Search (forward) ==== "
		<< "depth: " << depth << ", "
		<< "index: " << index;
	Logger::log();

	board.loggingWholeState();
}

void MinMaxNode::loggingSearchBackward(int depth) {
	Logger::ss << "Search (backward) ==== "
		<< "depth: " << depth << ", " 
		<< "index: " << index << endl;
	Logger::ss << "side: " << side << ", "
		<< "value: " << value << ", "
		<< "max_child_index: " << max_child_index;
	Logger::log();
}

void MinMaxNode::loggingSearchTerminal() {
	Logger::ss << "Terminal ==== "
		<< "index: " << index << ", "
		<< "side: " << side << ", "
		<< "value: " << value;
	Logger::log();
}

void MinMaxNode::loggingSearchSkip() {
	Logger::ss << "Skip recursive search ==== "
		<< "index: " << index << ", "
		<< "side: " << side << ", "
		<< "value: " << value;
	Logger::log();
}


ChildLink::ChildLink() {
	node = nullptr;
	next = nullptr;
}

ChildLink::ChildLink(MinMaxNode* src) {
	node = src;
	next = nullptr;
}

ChildLink::~ChildLink() {
	delete node;
	delete next;
}
