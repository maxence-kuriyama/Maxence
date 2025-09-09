#pragma once

#include "lib/components/enemy.h"

class EnemyYellow : public Enemy {
public:

	EnemyYellow() {
		comLevel = COM_LEVEL2;
		who = MESSAGE_WHO_YELLOW;
		skills[0] = { "����Ȃ��̂���Ȃ����낤�I", "�I���̎n�܂�" };
		skills[1] = { "���̒��q���I", "���X�g�X�p�[�g" };
		skills[2] = { "�����c����Łc\n�Ōゾ�c", "������" };
		cutin.setCharacter(CHARACTER_WHO_YELLOW);
	}

private:

	bool shouldUseSkill(Game game) {
		return ((skillIndex == 0 && game.getTurnCount() / 2 + 1 == 12)
			|| (skillIndex == 1 && game.getTurnCount() / 2 + 1 == 17)
			|| (skillIndex == 2 && game.getTurnCount() / 2 + 1 == 22));
	}
};
