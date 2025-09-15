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
	void doSkill(Game& game) {
		switch (skillIndex) {
		case 0:
			controlPlayer(game);
			break;
		case 1:
			whitenField(game);
			break;
		}
	}

	void controlPlayer(Game& game) {
		switch (cntPlayedTurns) {
		case 0:
			COM::setWait(0);
			if (play(game)) cntPlayedTurns++;
			break;
		case 1:
			if (play(game)) cntPlayedTurns++;
			break;
		case 2:
			if (play(game)) finishSkill();
			break;
		}
	}

	void whitenField(Game& game) {
		/*
		Game copied = game;

		COM::setWait(0);
		Coordinate choice = play(copied);
		double res = copied.update(choice);

		if (copied.isUpdated(res)) {
			game.forceWinLocalCheat(choice);
			COM::resetPlaying();
			finishSkill();
		}
		*/
	}
};
