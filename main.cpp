#include "DxLib.h"

using namespace DxLib;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetMainWindowText("Maxence 0.4.0");
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//ÉÅÉCÉìÉãÅ[Év
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {
		WaitTimer(10);
	}

	DxLib_End();

	return 0;
}