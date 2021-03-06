/*
==========================================
Copyright (c) 2020-2021 Ostrich Labs

Entry point for Windows
==========================================
*/

#include "../common/ost_common.h"

#if (OST_WINDOWS != 1)
#   error "This module should only be included in Windows builds"
#endif

#include <Windows.h>

#include "win_gl4display.h"
#include "win_input.h"
#include "../common/error.h"
#include "../game/ost_main.h"
#include "../gl4/gl4_renderer.h"

namespace {

ostrich::Main Game;

ostrich::DisplayGL4Windows  Display;
ostrich::IDisplay*          DisplayPtr = &Display;

ostrich::GL4Renderer        Renderer;
ostrich::IRenderer*         RendererPtr = &Renderer;

ostrich::InputWindows       Input;
ostrich::IInput*            InputPtr = &Input;

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    OST_UNUSED_PARAMETER(hInstance);
    OST_UNUSED_PARAMETER(hPrevInstance);
    OST_UNUSED_PARAMETER(lpCmdLine);
    OST_UNUSED_PARAMETER(nCmdShow);
    
    int returncode = 0;

    //InitMemoryTracker();

    try {
        returncode = Game.Start(DisplayPtr, RendererPtr, InputPtr);
    }
    catch (...) {
        returncode = -500000;
    }

    Game.Destroy();

    //PrintMemoryTracker();
    //DestroyMemoryTracker();

    if (returncode == 0)
        returncode = ::GetLastError();

    return returncode;
}
