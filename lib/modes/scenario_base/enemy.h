#pragma once

#include "lib/utils/character.h"
#include "lib/components/game.h"
#include "lib/components/anime/fade_cutin.h"

class Enemy {
private:
	int comLevel = COM_LEVEL0;
	FadeCutin cutin;

public:
	Enemy(int character) {
		cutin.setCharacter(character);
	}

	Coordinate play(Game game) {
		VectorXd input = game.stateToInput();
		COM::play(input, game.board, game.currentSide(), comLevel);
		return COM::choice;
	}

	void showCutin() {
		return cutin.start();
	}

	void updateCutin() {
		return cutin.update();
	}
};
