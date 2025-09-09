#pragma once

#include "lib/components/enemy.h"

class EnemyGreen : public Enemy {
public:

	EnemyGreen() {
		comLevel = COM_LEVEL1;
		who = MESSAGE_WHO_GREEN;
		skills[0] = { "どうして…\nどうして分かり合えないんだ…！", "忘れ得ぬ我らの日々" };
		cutin.setCharacter(CHARACTER_WHO_GREEN);
	}

private:

	bool shouldUseSkill(Game game) {
		return (game.getTurnCount() / 2 + 1 == 12);
	}
};
