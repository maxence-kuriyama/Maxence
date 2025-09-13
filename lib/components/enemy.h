#pragma once

#include "lib/utils/user_input.h"
#include "lib/components/game.h"
#include "lib/components/character.h"
#include "lib/components/anime/fade_cutin.h"
#include "lib/components/message.h"

const int SKILL_USING_STATUS_NONE(-1);
const int SKILL_USING_STATUS_RELOADED(0);
const int SKILL_USING_STATUS_PRE_MESSAGE(10);
const int SKILL_USING_STATUS_START_CUTIN(20);
const int SKILL_USING_STATUS_CUTIN(30);
const int SKILL_USING_STATUS_DO_SKILL(40);
const int SKILL_USING_STATUS_DONE_SKILL(50);

const int MAX_SKILL_NUM(5);

struct skillInfo {
	string message;
	string name;
};

class Enemy {
protected:
	FadeCutin cutin;
	Message msg;

	int comLevel = COM_LEVEL0;
	int skillUsingStatus = SKILL_USING_STATUS_RELOADED;

	int skillIndex = 0;
	struct skillInfo skills[MAX_SKILL_NUM] = {
		{ "", "" }
	};

public:
	int who = MESSAGE_WHO_DESC;

	Enemy() {
		initialize();
	}

	Enemy(int character) {
		initialize();
		cutin.setCharacter(character);
	}

	Coordinate play(Game game) {
		VectorXd input = game.stateToInput();
		COM::play(input, game.board, game.currentSide(), comLevel);
		return COM::choice;
	}

	bool useSkill(Game &game) {
		if (!shouldUseSkill(game)) {
			skillUsingStatus = SKILL_USING_STATUS_RELOADED;
			return false;
		}

		switch (skillUsingStatus) {
		case SKILL_USING_STATUS_RELOADED:
			skillUsingStatus = SKILL_USING_STATUS_PRE_MESSAGE;
			break;
		case SKILL_USING_STATUS_PRE_MESSAGE:
			drawPreMessage();
			break;
		case SKILL_USING_STATUS_START_CUTIN:
			startCutin();
			break;
		case SKILL_USING_STATUS_CUTIN:
			checkCutin();
			break;
		case SKILL_USING_STATUS_DO_SKILL:
			doSkill(game);
			break;
		case SKILL_USING_STATUS_DONE_SKILL:
			checkDone();
		default:
			return false;
		}
		return true;
	}

	bool usingSkill() {
		return (skillUsingStatus == SKILL_USING_STATUS_PRE_MESSAGE
			|| skillUsingStatus == SKILL_USING_STATUS_START_CUTIN
			|| skillUsingStatus == SKILL_USING_STATUS_CUTIN
			|| skillUsingStatus == SKILL_USING_STATUS_DO_SKILL);
	}

	bool doneSkill() {
		return (skillUsingStatus == SKILL_USING_STATUS_DONE_SKILL);
	}

	void showCutinDebug() {
		return startCutin();
	}

	void tick() {
		return cutin.update();
	}

private:

	void initialize() {
		skillUsingStatus = SKILL_USING_STATUS_RELOADED;
		skillIndex = 0;

		cutin.initialize();
		msg.initialize();
	}

	void drawPreMessage() {
		if (!msg.isShown) {
			string skillMessage = skills[skillIndex].message;
			msg.setWithoutNext(skillMessage, who);
		}

		msg.draw();
		if (isOnReturnOrClicked() && msg.skip()) {
			skillUsingStatus = SKILL_USING_STATUS_START_CUTIN;
		}
	}

	void startCutin() {
		skillUsingStatus = SKILL_USING_STATUS_CUTIN;
		string skillName = skills[skillIndex].name;
		cutin.setSkillName(skillName);
		return cutin.start();
	}

	void checkCutin() {
		if (cutin.active) return;
		
		skillUsingStatus = SKILL_USING_STATUS_DO_SKILL;
		skillIndex++;
	}

protected:

	virtual void doSkill(Game &game) {
		finishSkill();
	}

	void finishSkill() {
		skillUsingStatus = SKILL_USING_STATUS_DONE_SKILL;
	}

	void checkDone() {
		skillUsingStatus = SKILL_USING_STATUS_NONE;
	}

	virtual bool shouldUseSkill(Game game) {
		return false;
	}

	bool isOnReturnOrClicked() {
		Key* keyboard = UserInput::getKey();
		Mouse* mouse = UserInput::getMouse();
		return (keyboard->onReturn() || mouse->click());
	}
};
