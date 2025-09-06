#pragma once

#include "lib/components/game.h"
#include "lib/components/character.h"
#include "lib/components/anime/fade_cutin.h"

class Enemy {
protected:
	int comLevel = COM_LEVEL0;
	FadeCutin cutin;
	bool skillReloaded = true;

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

	bool useSkill(Game game) {
		if (!shouldUseSkill(game)) {
			skillReloaded = true;
			return false;
		}

		if (!skillReloaded) return false;

		showCutin(game);
		return true;
	}

	void showCutinDebug() {
		return cutin.start();
	}

	void tick() {
		return cutin.update();
	}

private:
	void showCutin(Game game) {
		skillReloaded = false;
		return cutin.start();
	}

	virtual bool shouldUseSkill(Game game) {
		return false;
	}
};
