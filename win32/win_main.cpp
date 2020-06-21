/*
==========================================
Copyright (c) 2020 Ostrich Labs

Entry point for Windows
==========================================
*/

#include "../common/ost_common.h"

#if (OST_WINDOWS != 1)
#   error "This module should only be included in Windows builds"
#endif

#include <Windows.h>

#include <iostream>

#include "win_display.h"
#include "win_input.h"
#include "../common/error.h"
#include "../game/ost_main.h"
#include "../gl4/gl4_renderer.h"

namespace {

ostrich::Main Game;

ostrich::DisplayWindows Display;
ostrich::IDisplay*      DisplayPtr = &Display;

ostrich::GL4Renderer    Renderer;
ostrich::IRenderer*     RendererPtr = &Renderer;

ostrich::InputWindows   Input;
ostrich::IInput*        InputPtr = &Input;

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNUSED_PARAMETER(hInstance);
    UNUSED_PARAMETER(hPrevInstance);
    UNUSED_PARAMETER(lpCmdLine);
    UNUSED_PARAMETER(nCmdShow);
    
    int returncode = 0;

    //magpie::InitMemoryTracker();

    try {
        returncode = Game.Start(DisplayPtr, RendererPtr, InputPtr);
        Game.Destroy();
    }
    catch (const std::exception &e) {
        std::cerr << u8"Standard exception during runtime: " << e.what() <<
            ost_char::g_NewLine;
        returncode = -2;
    }
    catch (...) {
        std::cerr << u8"Unknown exception during runtime" << ost_char::g_NewLine;
        returncode = -3;
    }

    //magpie::PrintMemoryTracker();
    //magpie::DestroyMemoryTracker();

    if (returncode == 0)
        returncode = ::GetLastError();

    return returncode;
}
