#pragma once

#include "lib/components/enemy.h"

class EnemyYellow : public Enemy {
public:

	EnemyYellow() {
		comLevel = COM_LEVEL2;
		who = MESSAGE_WHO_YELLOW;
		skills[0] = { "そんなものじゃないだろう！", "終わりの始まり" };
		skills[1] = { "その調子だ！", "ラストスパート" };
		skills[2] = { "もう…これで…\n最後だ…", "鎮魂歌" };
		cutin.setCharacter(CHARACTER_WHO_YELLOW);
	}

private:

	bool shouldUseSkill(Game game) {
		return ((skillIndex == 0 && game.getTurnCount() / 2 + 1 == 12)
			|| (skillIndex == 1 && game.getTurnCount() / 2 + 1 == 17)
			|| (skillIndex == 2 && game.getTurnCount() / 2 + 1 == 22));
	}
};
