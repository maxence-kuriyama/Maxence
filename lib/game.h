#pragma once

#include <time.h>
#include <Eigen/Core>
#include "lib/const.h"
#include "lib/basic.h"
#include "lib/field.h"
#include "lib/hist.h"
#include "lib/keyboard.h"
#include "lib/menu.h"
#include "lib/anime.h"
#include "lib/comment.h"


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
	Logo logo;

public:
	int flg = -3;	// -3,..,-1: Demo
					// 0: Menu, 1: Game, 2: Result
					// -4: Ending
					// -6: Story
	int taijin = 0;		// 0: vsHuman, 1: vsCOM
	int teban = 0;		// 0: senko, 1: koko
	int cnt = 0;		// ターン数
	int nextField = -1;		// 次の盤面、-1: anywhere
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
	Field mother;
	Field child[3][3];
	History hist;
	Camera camera;
	Mouse mouse;
	Key key;
	Anime cutin;
	Comment comment;
	Button btnAgain;
	Button btnSave;
	Button btnReset;

	Game() {
		// ボタン初期化
		btnLonely.initialize(TEXT1_X, TEXT1_Y, "ぼっちで");
		btnVsHuman.initialize(TEXT2_X, TEXT2_Y, "隣の人と");
		btnSenko.initialize(TEXT1_X, TEXT1_Y, "先攻");
		btnKoko.initialize(TEXT2_X, TEXT2_Y, "後攻");
		btnAgain.initialize(44, 44, 44 - 8, 44 - 8, 44 + 88, 44 + 24, "もう一回");
		// カットイン画像初期化
		int Cutin1 = LoadGraph("graph/cutin1.png");
		int Cutin10 = LoadGraph("graph/cutin10.png");
		GraphBlend(Cutin1, Cutin10, 255, DX_GRAPH_BLEND_MULTIPLE);
		cutin.image0 = Cutin1;
		cutin.image1 = Cutin10;
		comment.initialize();
		// フィールド画像初期化
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				child[i][j].stone1 = stone1;
				child[i][j].stone2 = stone2;
				child[i][j].stone1_t = stone1_t;
				child[i][j].stone2_t = stone2_t;
			}
		}
		// game初期化
		initialize();
		flg = -3;		// デモ画面へ
	}

	void initialize() {
		cnt = 0;
		drawCnt = 0;
		nextField = -1;
		camera.initialize();
		mother.initialize();
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				child[i][j].initialize();
			}
		}
		hist.initialize();
		mouse.set();
		key.initWait();
		menu.set(btnLonely, btnVsHuman);
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
		return taijin == 0;
	}

	bool isVsCOM() {
		return taijin == 1;
	}

	bool isTitle() {
		return (flg == 0);
	}

	bool isBattle() {
		return (flg == 1);
	}

	bool isResult() {
		return (flg == 2);
	}

	bool isScenario() {
		return (flg == -6);
	}

	bool isEnding() {
		return (flg == -4);
	}


	/*===========================*/
	//    フラグ関連
	/*===========================*/
	void goTitle() {
		flg = 0;
	}

	void goBattle(int player1 = BATTLE_PLAYER_NONE, int player2 = BATTLE_PLAYER_NONE) {
		flg = 1;
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

	void goResult() {
		flg = 2;
	}

	void goScenario() {
		flg = -6;
	}

	void goEnding() {
		flg = -4;
	}


	/*===========================*/
	//    リセットボタン
	/*===========================*/
	void drawLogo() {
		logo.draw(mouse);
		if (isBattle()) logo.update(); // 動く
	}

	int reset(Music& bgm) {
		if (flg > 0) {
			if (logo.isClicked(mouse)) {
				mouse.set();
				goTitle();
				taijin = 0;
				mode = "";
				bgm.unloadAll();
				DeleteGraph(player1);
				DeleteGraph(player2);
				return 1;
			}
		}
		return 0;
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
		if (hist.canCancel()) {
			int upLeftX = 160 + 100 * hist.last[0] + 33 * hist.last[2] + 1;
			int upLeftY = 80 + 100 * hist.last[1] + 33 * hist.last[3] + 1;
			int lowRightX = 160 + 100 * hist.last[0] + 33 * (hist.last[2] + 1);
			int lowRightY = 80 + 100 * hist.last[1] + 33 * (hist.last[3] + 1);
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
				if (mother.state[i][j] == 1) {
					DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, bkColorStateWin, TRUE);
				}
				else if (mother.state[i][j] == -1) {
					DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, bkColorStateLose, TRUE);
				}
				else if (mother.state[i][j] != 0) {
					DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, bkColorStateDraw, TRUE);
				}
			}
		}
	}

	void drawNextField() {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (nextField == 3 * i + j) {
					for (int w = -1; w <= 1; ++w) {
						int upLeftX = 160 + 100 * i + w;
						int upLeftY = 80 + 100 * j + w;
						int lowRightX = 160 + 100 * (i + 1) - w;
						int lowRightY = 80 + 100 * (j + 1) - w;
						DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, frColorNextField, FALSE);
					}
				}
				else if (nextField == -1 && child[i][j].victory() == 0) {
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
				child[i][j].draw(176.5 + 100 * i, 96.5 + 100 * j, 33);
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

	void drawPlayers(int side = 0) {
		// sideが指定されていなければ、cntとtebanから計算する
		if (side == 0) {
			side = 1 - 2 * (cnt % 2);
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
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				for (int k = 0; k < 3; ++k) {
					for (int l = 0; l < 3; ++l) {
						int upLeftX = 160 + 100 * i + 33 * k;
						int upLeftY = 80 + 100 * j + 33 * l;
						int lowRightX = 160 + 100 * i + 33 * (k + 1);
						int lowRightY = 80 + 100 * j + 33 * (l + 1);
						if (mouse.onButton(upLeftX, upLeftY, lowRightX, lowRightY)) {
							globalX = i;
							globalY = j;
							localX = k;
							localY = l;
						}
					}
				}
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

	double update(int global_x, int global_y, int local_x, int local_y, int side = 0) {
		// sideが指定されていなければ、cntから計算する
		if (side == 0) {
			side = 1 - 2 * (cnt % 2);
		}
		//盤面の更新
		if (nextField == 3 * global_x + global_y || nextField == -1) {
			if (child[global_x][global_y].update(local_x, local_y, side) == 0) {
				cnt++;
				//履歴を残す
				hist.add(global_x, global_y, local_x, local_y, nextField);
				//全体の更新
				mother.update(global_x, global_y, child[global_x][global_y].victory());
				if (child[local_x][local_y].victory() != 0) {
					nextField = -1;
					return RWD_DOM;
				}
				else {
					nextField = local_x * 3 + local_y;
					return RWD_PUT;
				}
			}
		}
		return -100.0;
	}

	bool goBackHist() {
		if (hist.canCancel() && isVsHuman()) {
			child[hist.last[0]][hist.last[1]].state[hist.last[2]][hist.last[3]] = 0;
			mother.state[hist.last[0]][hist.last[1]] = 0;
			mother.update(hist.last[0], hist.last[1], child[hist.last[0]][hist.last[1]].victory());
			nextField = hist.last[4];
			hist.goBack();
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
	Eigen::VectorXd stateToInput(int side = 0, int dim = 162) {
		if (side == 0) {
			side = 1 - 2 * (cnt % 2);
		}
		Eigen::VectorXd trg(dim);
		for (int i1 = 0; i1 < 3; ++i1) {
			for (int j1 = 0; j1 < 3; ++j1) {
				for (int k1 = 0; k1 < 3; ++k1) {
					for (int l1 = 0; l1 < 3; ++l1) {
						trg(27 * i1 + 9 * j1 + 3 * k1 + l1) = child[i1][j1].state[k1][l1] * side;
						if ((nextField == -1 || nextField == 3 * i1 + j1) && child[i1][j1].state[k1][l1] == 0 && child[i1][j1].victory() == 0) {
							trg(27 * i1 + 9 * j1 + 3 * k1 + l1 + 81) = 1.0;
						}
						else {
							trg(27 * i1 + 9 * j1 + 3 * k1 + l1 + 81) = -1.0;
						}
					}
				}
			}
		}
		return trg;
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
			// Hist
			DrawFormatString(125, 145, strColor, "cancelCnt: %d", hist.cancelCnt);
			DrawFormatString(125, 165, strColor, "last[0]: %d", hist.last[0]);
			DrawFormatString(125, 185, strColor, "last[1]: %d", hist.last[1]);
			DrawFormatString(125, 205, strColor, "last[2]: %d", hist.last[2]);
			DrawFormatString(125, 225, strColor, "last[3]: %d", hist.last[3]);
			DrawFormatString(125, 245, strColor, "last[4]: %d", hist.last[4]);
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
