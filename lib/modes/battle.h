#pragma once

#include <fstream>
#include "lib/const.h"
#include "lib/com.h"
#include "lib/menu.h"
#include "lib/utils/user_input.h"
#include "./common/game.h"
#include "./battle/camera.h"
#include "./battle/anime.h"
#include "./battle/comment.h"

// バトルモード
// 主にGameの操作と描画
// Gameの描画はEndingとBattleResultでも行うため、公開する必要がある
class Battle {
private:
	Menu menu;
	Button btnSave;
	Button btnReset;

	Anime cutin;
	Camera camera;
	Comment comment;
	double theta = 0.3;

	int strColor = GetColor(255, 255, 255);
	int strColorDebug = GetColor(50, 50, 200);
	const string saveFilePath = SAVE_FILE_BATTLE_GAME;

	bool likelihoodFlg = false; // 学習機械の出力フラグ
	bool commentFlg = false;
	int playCnt = 0;			// 1ターンに費やしたカウント

public:
	Game game;

	Battle() {
		UserInput::reset();

		game.initialize();
		camera.initialize();
		comment.initialize();

		// ボタン初期化
		btnSave.initialize(TEXT_SAVE_X, TEXT_SAVE_Y, "中断");
		btnReset.initialize(TEXT_RESET_X, TEXT_RESET_Y, "タイトル");
		menu.set(btnSave, btnReset);

		// カットイン画像初期化
		int Cutin1 = LoadGraph("graph/cutin1.png");
		int Cutin10 = LoadGraph("graph/cutin10.png");
		GraphBlend(Cutin1, Cutin10, 255, DX_GRAPH_BLEND_MULTIPLE);
		cutin.image0 = Cutin1;
		cutin.image1 = Cutin10;
	}

	~Battle() {}

	void tick() {
		playCnt = (playCnt + 1) % 10000000;
	}

	void start(int player1, int player2, bool init = true) {
		UserInput::reset();
		game.prepare(player1, player2, init);
	}

	void startVsHuman() {
		game.setVsHuman();
		start(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_YELLOW);
	}

	void setVsCOM() {
		game.setVsCOM();
	}

	void setSenko() {
		game.setSenko();
	}

	void setKoko() {
		game.setKoko();
	}


	/*===========================*/
	//    Save and Load
	/*===========================*/
	bool hasSaveFile() {
		ifstream file(saveFilePath);
		if (file) {
			file.close();
			return true;
		}
		else {
			file.close();
			return false;
		}
	}

	void save() {
		game.save(saveFilePath);
	}

	void load() {
		start(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_YELLOW);
		game.load(saveFilePath);
	}


	/*===========================*/
	//    Battle Mode
	/*===========================*/
	int show(COM& com, bool debug) {
		int return_flg = FLAG_BATTLE;

		game.drawBeforePlay();

		if (playByPlayer()) {
			if (game.isVsCOM()) com.setWait();
		}
		
		VectorXd input = game.stateToInput();

		// 学習機械の出力描画
		if (likelihoodFlg) {
			com.visualize(input);
		}

		// COMの手番
		if (!game.isPlayTurn()) {
			com.play(input, game.board, game.currentSide());
			//com.playMinMax(game.board, game.currentSide());
			game.update(com.choice);
		}

		game.drawAfterPlay();
		drawMessage();
		drawComment();
		cutin.update();

		// 勝利判定
		if (game.victory() != 0) {
			UserInput::reset();
			return_flg = FLAG_RESULT;
		}

		if (cancelPlay()) com.setWait();
		if (saveOrReset()) return_flg = FLAG_TITLE;

		return return_flg;
	}

	void drawMessage() {
		DrawFormatString(470, 80, strColor, "右クリック:");
		DrawFormatString(540, 100, strColor, "石を置く");
		DrawFormatString(470, 124, strColor, "zキー（BkSpキー）:");
		DrawFormatString(540, 144, strColor, "一手戻る");
	}

	void drawComment() {
		// コメント描画
		if (commentFlg) comment.draw(strColor);
		// コメント差し替え
		comment.update(playCnt);
	}

	void updateCommentInBattle() {
		comment.forceUpdate(COMMENT_CHANGE_TYPE_PLAY_TURN, 0.40);
	}

	void toggleByKey(bool debug) {
		// コメントを消す
		if (UserInput::onKeyComment()) {
			commentFlg = !commentFlg;
		}

		if (debug) {
			// カットインを入れる
			if (UserInput::onKeyCutinDebug()) {
				cutin.flg = 1;
			}

			// 学習機械の出力を見る
			if (UserInput::onKeyVisualizeDebug()) {
				likelihoodFlg = !likelihoodFlg;
			}
		}
	}


	/*===========================*/
	//    デバッグ情報
	/*===========================*/
	void debugDump() {
		game.debugDump();

		int strColor = strColorDebug;
		// Option
		DrawFormatString(5, 185, strColor, "playCnt: %d", playCnt);
		DrawFormatString(5, 205, strColor, "likeliFlg: %d", likelihoodFlg);
		DrawFormatString(5, 225, strColor, "commentFlg: %d", commentFlg);
		// Comment
		comment.debugDump(strColor);
	}

private:

	bool playByPlayer() {
		if (game.isPlayTurn() && game.playTurn()) {
			double res = game.update();
			if (game.isUpdated(res)) {
				updateCommentInBattle();
				playCnt = 0;
				return true;
			}
		}
		return false;
	}

	bool cancelPlay() {
		return (UserInput::onBack() && game.goBackHist());
	}

	bool saveOrReset() {
		bool no_keyboard = true;
		int choice = menu.choose(strColor, no_keyboard);

		// save
		if (choice == 0) save();

		//reset
		if (choice == 0 || choice == 1) {
			UserInput::reset();
			game.reset(true);
			return true;
		}

		return false;
	}
};
