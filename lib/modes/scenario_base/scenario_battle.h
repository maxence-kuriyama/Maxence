#pragma once

#include <string>
#include "lib/utils/com.h"
#include "lib/utils/user_input.h"
#include "lib/utils/encrypter.h"
#include "lib/utils/character.h"
#include "lib/components/game.h"
#include "lib/components/anime/fade_cutin.h"
#include "./enemy.h"

using namespace std;

class ScenarioBattle {
protected:
	FadeCutin cutin;
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

		if (FlagStore::isDebug() && UserInput::onKeyCutinDebug()) {
			cutin.start();
		}

		showBefore();
		showAfter();
	}

	void showBefore() {
		game.drawBeforePlay();
	}

	void showAfter() {
		game.drawAfterPlay();
		cutin.update();
	}

	void start(int player1, int player2) {
		game.prepare(player1, player2);
		game.setVsCOM();
		onGame = true;

		cutin.setCharacter(getCharacter(player2));
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

	bool playByCom(Enemy enemyCom) {
		Coordinate choice = enemyCom.play(game);

		double res = game.update(choice);
		return game.isUpdated(res);
	}

	string getTrigger() {
		return trigger;
	}

	bool checkTriggered() {
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

	bool isTriggered() {
		return (trigger == "fired");
	}

	bool hasNoTrigger() {
		return (trigger == "");
	}

	bool isWon() {
		return (game.victory() == 1);
	}

	bool isLost() {
		return (game.victory() == -1);
	}

	bool isDraw() {
		return (game.victory() == 10);
	}

	Coordinate last() {
		return game.last();
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

private:

	int getCharacter(int player) {
		switch (player) {
		case BATTLE_PLAYER_YELLOW:
			return CHARACTER_WHO_YELLOW;
		case BATTLE_PLAYER_BLUE:
			return CHARACTER_WHO_BLUE;
		case BATTLE_PLAYER_RED:
			return CHARACTER_WHO_RED;
		case BATTLE_PLAYER_GREEN:
			return CHARACTER_WHO_GREEN;
		case BATTLE_PLAYER_DEER:
			return CHARACTER_WHO_DEER;
		case BATTLE_PLAYER_PLAYER:
			return CHARACTER_WHO_PL_YELLOW;
		default:
			return CHARACTER_WHO_NONE;
		}
	}
};
