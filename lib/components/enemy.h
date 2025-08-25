#pragma once

#include "lib/utils/character.h"
#include "lib/components/game.h"
#include "lib/components/anime/fade_cutin.h"

class Enemy {
protected:
	int comLevel = COM_LEVEL0;
	FadeCutin cutin;

public:
	Enemy() {}

	Enemy(int character) {
		cutin.setCharacter(character);
	}

	Coordinate play(Game game) {
		VectorXd input = game.stateToInput();
		COM::play(input, game.board, game.currentSide(), comLevel);
		return COM::choice;
	}

	void showCutin(Game game) {
		if (!shouldShowCutin(game)) return;
		
		return cutin.start();
	}

	void showCutinDebug() {
		return cutin.start();
	}

	void updateCutin() {
		return cutin.update();
	}

private:
	virtual bool shouldShowCutin(Game game) {
		return false;
	}
};
