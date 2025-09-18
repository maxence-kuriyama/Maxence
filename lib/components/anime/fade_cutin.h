#pragma once

#include "lib/components/anime.h"
#include "lib/components/character.h"

const int FADE_CUTIN_RIGHT_CHARA_X(400);
const int FADE_CUTIN_RIGHT_CHARA_Y(0);

// 試合中のアニメーションクラス
class FadeCutin : public AnimeBase {
private:
	static int fontHandle;
	int Black = GetColor(0, 0, 0);
	int strColor = GetColor(255, 255, 255);
	string skillName = "";

	Character chara;

public:

	FadeCutin() {
		initialize();
		if (FadeCutin::fontHandle == NULL) {
			FadeCutin::fontHandle = CreateFontToHandle("游明朝", 16, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
		}
	}

	void initialize() {
		AnimeBase::initialize();

		chara.initialize();
		chara.alpha = 0;
		chara.x = FADE_CUTIN_RIGHT_CHARA_X;
		chara.y = FADE_CUTIN_RIGHT_CHARA_Y;
		skillName = "";
	}

	void setCharacter(int who) {
		chara.initialize(who);
		chara.alpha = 0;
		chara.x = FADE_CUTIN_RIGHT_CHARA_X;
		chara.y = FADE_CUTIN_RIGHT_CHARA_Y;
	}

	void setSkillName(string str) {
		skillName = str;
	}

private:

	void _update() {
		if (cnt <= 15) {
			chara.alpha += 10;
		}
		else if (cnt <= 105) {
			// do nothing
		}
		else if (cnt <= 120) {
			chara.alpha -= 10;
		}
		chara.x -= 1;
		display();
	}

	void display() {
		int screenHandle = MakeScreen(640, 480, TRUE);

		SetDrawScreen(screenHandle);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawBox(-1, -1, 641, 481, Black, TRUE);
		chara.draw(screenHandle);

		SetDrawScreen(DX_SCREEN_BACK);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawGraph(0, 0, screenHandle, TRUE);
		if (skillName != "") {
			const char* skillNameStr = skillName.c_str();
			int strWidth = GetDrawStringWidth(skillNameStr, strlen(skillNameStr)) * 1.65;
			DrawStringToHandle(640 - strWidth, 30, skillNameStr, strColor, FadeCutin::fontHandle);
		}

		DeleteGraph(screenHandle);
	}
};

int FadeCutin::fontHandle = NULL;
