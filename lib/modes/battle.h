#pragma once

#include <fstream>
#include "lib/const.h"
#include "lib/mode.h"
#include "lib/utils/flag_store.h"
#include "lib/utils/user_input.h"
#include "lib/utils/com.h"
#include "lib/components/menu.h"
#include "lib/components/game.h"
#include "lib/components/logo.h"
#include "lib/components/anime/slide_cutin.h"
#include "./battle/comment.h"
#include "./battle/result.h"

const int BATTLE_MODE_BATTLE(0);
const int BATTLE_MODE_RESULT(1);

// バトルモード
// 主にGameの操作と描画
// Gameの描画はEndingとBattleResultでも行うため、公開する必要がある
class Battle {
private:
	Menu menu;
	Button btnSave;
	Button btnReset;

	SlideCutin cutin;
	Comment comment;
	Logo logo;
	Result result;
	double theta = 0.3;

	int strColor = GetColor(255, 255, 255);
	int strColorDebug = GetColor(50, 50, 200);
	const string saveFilePath = SAVE_FILE_BATTLE_GAME;

	bool likelihoodFlg = false; // 学習機械の出力フラグ
	bool commentFlg = false;
	int playCnt = 0;			// 1ターンに費やしたカウント
	int innerMode = BATTLE_MODE_BATTLE;

public:
	Game game;

	Battle() {
		UserInput::reset();

		game.initialize();
		comment.initialize();

		// ボタン初期化
		btnSave.initialize(TEXT_SAVE_X, TEXT_SAVE_Y, "中断");
		btnReset.initialize(TEXT_RESET_X, TEXT_RESET_Y, "タイトル");
		menu.set(btnSave, btnReset);
	}

	~Battle() {}

	void start(int player1, int player2, bool init = true) {
		UserInput::reset();
		game.prepare(player1, player2, init);
	}

	void startVsHuman() {
		game.setVsHuman();
		start(BATTLE_PLAYER_YELLOW, BATTLE_PLAYER_YELLOW);
	}

	void startNewVsHuman() {
		if (hasSaveFile()) return;

		startVsHuman();
	}

	void resume(int player1, int player2) {
		start(player1, player2, false);
	}

	/*===========================*/
	//    Battle Mode
	/*===========================*/
	int show() {
		SetBackgroundColor(0, 0, 0);

		if (innerMode == BATTLE_MODE_RESULT) return showResult();

		toggleByKey();
		tick();
		game.drawBeforePlay();

		if (playByPlayer()) {
			if (game.isVsCOM()) COM::setWait();
		}
		
		VectorXd input = game.stateToInput();

		// 学習機械の出力描画
		if (likelihoodFlg) {
			COM::visualize(input);
		}

		// COMの手番
		if (!game.isPlayTurn()) {
			COM::play(input, game.board, game.currentSide());
			game.update(COM::choice);
		}

		game.drawAfterPlay();
		drawMessage();
		drawComment();
		cutin.update();
		logo.draw(true);

		// 勝利判定
		if (game.victory() != 0) {
			UserInput::reset();
			innerMode = BATTLE_MODE_RESULT;
		}

		if (cancelPlay()) COM::setWait();
	
		if (saveOrReset()) return MODE_TITLE;

		return MODE_BATTLE;
	}

	void route(Mode& mode, int res) {
		switch (res) {
		case MODE_TITLE:
			// initialize();
			mode.goTitle();
			break;
		}
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

	int showResult() {
		int res = result.show(game);

		if (res == MODE_BATTLE) {
			innerMode = BATTLE_MODE_BATTLE;
			start(BATTLE_PLAYER_NONE, BATTLE_PLAYER_NONE);
		}
		else if (res == MODE_RESULT_CANCEL) {
			innerMode = BATTLE_MODE_BATTLE;
			resume(BATTLE_PLAYER_NONE, BATTLE_PLAYER_NONE);
		}

		return MODE_BATTLE;
	}

	void tick() {
		playCnt = (playCnt + 1) % 10000000;
	}

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

	void toggleByKey() {
		// コメントを消す
		if (UserInput::onKeyComment()) {
			commentFlg = !commentFlg;
		}

		if (FlagStore::isDebug()) {
			// カットインを入れる
			if (UserInput::onKeyCutinDebug()) {
				cutin.start();
			}

			// 学習機械の出力を見る
			if (UserInput::onKeyVisualizeDebug()) {
				likelihoodFlg = !likelihoodFlg;
			}
		}
	}

	/*===========================*/
	//    描画系
	/*===========================*/
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
};
