#pragma once

#include "lib/components/enemy.h"

class EnemyRed : public Enemy {
public:
	string skillMessage = { "‚±‚ê‚ğg‚í‚¹‚é‚Æ‚Í‚ÈII" };
	
	EnemyRed() {
		cutin.setCharacter(CHARACTER_WHO_RED);
	}

private:
	bool shouldUseSkill(Game game) {
		return (game.getTurnCount() / 2 == 2);
	}
};
