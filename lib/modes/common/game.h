#pragma once

#include <Eigen/Core>
#include "lib/const.h"
#include "lib/user_input.h"
#include "lib/board.h"


const int VS_HUMAN(0);
const int VS_COM(1);

// 試合進行管理インスタンスクラス
class Game {
private:
	// 定数
	int Green = GetColor(0, 255, 0);
	int Red = GetColor(255, 0, 0);
	int Blue = GetColor(0, 0, 255);
	int White = GetColor(255, 255, 255);
	int Black = GetColor(0, 0, 0);
	int bkColorBase = GetColor(255, 255, 245);
	int frColorBase = Black;
	int bkColorLastHist = GetColor(255, 160, 160);
	int bkColorStateWin = GetColor(130, 70, 70);
	int bkColorStateLose = GetColor(70, 70, 130);
	int bkColorStateDraw = GetColor(70, 130, 70);
	int frColorNextField = Red;
	int frColorCurrentCoord = Black;
	int strColorDebug = Blue;
	int bkGraph = LoadGraph("graph/game_background.png");
	int stone1 = LoadGraph("graph/stone1.png");
	int stone2 = LoadGraph("graph/stone2.png");
	int stone1_t = LoadGraph("graph/stone1.png");
	int stone2_t = LoadGraph("graph/stone2.png");

	// プレイヤー画像のハンドラ
	int player1 = 0;
	int player2 = 0;

	int taijin = VS_HUMAN;
	int cnt = 0;			// ターン数
	int drawCnt = 0;		// 引き分け時の強制終了のためのカウント

public:
	string mode = "";
	Board board;
	int teban = TEBAN_SENKO;	// 0: senko, 1: koko
	int playCnt = 0;			// 1ターンに費やしたカウント

	Coordinate coordinate = { 0, 0, 0, 0, DUMMY_LAST_FIELD }; //キーボード操作時の座標


	Game() {
		// フィールド画像初期化
		board.setStoneGraphs(stone1, stone2, stone1_t, stone2_t);
	}

	// 試合情報の初期化
	void initialize() {
		cnt = 0;
		drawCnt = 0;
		board.initialize();
	}

	void tick() {
		playCnt = (playCnt + 1) % 10000000;
	}

	void reset(Music& bgm) {
		taijin = VS_HUMAN;
		mode = "";
		bgm.unloadAll();
		DeleteGraph(player1);
		DeleteGraph(player2);
	}


	/*===========================*/
	//    準備処理
	/*===========================*/
	void goBattle(int player1, int player2, bool init = true) {
		if (init) initialize();
		setPlayersGraph(player1, player2);
	}

	void setPlayersGraph(int pl1, int pl2) {
		if (pl1 != BATTLE_PLAYER_NONE) DeleteGraph(player1);
		switch (pl1)
		{
		case BATTLE_PLAYER_YELLOW:
			player1 = LoadGraph("graph/player_yellow.png");
			break;
		case BATTLE_PLAYER_PLAYER:
			player1 = LoadGraph("graph/player_player.png");
			break;
		case BATTLE_PLAYER_NONE:
		default:
			break;
		}

		if (pl2 != BATTLE_PLAYER_NONE) DeleteGraph(player2);
		switch (pl2)
		{
		case BATTLE_PLAYER_YELLOW:
			player2 = LoadGraph("graph/enemy_yellow.png");
			break;
		case BATTLE_PLAYER_RED:
			player2 = LoadGraph("graph/enemy_red.png");
			break;
		case BATTLE_PLAYER_BLUE:
			player2 = LoadGraph("graph/enemy_blue.png");
			break;
		case BATTLE_PLAYER_GREEN:
			player2 = LoadGraph("graph/enemy_green.png");
			break;
		case BATTLE_PLAYER_NONE:
		default:
			break;
		}
	}

	void setVsHuman() {
		mode = "隣の人と";
		taijin = VS_HUMAN;
	}

	void setVsCOM() {
		mode = "ぼっちで";
		taijin = VS_COM;
	}


	/*===========================*/
	//    Boolean
	/*===========================*/
	bool isPlayTurn() {
		// 対人戦、あるいは人vsCOMの人の手番
		return (isVsHuman() || (isVsCOM() && cnt % 2 == teban));
	}

	bool isVsHuman() {
		return taijin == VS_HUMAN;
	}

	bool isVsCOM() {
		return taijin == VS_COM;
	}


	/*===========================*/
	//    盤面表示関連
	/*===========================*/
	void drawBase() {
		DrawExtendGraph(-50, 40, 690, 420, bkGraph, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
		DrawBox(160, 80, 460, 380, bkColorBase, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 256);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), frColorBase, FALSE);
			}
		}
	}

	void drawHistLast() {
		if (board.canCancel()) {
			Coordinate c = board.last();

			int upLeftX = 160 + 100 * c.global_x + 33 * c.x + 1;
			int upLeftY = 80 + 100 * c.global_y + 33 * c.y + 1;
			int lowRightX = 160 + 100 * c.global_x + 33 * (c.x + 1);
			int lowRightY = 80 + 100 * c.global_y + 33 * (c.y + 1);
			DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, bkColorLastHist, TRUE);
		}
	}

	void drawGlobalState() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				int upLeftX = 160 + 100 * i + 1;
				int upLeftY = 80 + 100 * j + 1;
				int lowRightX = 160 + 100 * (i + 1) - 1;
				int lowRightY = 80 + 100 * (j + 1) - 1;
				if (board.globalState(i, j) == 1) {
					DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, bkColorStateWin, TRUE);
				}
				else if (board.globalState(i, j) == -1) {
					DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, bkColorStateLose, TRUE);
				}
				else if (board.globalState(i, j) != 0) {
					DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, bkColorStateDraw, TRUE);
				}
			}
		}
	}

	void drawNextField() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (board.isStrictNext(i, j)) {
					for (int w = -1; w <= 1; ++w) {
						int upLeftX = 160 + 100 * i + w;
						int upLeftY = 80 + 100 * j + w;
						int lowRightX = 160 + 100 * (i + 1) - w;
						int lowRightY = 80 + 100 * (j + 1) - w;
						DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, frColorNextField, FALSE);
					}
				}
				else if (board.isNextAny() && board.localVictory(i, j) == 0) {
					int upLeftX = 160 + 100 * i;
					int upLeftY = 80 + 100 * j;
					int lowRightX = 160 + 100 * (i + 1);
					int lowRightY = 80 + 100 * (j + 1);
					DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, frColorNextField, FALSE);
				}
			}
		}
	}

	void drawLocalState() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				board.drawLocal(i, j, 176.5 + 100 * i, 96.5 + 100 * j, 33);
			}
		}
	}

	void drawCurrentCoord() {
		Coordinate c = coordinate;
		int upLeftX = 160 + 100 * c.global_x + 33 * c.x;
		int upLeftY = 80 + 100 * c.global_y + 33 * c.y;
		int lowRightX = 160 + 100 * c.global_x + 33 * (c.x + 1) + 1;
		int lowRightY = 80 + 100 * c.global_y + 33 * (c.y + 1) + 1;
		DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, frColorCurrentCoord, FALSE);
	}

	void drawPlayers(int side = 0) {
		// sideが指定されていなければ、cntとtebanから計算する
		if (side == 0) {
			side = currentSide();
			if (isVsCOM()) {
				side = side * (1 - 2 * (teban % 2));
			}
		}

		if (side == 1) {
			DrawExtendGraph(0, 100, 200, 340, player1, TRUE);
			SetDrawBright(155, 155, 155);
			DrawExtendGraph(440, 100, 640, 340, player2, TRUE);
			SetDrawBright(255, 255, 255);
		}
		else if (side == -1) {
			SetDrawBright(155, 155, 155);
			DrawExtendGraph(0, 100, 200, 340, player1, TRUE);
			SetDrawBright(255, 255, 255);
			DrawExtendGraph(440, 100, 640, 340, player2, TRUE);
		}
	}


	/*===========================*/
	//    盤面更新関連
	/*===========================*/
	void getMouseCoord(Mouse& mouse) {
		for (int index = 0; index < 81; index++) {
			Coordinate c = Board::coordinates(index);

			int upLeftX = 160 + 100 * c.global_x + 33 * c.x;
			int upLeftY = 80 + 100 * c.global_y + 33 * c.y;
			int lowRightX = 160 + 100 * c.global_x + 33 * (c.x + 1);
			int lowRightY = 80 + 100 * c.global_y + 33 * (c.y + 1);
			if (mouse.onButton(upLeftX, upLeftY, lowRightX, lowRightY)) {
				coordinate = c;
			}
		}
	}

	void moveCoordByKey(Key& key) {
		int globalX = coordinate.global_x;
		int globalY = coordinate.global_y;
		int localX = coordinate.x;
		int localY = coordinate.y;

		if (key.onUp()) {
			localY--;
			if (localY < 0) {
				globalY = (globalY - 1 + 3) % 3;
				localY += 3;
			}
		}
		if (key.onDown()) {
			localY++;
			if (localY >= 3) {
				globalY = (globalY + 1) % 3;
				localY -= 3;
			}
		}
		if (key.onLeft()) {
			localX--;
			if (localX < 0) {
				globalX = (globalX - 1 + 3) % 3;
				localX += 3;
			}
		}
		if (key.onRight()) {
			localX++;
			if (localX > 2) {
				globalX = (globalX + 1) % 3;
				localX -= 3;
			}
		}

		coordinate = { globalX, globalY, localX, localY, DUMMY_LAST_FIELD };
	}

	bool playTurn(UserInput& ui) {
		if (ui.keyboard) {
			moveCoordByKey(*ui.key);
		}
		else {
			getMouseCoord(*ui.mouse);
		}

		if (ui.check()) {
			playCnt = 0;
			return true;
		}
		return false;
	}

	double update(int side = 0) {
		return update(coordinate, side);
	}

	double update(Coordinate& c, int side = 0) {
		return update(c.global_x, c.global_y, c.x, c.y, side);
	}

	double update(int global_x, int global_y, int local_x, int local_y, int side = 0) {
		// sideが指定されていなければ、cntから計算する
		if (side == 0) side = currentSide();

		double reward = board.update(global_x, global_y, local_x, local_y, side);
		if (isUpdated(reward)) {
			cnt++;
		}
		return reward;
	}

	bool goBackHist() {
		if (board.canCancel() && isVsHuman()) {
			board.goBack();
			cnt--;
			return true;
		}
		return false;
	}


	/*===========================*/
	//    盤面情報
	/*===========================*/
	Eigen::VectorXd stateToInput(int side = 0, int dim = 162) {
		if (side == 0) side = currentSide();

		Eigen::VectorXd trg(dim);
		for (int index = 0; index < 81; index++) {
			trg(index) = board.localState(index) * side;
			if (board.canPut(index)) {
				trg(index + 81) = 1.0;
			}
			else {
				trg(index + 81) = -1.0;
			}
		}
		return trg;
	}

	bool isUpdated(double reward) {
		return (reward > -10.0);
	}

	int victory() {
		return board.victory();
	}

	int currentSide() {
		return 1 - 2 * (cnt % 2);
	}


	/*===========================*/
	//    デバッグ情報
	/*===========================*/
	void debugDump() {
		int strColor = strColorDebug;
		// Game
		DrawFormatString(5, 105, strColor, "taijin: %d", taijin);
		DrawFormatString(5, 125, strColor, "teban: %d", teban);
		DrawFormatString(5, 145, strColor, "cnt: %d", cnt);
		DrawFormatString(5, 165, strColor, "mode: %s", mode.c_str());
		DrawFormatString(5, 185, strColor, "playCnt: %d", playCnt);
	}
};
