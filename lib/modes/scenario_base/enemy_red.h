#pragma once

#include "lib/components/enemy.h"

class EnemyRed : public Enemy {
public:

	EnemyRed() {
		comLevel = COM_LEVEL0;
		who = MESSAGE_WHO_RED;
		skills[0] = { "これを使わせるとはな！！", "燃やし尽くす義憤" };
		cutin.setCharacter(CHARACTER_WHO_RED);
	}

private:

	bool shouldUseSkill(Game game) {
		return (game.getTurnCount() / 2 + 1 == 12);
	}
};
