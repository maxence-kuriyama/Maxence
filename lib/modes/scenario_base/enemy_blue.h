#pragma once

#include "lib/components/enemy.h"

class EnemyBlue : public Enemy {
public:

	EnemyBlue() {
		comLevel = COM_LEVEL2;
		who = MESSAGE_WHO_BLUE;
		skills[0] = { "Ç‹Ç∏ÇÕä»íPÇ»à•éAÇ©ÇÁÇÕÇ∂ÇﬂÇ‹ÇµÇÂÇ§Ç©", "å∂âe - Blue Phantasm" };
		skills[1] = { "Ç±Ç±Ç©ÇÁÇ™ñ{î‘Ç≈Ç∑ÅI", "óHâe - Will o the Wisp" };
		cutin.setCharacter(CHARACTER_WHO_BLUE);
	}

private:

	bool shouldUseSkill(Game game) {
		return ((skillIndex == 0 && game.getTurnCount() / 2 + 1 == 8)
			|| (skillIndex == 1 && game.getTurnCount() / 2 + 1 == 15));
	}
};
