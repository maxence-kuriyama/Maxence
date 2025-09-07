#pragma once

#include "lib/components/enemy.h"

class EnemyRed : public Enemy {
public:
	string skillMessage = { "������g�킹��Ƃ͂ȁI�I" };
	
	EnemyRed() {
		cutin.setCharacter(CHARACTER_WHO_RED);
	}

private:
	bool shouldUseSkill(Game game) {
		return (game.getTurnCount() / 2 == 2);
	}
};
