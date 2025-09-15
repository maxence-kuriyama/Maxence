#pragma once

#include "lib/components/enemy.h"

class EnemyGreen : public Enemy {
public:

	EnemyGreen() {
		comLevel = COM_LEVEL1;
		who = MESSAGE_WHO_GREEN;
		skills[0] = { "�ǂ����āc\n�ǂ����ĕ����荇���Ȃ��񂾁c�I", "�Y�꓾�ʉ��̓��X" };
		cutin.setCharacter(CHARACTER_WHO_GREEN);
	}

private:

	bool shouldUseSkill(Game game) {
		return (game.getTurnCount() / 2 + 1 == 12);
	}

	// ���ɒu�������ꏊ�������m��
	void doSkill(Game& game) {
		Game copied = game;

		COM::setWait(0);
		Coordinate choice = play(copied);
		double res = copied.update(choice);

		if (copied.isUpdated(res)) {
			game.forceWinLocalCheat(choice);
			COM::resetPlaying();
			finishSkill();
		}
	}
};
