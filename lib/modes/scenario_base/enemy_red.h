#pragma once

#include "lib/components/enemy.h"

class EnemyRed : public Enemy {
public:

	EnemyRed() {
		comLevel = COM_LEVEL0;
		who = MESSAGE_WHO_RED;
		skillMessages[0] = "������g�킹��Ƃ͂ȁI�I";
		cutin.setCharacter(CHARACTER_WHO_RED);
	}

private:

	bool shouldUseSkill(Game game) {
		return (game.getTurnCount() / 2 == 2);
	}
};
