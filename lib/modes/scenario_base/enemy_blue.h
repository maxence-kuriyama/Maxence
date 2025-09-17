#pragma once

#include "lib/components/enemy.h"

class EnemyBlue : public Enemy {
public:

	EnemyBlue() {
		comLevel = COM_LEVEL2;
		who = MESSAGE_WHO_BLUE;
		skills[0] = { "Ç‹Ç∏ÇÕä»íPÇ»à•éAÇ©ÇÁÇÕÇ∂ÇﬂÇ‹ÇµÇÂÇ§Ç©", "óHâe - Will o the Wisp" };
		skills[1] = { "Ç±Ç±Ç©ÇÁÇ™ñ{î‘Ç≈Ç∑ÅI", "å∂âe - Blue Phantasm" };
		cutin.setCharacter(CHARACTER_WHO_BLUE);
	}

private:
	int cntPlayedTurns = 0;

	bool shouldUseSkill(Game game) {
		return ((skillIndex == 0 && game.getTurnCount() / 2 + 1 == 8)
			|| (skillIndex == 0 && game.getTurnCount() / 2 + 1 == 9)
			|| (skillIndex == 1 && game.getTurnCount() / 2 + 1 == 16));
	}

	// éüÇ…íuÇ´ÇΩÇ¢èÍèäÇã≠êßämï€
	void doSkill(Game &game) {
		switch (skillIndex) {
		case 0:
			controlPlayer(game);
			break;
		case 1:
			whitenField(game);
			break;
		}
	}

	void controlPlayer(Game &game) {
		switch (cntPlayedTurns) {
		case 0:
			COM::setWait(0);
			if (play(game)) cntPlayedTurns++;
			break;
		case 1:
			if (playAsPlayer(game)) cntPlayedTurns++;
			break;
		case 2:
			if (play(game)) finishSkill();
			break;
		}
	}

	bool playAsPlayer(Game &game) {
		VectorXd input = game.stateToInput();
		COM::playAsPlayerCheat(input);

		double res = game.update(COM::choice);
		if (game.isUpdated(res)) {
			COM::resetPlaying();
			return true;
		}
		return false;
	}

	void whitenField(Game &game) {
		double minValue = 100.0;
		Coordinate minCoord;

		for (int i = 0; i < 9; i++) {
			Game copied = game;
			Coordinate c = { i / 3, i % 3, 1, 1, DUMMY_LAST_FIELD };
			copied.resetLocalCheat(c);

			// ëäéËÇ©ÇÁå©ÇΩï]âøíl
			double value = COM::evaluateByMinMax(game.board, game.currentSide());
			if (minValue > value) {
				minValue = value;
				minCoord = c;
			}
		}

		game.resetLocalCheat(minCoord);
		finishSkill();
	}
};
