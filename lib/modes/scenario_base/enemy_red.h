#pragma once

#include "lib/components/enemy.h"

class EnemyRed : public Enemy {
public:
	EnemyRed() {
		cutin.setCharacter(CHARACTER_WHO_RED);
	}

private:
	bool shouldUseSkill(Game game) {
		return (game.getTurnCount() / 2 == 2);
	}
};
