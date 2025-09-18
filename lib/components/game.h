#pragma once

#include <Eigen/Core>
#include "lib/const.h"
#include "lib/utils/coordinate.h"
#include "lib/utils/music.h"
#include "lib/utils/user_input.h"
#include "lib/utils/encrypter.h"
#include "lib/utils/logger.h"
#include "lib/components/board.h"
#include "lib/components/character.h"

const int VS_HUMAN(0);
const int VS_COM(1);

const int GAME_CHARACTER_X1(-40);
const int GAME_CHARACTER_X2(450);
const int GAME_CHARACTER_Y(100);

const double GAME_CHARACTER_SCALE(5.7);
const int GAME_CHARACTER_ALPHA_ACTIVE(245);
const int GAME_CHARACTER_ALPHA_INACTIVE(230);

// 試合進行管理インスタンスクラス
class Game {
private:
	// 定数
	int bkColorBase = GetColor(255, 255, 245);
	int frColorBase = GetColor(0, 0, 0);
	int bkColorLastHist = GetColor(255, 160, 160);
	int bkColorStateWin = GetColor(130, 70, 70);
	int bkColorStateLose = GetColor(70, 70, 130);
	int bkColorStateDraw = GetColor(70, 130, 70);
	int frColorNextField = GetColor(255, 0, 0);
	int frColorCurrentCoord = GetColor(0, 0, 0);
	int strColorDebug = GetColor(0, 0, 255);
	int bkGraph = LoadGraph("graph/game_background.png");
	int stone1 = LoadGraph("graph/stone1.png");
	int stone2 = LoadGraph("graph/stone2.png");
	int stone1_t = LoadGraph("graph/stone1.png");
	int stone2_t = LoadGraph("graph/stone2.png");

	int taijin = VS_HUMAN;
	int teban = TEBAN_SENKO;	// 0: senko, 1: koko
	int origPl1 = BATTLE_PLAYER_NONE;
	int origPl2 = BATTLE_PLAYER_NONE;
	Character player1;
	Character player2;
	int cnt = 0; // ターン数

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

	void reset(bool resetMusic = false) {
		initialize();

		// こいつらは試合情報でないのでinitializeで初期化しない
		mode = "";
		taijin = VS_HUMAN;
		teban = TEBAN_SENKO;

		if (resetMusic) Music::unloadAll();
	}

	/*===========================*/
	//    準備処理
	/*===========================*/
	void prepare(int pl1, int pl2 , bool init = true) {
		if (init) initialize();
		if (pl1 == BATTLE_PLAYER_NONE && pl2 == BATTLE_PLAYER_NONE) return;
	
		setPlayer1(pl1);
		setPlayer2(pl2);
	}

	void setPlayer1(int pl1) {
		origPl1 = pl1;
		switch (pl1) {
		case BATTLE_PLAYER_YELLOW:
			player1.initialize(CHARACTER_WHO_PL_YELLOW);
			break;
		case BATTLE_PLAYER_PLAYER:
			player1.initialize(CHARACTER_WHO_PL_PLAYER);
			break;
		case BATTLE_PLAYER_NONE:
			player1.initialize();
		default:
			break;
		}
		player1.x = GAME_CHARACTER_X1;
		player1.y = GAME_CHARACTER_Y;
		player1.scale = GAME_CHARACTER_SCALE;
		player1.alpha = GAME_CHARACTER_ALPHA_ACTIVE;
	}

	void setPlayer2(int pl2) {
		origPl1 = pl2;
		switch (pl2) {
		case BATTLE_PLAYER_YELLOW:
			player2.initialize(CHARACTER_WHO_YELLOW);
			break;
		case BATTLE_PLAYER_RED:
			player2.initialize(CHARACTER_WHO_RED);
			break;
		case BATTLE_PLAYER_BLUE:
			player2.initialize(CHARACTER_WHO_BLUE);
			break;
		case BATTLE_PLAYER_GREEN:
			player2.initialize(CHARACTER_WHO_GREEN);
			break;
		case BATTLE_PLAYER_TUTO_RED:
			player2.initialize(CHARACTER_WHO_TUTO_RED);
			break;
		case BATTLE_PLAYER_NONE:
		default:
			break;
		}
		player2.x = GAME_CHARACTER_X2;
		player2.y = GAME_CHARACTER_Y;
		player2.scale = GAME_CHARACTER_SCALE;
		player2.alpha = GAME_CHARACTER_ALPHA_ACTIVE;
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

	Game operator = (const Game& src) {
		taijin = src.taijin;
		teban = src.teban;
		cnt = src.cnt;
		player1 = src.player1;
		player2 = src.player2;

		mode = src.mode;
		board = src.board;

		return src;
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
			SetDrawBright(255, 255, 255);
			player1.alpha = GAME_CHARACTER_ALPHA_ACTIVE;
			player1.draw(DX_SCREEN_BACK);
			SetDrawBright(200, 200, 200);
			player2.alpha = GAME_CHARACTER_ALPHA_INACTIVE;
			player2.draw(DX_SCREEN_BACK);
		}
		else if (side == -1) {
			SetDrawBright(200, 200, 200);
			player1.alpha = GAME_CHARACTER_ALPHA_INACTIVE;
			player1.draw(DX_SCREEN_BACK);
			SetDrawBright(255, 255, 255);
			player2.alpha = GAME_CHARACTER_ALPHA_ACTIVE;
			player2.draw(DX_SCREEN_BACK);
		}
		SetDrawBright(255, 255, 255);
	}


	/*===========================*/
	//    盤面更新関連
	/*===========================*/
	void getMouseCoord(Mouse& mouse) {
		for (int index = 0; index < 81; index++) {
			Coordinate c;
			c.set(index);

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

	bool playTurn() {
		if (UserInput::usingKeyboard()) {
			moveCoordByKey(*UserInput::getKey());
		}
		else {
			getMouseCoord(*UserInput::getMouse());
		}

		return UserInput::check();
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

	Coordinate last() {
		return board.last();
	}

	int getTurnCount() {
		return cnt;
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
			{"player1", origPl1},
			{"player2", origPl2},
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
		origPl1 = res["player1"];
		origPl2 = res["player2"];
		setPlayer1(origPl1);
		setPlayer2(origPl2);

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


	/*===========================*/
	//    Enemyスキル用
	/*===========================*/
	void minusOneTurnCheat() {
		cnt--;
	}

	void forceWinLocalCheat(Coordinate c) {
		board.forceVictory(c, currentSide());
		cnt++;
	}

	void resetLocalCheat(Coordinate c) {
		board.forceVictory(c, 0);
		cnt++;
	}
};
