/*
==========================================
Copyright (c) 2020-2021 Ostrich Labs

Entry point for Linux using X11
==========================================
*/

#include "../common/ost_common.h"

#if (OST_LINUX != 1)
#   error "This module should only be included in Linux builds"
#endif

#include "x11_input.h"
#include "x11_display.h"
#include "../common/error.h"
#include "../game/ost_main.h"
#include "../gl4/gl4_renderer.h"

namespace {

ostrich::Main Game;

ostrich::DisplayX11     DisplayObj;
ostrich::IDisplay*      DisplayPtr = &DisplayObj;

ostrich::GL4Renderer    Renderer;
ostrich::IRenderer*     RendererPtr = &Renderer;

ostrich::InputX11       Input;
ostrich::IInput*        InputPtr = &Input;

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    OST_UNUSED_PARAMETER(argc);
    OST_UNUSED_PARAMETER(argv);

    int returncode = 0;

    //magpie::InitMemoryTracker();

    try {
        returncode = Game.Start(DisplayPtr, RendererPtr, InputPtr);
    }
    catch (...) {
        returncode = -500000;
    }

    Game.Destroy();

    //magpie::PrintMemoryTracker();
    //magpie::DestroyMemoryTracker();

    return returncode;
}
