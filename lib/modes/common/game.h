#pragma once

#include <Eigen/Core>
#include "lib/const.h"
#include "lib/user_input.h"
#include "lib/board.h"
#include "lib/encrypter.h"
#include "lib/logger.h"


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

	int taijin = VS_HUMAN;
	int teban = TEBAN_SENKO;	// 0: senko, 1: koko
	int player1 = 0;
	int player2 = 0;
	int cnt = 0;			// ターン数

	// プレイヤー画像のハンドラ
	int player1_h = 0;
	int player2_h = 0;

	// 引き分け時の強制終了のためのカウント
	int drawCnt = 0;

public:
	string mode = "";
	Board board;

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

	void reset() {
		initialize();

		// こいつらは試合情報でないのでinitializeで初期化しない
		mode = "";
		taijin = VS_HUMAN;
		teban = TEBAN_SENKO;
	}

	void reset(Music& bgm) {
		reset();
		bgm.unloadAll();
	}


	/*===========================*/
	//    準備処理
	/*===========================*/
	void prepare(int pl1, int pl2 , bool init = true) {
		if (init) initialize();
		player1 = pl1;
		player2 = pl2;
		setPlayersGraph(pl1, pl2);
	}

	void setPlayersGraph(int pl1, int pl2) {
		DeleteGraph(player1_h);
		switch (pl1)
		{
		case BATTLE_PLAYER_YELLOW:
			player1_h = LoadGraph("graph/player_yellow.png");
			break;
		case BATTLE_PLAYER_PLAYER:
			player1_h = LoadGraph("graph/player_player.png");
			break;
		case BATTLE_PLAYER_NONE:
		default:
			break;
		}

		DeleteGraph(player2_h);
		switch (pl2)
		{
		case BATTLE_PLAYER_YELLOW:
			player2_h = LoadGraph("graph/enemy_yellow.png");
			break;
		case BATTLE_PLAYER_RED:
			player2_h = LoadGraph("graph/enemy_red.png");
			break;
		case BATTLE_PLAYER_BLUE:
			player2_h = LoadGraph("graph/enemy_blue.png");
			break;
		case BATTLE_PLAYER_GREEN:
			player2_h = LoadGraph("graph/enemy_green.png");
			break;
		case BATTLE_PLAYER_NONE:
		default:
			break;
		}
	}

	void setVsHuman() {
		mode = "vs_human";
		taijin = VS_HUMAN;
	}

	void setVsCOM() {
		mode = "vs_com";
		taijin = VS_COM;
	}

	void setTutorial() {
		mode = "tutorial";
		taijin = VS_COM;
		teban = TEBAN_SENKO;
	}

	void setSenko() {
		teban = TEBAN_SENKO;
	}

	void setKoko() {
		teban = TEBAN_KOKO;
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
	void drawBeforePlay() {
		drawBase();
		drawGlobalState();
		drawHistLast();
		drawNextField();
	}

	void drawAfterPlay() {
		drawLocalState();
		drawCurrentCoord();
		drawPlayers();
	}

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
			DrawExtendGraph(0, 100, 200, 340, player1_h, TRUE);
			SetDrawBright(155, 155, 155);
			DrawExtendGraph(440, 100, 640, 340, player2_h, TRUE);
			SetDrawBright(255, 255, 255);
		}
		else if (side == -1) {
			SetDrawBright(155, 155, 155);
			DrawExtendGraph(0, 100, 200, 340, player1_h, TRUE);
			SetDrawBright(255, 255, 255);
			DrawExtendGraph(440, 100, 640, 340, player2_h, TRUE);
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

		return ui.check();
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
	Eigen::VectorXd stateToInput(int side = 0) {
		if (side == 0) side = currentSide();
		int shift = 0;

		Eigen::VectorXd trg(MACHINE_INPUT_SIZE);

		// 盤面情報を入力とする
		for (int index = 0; index < 81; index++) {
			trg(index) = board.localState(index) * side;
		}

		// 配置可能な場所のデータも入力に入れる
		shift = 81;
		for (int index = 0; index < 81; index++) {
			trg(index + shift) = (board.canPut(index) ? 1.0 : -1.0);
		}

		// 大域状況を入れる
		shift = 81 + 81;
		for (int g_index = 0; g_index < 9; g_index++) {
			int g_state = board.globalState(g_index);
			trg(g_index + shift) = (g_state != VICTORY_DRAW ? g_state * side : 0.0);
			trg(g_index + shift + 9) = (g_state != VICTORY_NONE ? 1.0 : -1.0);
		}
		shift = 81 + 81 + 9 + 9;
		for (int gw_index = 0; gw_index < 8; gw_index++) {
			trg(gw_index + shift) = board.isWaitingGlobal(gw_index, side);
			trg(gw_index + shift + 8) = board.isWaitingGlobal(gw_index, -side);
		}

		// 局所のリーチ状況も入れる
		for (int g_index = 0; g_index < 9; g_index++) {
			shift = 81 + 81 + 9 + 9 + 8 + 8 + g_index * 8;
			for (int lw_index = 0; lw_index < 8; lw_index++) {
				trg(lw_index + shift) = board.isWaitingLocal(g_index, lw_index, side);
				trg(lw_index + shift + 9 * 8) = board.isWaitingLocal(g_index, lw_index, -side);
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
	//    Save and Load
	/*===========================*/
	void save(const string filepath) {
		Encrypter encrypter(filepath);
		nlohmann::json data = {
			{"taijin", taijin},
			{"teban", teban},
			{"cnt", cnt},
			{"mode", mode},
			{"player1", player1},
			{"player2", player2},
			{"board", board.json()},
		};
		encrypter.write(data);
	}

	void load(const string filepath) {
		Encrypter encrypter(filepath);
		nlohmann::json res = encrypter.read();
		Logger::ss << "Game loaded: " << res.dump();
		Logger::log();

		taijin = res["taijin"];
		teban = res["teban"];
		cnt = res["cnt"];
		mode = res["mode"];
		player1 = res["player1"];
		player2 = res["player2"];
		setPlayersGraph(player1, player2);

		board.restore(res["board"]);

		remove(filepath.c_str());
	}


	/*===========================*/
	//    デバッグ情報
	/*===========================*/
	void debugDump() {
		// if (mode != "vs_human") return;

		int strColor = strColorDebug;
		// Game
		DrawFormatString(5, 105, strColor, "taijin: %d", taijin);
		DrawFormatString(5, 125, strColor, "teban: %d", teban);
		DrawFormatString(5, 145, strColor, "cnt: %d", cnt);
		DrawFormatString(5, 165, strColor, "mode: %s", mode.c_str());
	}
};
