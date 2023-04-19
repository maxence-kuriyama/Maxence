#pragma once

#include <time.h>
#include <Eigen/Core>
#include "lib/const.h"
#include "lib/basic.h"
#include "lib/keyboard.h"
#include "lib/menu.h"
#include "lib/anime.h"
#include "lib/comment.h"
#include "lib/board.h"


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
	int Font0 = CreateFontToHandle("HG教科書体", 24, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
	int stone1 = LoadGraph("graph/stone1.png");
	int stone2 = LoadGraph("graph/stone2.png");
	int stone1_t = LoadGraph("graph/stone1.png");
	int stone2_t = LoadGraph("graph/stone2.png");

	// プレイヤー画像のハンドラ
	int player1 = 0;
	int player2 = 0;

	// 同期処理関連
	long start = clock();	// 同期処理開始時刻
	long fpsStart = clock();	// FPS計測開始時刻
	int fpsCnt = 0;			// FPS計測用

	Menu menu;
	Button btnLonely;
	Button btnVsHuman;
	Button btnSenko;
	Button btnKoko;
	Button btnSave;
	Button btnReset;
	Logo logo;

public:
	int flg = GAME_FLAG_DEMO_FIRST;
	int taijin = GAME_VS_HUMAN;
	int teban = 0;		// 0: senko, 1: koko
	int cnt = 0;		// ターン数
	int keyboardFlg = 0;	// 0: マウス操作, 1: キーボード操作
	int debugFlg = 0;
	int debugEndingFlg = 0;
	string mode = "";
	int fps = 0;			// 実効FPS

	// 盤面上の操作関連
	int globalX = 1;
	int globalY = 1;
	int localX = 1;
	int localY = 1;			//キーボード操作時の座標

	int playCnt = 0;		// 1ターンに費やしたカウント
	int drawCnt = 0;		// 引き分け時の強制終了のためのカウント

	Option option;
	Camera camera;
	Mouse mouse;
	Key key;
	Anime cutin;
	Comment comment;
	Button btnAgain;
	Board board;

	Game() {
		// ボタン初期化
		btnLonely.initialize(TEXT1_X, TEXT1_Y, "ぼっちで");
		btnVsHuman.initialize(TEXT2_X, TEXT2_Y, "隣の人と");
		btnSenko.initialize(TEXT1_X, TEXT1_Y, "先攻");
		btnKoko.initialize(TEXT2_X, TEXT2_Y, "後攻");
		btnAgain.initialize(44, 44, 44 - 8, 44 - 8, 44 + 88, 44 + 24, "もう一回");
		btnSave.initialize(TEXT_SAVE_X, TEXT_SAVE_Y, "中断");
		btnReset.initialize(TEXT_RESET_X, TEXT_RESET_Y, "タイトル");
		// カットイン画像初期化
		int Cutin1 = LoadGraph("graph/cutin1.png");
		int Cutin10 = LoadGraph("graph/cutin10.png");
		GraphBlend(Cutin1, Cutin10, 255, DX_GRAPH_BLEND_MULTIPLE);
		cutin.image0 = Cutin1;
		cutin.image1 = Cutin10;
		comment.initialize();
		// フィールド画像初期化
		board.setStoneGraphs(stone1, stone2, stone1_t, stone2_t);
		// 試合情報の初期化
		initialize();
		menu.set(btnLonely, btnVsHuman);
		flg = GAME_FLAG_DEMO_FIRST;
	}

	// 試合情報の初期化（goBattleと統合してもいいかも）
	void initialize() {
		cnt = 0;
		drawCnt = 0;
		board.initialize();
		camera.initialize();
		mouse.set();
		key.initWait();
	}

	// 同期処理
	void sync() {
		// 実効fps計測
		fpsCnt++;
		playCnt = (playCnt + 1) % 10000000;
		if (clock() - fpsStart > 1000.0) {
			fps = fpsCnt;
			fpsCnt = 0;
			fpsStart = clock();
		}
		// 同期処理
		while (clock() - start < 1000.0 / FPS && flg != 5) {
			WaitTimer(1);
		}
		start = clock();
	}


	/*===========================*/
	//    Boolean
	/*===========================*/
	bool isPlayTurn() {
		// 対人戦、あるいは人vsCOMの人の手番
		return (isVsHuman() || (isVsCOM() && cnt % 2 == teban));
	}

	bool isVsHuman() {
		return taijin == GAME_VS_HUMAN;
	}

	bool isVsCOM() {
		return taijin == GAME_VS_COM;
	}

	bool isTitle() {
		return (flg == GAME_FLAG_TITLE);
	}

	bool isBattle() {
		return (flg == GAME_FLAG_BATTLE);
	}

	bool isResult() {
		return (flg == GAME_FLAG_RESULT);
	}

	bool isScenario() {
		return (flg == GAME_FLAG_SCENARIO);
	}

	bool isEnding() {
		return (flg == GAME_FLAG_ENDING);
	}


	/*===========================*/
	//    フラグ関連
	/*===========================*/
	void goTitle() {
		flg = GAME_FLAG_TITLE;
	}

	void goBattle(int player1 = BATTLE_PLAYER_NONE, int player2 = BATTLE_PLAYER_NONE) {
		flg = GAME_FLAG_BATTLE;
		setPlayersGraph(player1, player2);
		menu.set(btnSave, btnReset);
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

	void goResult() {
		flg = GAME_FLAG_RESULT;
	}

	void goScenario() {
		flg = GAME_FLAG_SCENARIO;
	}

	void goEnding() {
		flg = GAME_FLAG_ENDING;
	}


	/*===========================*/
	//    リセットボタン
	/*===========================*/
	void drawLogo() {
		logo.draw(mouse);
		if (isBattle()) logo.update(); // 動く
	}

	void reset(Music& bgm) {
		mouse.set();
		goTitle();
		taijin = 0;
		mode = "";
		bgm.unloadAll();
		DeleteGraph(player1);
		DeleteGraph(player2);
		menu.set(btnLonely, btnVsHuman);
	}


	/*===========================*/
	//    メニュー画面
	/*===========================*/
	int menuChoose() {
		return menu.choose(keyboardFlg, mouse, key, option.strColor);
	}

	void setOrderMenu() {
		menu.set(btnSenko, btnKoko);
	}


	/*===========================*/
	//    キーボード入力関連
	/*===========================*/
	void toggleByKey(Music& music) {
		key.toggleSetting(option, logo);
		//key.configLearning();
		key.toggleDebug(debugFlg);
		if (debugFlg) {
			key.toggleForDebug(option, cutin.flg, debugEndingFlg);
		}
	}

	void skipBattle(int& sceneFlg) {
		if (debugFlg && isVsCOM()) {
			key.skipBattle(flg, sceneFlg);
		}
	}

	void toggleMouseOrKeyboard() {
		if (mouse.isUsed()) keyboardFlg = 0;
		if (key.isUsed()) keyboardFlg = 1;
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
			int* last = board.last();
			int global_x = last[0];
			int global_y = last[1];
			int local_x = last[2];
			int local_y = last[3];

			int upLeftX = 160 + 100 * global_x + 33 * local_x + 1;
			int upLeftY = 80 + 100 * global_y + 33 * local_y + 1;
			int lowRightX = 160 + 100 * global_x + 33 * (local_x + 1);
			int lowRightY = 80 + 100 * global_y + 33 * (local_y + 1);
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
		int upLeftX = 160 + 100 * globalX + 33 * localX;
		int upLeftY = 80 + 100 * globalY + 33 * localY;
		int lowRightX = 160 + 100 * globalX + 33 * (localX + 1) + 1;
		int lowRightY = 80 + 100 * globalY + 33 * (localY + 1) + 1;
		DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, frColorCurrentCoord, FALSE);
	}

	void drawBattleMessage() {
		DrawFormatString(470, 80, option.strColor, "右クリック:");
		DrawFormatString(540, 100, option.strColor, "石を置く");
		DrawFormatString(470, 124, option.strColor, "zキー（BkSpキー）:");
		DrawFormatString(540, 144, option.strColor, "一手戻る");
	}

	void drawComment() {
		// コメント描画
		if (option.commentFlg) {
			comment.draw(option.strColor);
		}
		// コメント差し替え
		comment.update(playCnt);
	}

	void drawPlayers(int src_side = 0) {
		// sideが指定されていなければ、cntとtebanから計算する
		if (src_side == 0) {
			src_side = side();
			if (isVsCOM()) {
				src_side = src_side * (1 - 2 * (teban % 2));
			}
		}

		if (src_side == 1) {
			DrawExtendGraph(0, 100, 200, 340, player1, TRUE);
			SetDrawBright(155, 155, 155);
			DrawExtendGraph(440, 100, 640, 340, player2, TRUE);
			SetDrawBright(255, 255, 255);
		}
		else if (src_side == -1) {
			SetDrawBright(155, 155, 155);
			DrawExtendGraph(0, 100, 200, 340, player1, TRUE);
			SetDrawBright(255, 255, 255);
			DrawExtendGraph(440, 100, 640, 340, player2, TRUE);
		}
	}

	void drawWinner(int vict) {
		if (vict == 1) {
			DrawFormatString(20, 20, Red, "Player1");
		}
		else if (vict == -1) {
			DrawFormatString(20, 20, Black, "Player2");
		}
		else {
			DrawFormatString(20, 20, Green, "No one");
		}
		DrawFormatString(20 + 96, 20, Green, "Won");
	}


	/*===========================*/
	//    盤面更新関連
	/*===========================*/
	void getMouseCoord() {
		for (int index = 0; index < 81; index++) {
			int* coord_info = Board::coordinates(index);
			int global_x = coord_info[0];
			int global_y = coord_info[1];
			int local_x = coord_info[2];
			int local_y = coord_info[3];

			int upLeftX = 160 + 100 * global_x + 33 * local_x;
			int upLeftY = 80 + 100 * global_y + 33 * local_y;
			int lowRightX = 160 + 100 * global_x + 33 * (local_x + 1);
			int lowRightY = 80 + 100 * global_y + 33 * (local_y + 1);
			if (mouse.onButton(upLeftX, upLeftY, lowRightX, lowRightY)) {
				globalX = global_x;
				globalY = global_y;
				localX = local_x;
				localY = local_y;
			}
		}
	}

	void moveCoordByKey() {
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
	}

	bool playTurn() {
		moveCoordByKey();
		if (keyboardFlg) {
			if (key.onCheck()) {
				playCnt = 0;
				return true;
			}
		}
		else {
			getMouseCoord();
			if (mouse.clickRight()) {
				playCnt = 0;
				return true;
			}
		}
		return false;
	}

	double update(int side = 0) {
		return update(globalX, globalY, localX, localY, side);
	}

	double update(int global_x, int global_y, int local_x, int local_y, int src_side = 0) {
		// sideが指定されていなければ、cntから計算する
		if (src_side == 0) src_side = side();

		double reward = board.update(global_x, global_y, local_x, local_y, src_side);
		if (isUpdated(reward)) {
			cnt++;
			//履歴を残す
			board.addHistory(global_x, global_y, local_x, local_y);
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

	void updateCommentInBattle() {
		comment.forceUpdate(COMMENT_CHANGE_TYPE_PLAY_TURN, 0.40);
	}


	/*===========================*/
	//    盤面情報
	/*===========================*/
	Eigen::VectorXd stateToInput(int src_side = 0, int dim = 162) {
		if (src_side == 0) src_side = side();

		Eigen::VectorXd trg(dim);
		for (int index = 0; index < 81; index++) {
			trg(index) = board.localState(index) * src_side;
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

	int side() {
		return 1 - 2 * (cnt % 2);
	}


	/*===========================*/
	//    デバッグ情報
	/*===========================*/
	void debugDump() {
		if (debugFlg) {
			int strColor = strColorDebug;
			// Game
			DrawFormatString(5, 25, strColor, "fps: %d", fps);
			DrawFormatString(5, 45, strColor, "gameFlg: %d", flg);
			DrawFormatString(5, 65, strColor, "taijin: %d", taijin);
			DrawFormatString(5, 85, strColor, "teban: %d", teban);
			DrawFormatString(5, 105, strColor, "cnt: %d", cnt);
			DrawFormatString(5, 125, strColor, "keyboardFlg: %d", keyboardFlg);
			DrawFormatString(5, 145, strColor, "mode: %s", mode.c_str());
			DrawFormatString(5, 165, strColor, "playCnt: %d", playCnt);
			// Option
			DrawFormatString(125, 25, strColor, "musicFlg: %d", option.musicFlg);
			DrawFormatString(125, 45, strColor, "soundFlg: %d", option.soundFlg);
			DrawFormatString(125, 65, strColor, "likeliFlg: %d", option.likeliFlg);
			DrawFormatString(125, 85, strColor, "commentFlg: %d", option.commentFlg);
			// Comment
			comment.debugDump(strColor);
			// Menu
			DrawFormatString(365, 25, strColor, "menu.size: %d", menu.size);
			DrawFormatString(365, 45, strColor, "menu.id: %d", menu.id);
			DrawFormatString(365, 65, strColor, "btn1.upLX: %d", menu.button[0].upLeftX);
			DrawFormatString(365, 85, strColor, "btn1.upLY: %d", menu.button[0].upLeftY);
			DrawFormatString(365, 105, strColor, "btn1.lowRX: %d", menu.button[0].lowRightX);
			DrawFormatString(365, 125, strColor, "btn1.lowRY: %d", menu.button[0].lowRightY);
			DrawFormatString(365, 145, strColor, "btn2.upLX: %d", menu.button[1].upLeftX);
			DrawFormatString(365, 165, strColor, "btn2.upLY: %d", menu.button[1].upLeftY);
			DrawFormatString(365, 185, strColor, "btn2.lowRX: %d", menu.button[1].lowRightX);
			DrawFormatString(365, 205, strColor, "btn2.lowRY: %d", menu.button[1].lowRightY);
		}
	}
};
