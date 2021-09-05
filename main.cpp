#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//ÉÅÉCÉìÉãÅ[Év
	while (!ScreenFlip() && ProcessMessage() == 0 && !ClearDrawScreen()) {
		WaitTimer(10);
	}

	DxLib_End();

	return 0;
}