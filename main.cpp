#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1) {
		return -1;
	}

	while (ProcessMessage() == 0) {
		WaitTimer(10);
	}

	DxLib_End();

	return 0;
}