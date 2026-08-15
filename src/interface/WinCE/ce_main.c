// Nozomi Engine
// WinCE backend
// ce_main.c

#include "ce_main.h"
#include "ce_input.h"

#include "../../i_event.h"
#include "../../i_system.h"
#include "../../i_video.h"

#include "../../game_defs.h"
#include "../../game_main.h"
#include "../../game_video.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_PAINT:
		{
			PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			BITMAPINFO bmi;

			ZeroMemory(&bmi, sizeof(bmi));

			bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth       = VID_WIDTH;
			bmi.bmiHeader.biHeight      = -VID_HEIGHT;
			bmi.bmiHeader.biPlanes      = 1;
			bmi.bmiHeader.biBitCount    = 32;
			bmi.bmiHeader.biCompression = BI_RGB;
		
			StretchDIBits(
				hdc,
				-8, 64,
				256, 192,
				0, 0,
				VID_WIDTH, VID_HEIGHT,
				dibPixels,
				&bmi,
				DIB_RGB_COLORS,
				SRCCOPY
			);

            EndPaint(hWnd, &ps);
            return 0;
		}

		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
				DestroyWindow(hWnd);
			return 0;

		case WM_CLOSE:
			DestroyWindow(hWnd);
			return 0;

		case WM_DESTROY:
			game_quit = true;
			PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND g_hWnd = NULL;

int WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {0};

    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wc.lpszClassName = L"NozomiWindowClass";
    RegisterClass(&wc);

    g_hWnd = CreateWindow(
        L"NozomiWindowClass", 
		L"Nozomi Engine",     
        WS_VISIBLE,          
        0, 0,                
        240, 320,            
        NULL, NULL, hInst, NULL
    );

	ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);
	gameMain();
	
	I_printf("Starting main game loop...\n");
	gameLoop();
	
	I_printf("Quitting %s...\n", GAME_NAME);
	exit(0);
}
