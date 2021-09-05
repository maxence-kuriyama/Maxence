#include "DxLib.h"

using namespace DxLib;

class Game {
public:
	unsigned int White = GetColor(255, 255, 255);
	unsigned int Black = GetColor(0, 0, 0);
	int globalX = 1;
	int globalY = 1;
	int localX = 1;
	int localY = 1;

	void drawBase() {
		DrawBox(160, 80, 460, 380, White, TRUE);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				DrawBox(160 + 100 * i, 80 + 100 * j, 160 + 100 * (i + 1), 80 + 100 * (j + 1), Black, FALSE);
			}
		}
	}

	void drawCurrentCoord() {
		int upLeftX = 160 + 100 * globalX + 33 * localX;
		int upLeftY = 80 + 100 * globalY + 33 * localY;
		int lowRightX = 160 + 100 * globalX + 33 * (localX + 1) + 1;
		int lowRightY = 80 + 100 * globalY + 33 * (localY + 1) + 1;
		DrawBox(upLeftX, upLeftY, lowRightX, lowRightY, Black, FALSE);
	}
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetMainWindowText("Maxence 0.4.0");
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	Game game;

	//ƒƒCƒ“ƒ‹[ƒv
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {

		game.drawBase();
		game.drawCurrentCoord();
	}

	DxLib_End();

	return 0;
}