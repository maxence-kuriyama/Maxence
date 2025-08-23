#pragma once

#include "lib/components/game.h"

class Enemy {
private:
	int comLevel = COM_LEVEL0;

public:
	Enemy() {}
	~Enemy() {}

	Coordinate play(Game game) {
		VectorXd input = game.stateToInput();
		COM::play(input, game.board, game.currentSide(), comLevel);
		return COM::choice;
	}
};
