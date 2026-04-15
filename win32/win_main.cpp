// JADEFRACTURE
// win_main.c

#include "win_main.h"
#include "../game_main.h"

// Win32 prototypes.

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Win32 uses its own entrypoint which is kind of weird... but understandable I guess... Nozomi 04-09-2026
__stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Register the window class.
	WNDCLASS wc;

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "JADEFRACTUREWC";

	RegisterClass(&wc);

	HWND hWnd = CreateWindowEx(
		0,
		"JADEFRACTUREWC",
		"JADEFRACTURE",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hWnd == NULL)
    {
        return 0;
    }

	ShowWindow(hWnd, nShowCmd);

	MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);

				// All painting occurs here, between BeginPaint and EndPaint.

				FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

				EndPaint(hwnd, &ps);
			}
			return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}