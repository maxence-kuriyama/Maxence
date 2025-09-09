#pragma once

#include "lib/components/enemy.h"

class EnemyBlue : public Enemy {
public:

	EnemyBlue() {
		comLevel = COM_LEVEL2;
		who = MESSAGE_WHO_BLUE;
		skills[0] = { "�܂��͊ȒP�Ȉ��A����͂��߂܂��傤��", "���e - Blue Phantasm" };
		skills[1] = { "�������炪�{�Ԃł��I", "�H�e - Will o the Wisp" };
		cutin.setCharacter(CHARACTER_WHO_BLUE);
	}

private:

	bool shouldUseSkill(Game game) {
		return ((skillIndex == 0 && game.getTurnCount() / 2 + 1 == 8)
			|| (skillIndex == 1 && game.getTurnCount() / 2 + 1 == 15));
	}
};
