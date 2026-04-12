// JADEFRACTURE
// win_main.c

#include "win_main.h"

#include "../game_main.h"

// Win32 uses its own entrypoint which is kind of weird... but understandable I guess... Nozomi 04-09-2026
__stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	gameMain();

	return 0;
}