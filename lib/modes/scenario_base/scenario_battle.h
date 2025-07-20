#pragma once

#include <string>
#include "lib/utils/com.h"
#include "lib/utils/user_input.h"
#include "lib/utils/encrypter.h"
#include "lib/components/game.h"

using namespace std;

class ScenarioBattle {
protected:
	Game game;
	bool onGame = false;
	string trigger = "";

public:

	ScenarioBattle() {
		initialize();
	}

	void initialize() {
		onGame = false;
		game.initialize();
		game.setVsCOM();
		trigger = "";
	}

	void show() {
		if (!onGame) return;

		game.drawBeforePlay();
		game.drawAfterPlay();
	}

	void showBefore() {
		game.drawBeforePlay();
	}

	void showAfter() {
		game.drawAfterPlay();
	}

	void start(int player1, int player2) {
		game.prepare(player1, player2);
		game.setVsCOM();
		onGame = true;
	}

	void startTutorial(int player1, int player2) {
		start(player1, player2);
		game.setTutorial();
	}

	void resetGame() {
		UserInput::reset();
		game.reset();
	}

	void setTrigger(string srcTrigger) {
		trigger = srcTrigger;
	}

	bool isOnGame() {
		return onGame;
	}

	bool isPlayTurn() {
		return game.isPlayTurn();
	}

	bool playByPlayer() {
		if (!game.playTurn()) return false;

		double res = game.update();
		return game.isUpdated(res);
	}

	bool playByComLevel0() {
		MinMaxNode node(game.board, game.currentSide());
		int depth = 1;
		int index = node.search(depth);
		Coordinate choice = Board::coordinates(index);

		double res = game.update(choice);
		return game.isUpdated(res);
	}

	bool playByComLevel1(COM& com) {
		// MinMaxNode node(game.board, game.currentSide());
		// int depth = 2;
		// int index = node.search(depth);
		// Coordinate choice = Board::coordinates(index);

		VectorXd input = game.stateToInput();
		com.play(input, game.board, game.currentSide());
		Coordinate choice = com.choice;

		double res = game.update(choice);
		return game.isUpdated(res);
	}

	string getTrigger() {
		return trigger;
	}

	bool isTriggered() {
		if (trigger == "play_once") {
			// do nothing here
		}
		else if (trigger == "local_victory") {
			for (int index = 0; index < 9; index++) {
				if (game.board.localVictory(index)) {
					trigger = "fired";
				}
			}
		}
		else if (trigger == "victory") {
			if (game.victory() != 0) {
				UserInput::reset();
				trigger = "fired";
			}
		}
		else if (trigger == "fired") {
			trigger = "";
			return true;
		}
		return false;
	}

	bool hasNoTrigger() {
		return (trigger == "");
	}

	bool isLost() {
		return (game.victory() == -1);
	}

	void save(const string filePath) {
		if (onGame) {
			game.save(filePath);
		}
	}

	void load(nlohmann::json res, const string filePath) {
		onGame = res["onBattle"];
		trigger = res["battle_trigger"];
		if (onGame) {
			game.load(filePath);
		}
	}
};
