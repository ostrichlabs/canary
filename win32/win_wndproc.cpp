/*
==========================================
win_wndproc.cpp

Copyright (c) 2020 Ostrich Labs

Windows message pump
==========================================
*/

#include "win_wndproc.h"

/////////////////////////////////////////////////
// Messages handling:
//
// WM_DESTROY - this is sent after OpenGL is initialized, because we create a window to get WGL
//              so we can't really respond to this message cleanly
// WM_CLOSE - post a Quit, which we will handle via IInput
// 0x0737 and 0x0738 - I have no idea what these messages are
/////////////////////////////////////////////////
LRESULT CALLBACK ostrich::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CLOSE:
            //::PostQuitMessage(0);
            break;
    }

    return DefWindowProcW(hWnd, message, wParam, lParam);;
}
