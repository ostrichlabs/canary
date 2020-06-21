/*
==========================================
Copyright (c) 2020 Ostrich Labs

Entry point for the Raspberry Pi
==========================================
*/

#include "../common/ost_common.h"

#if (OST_RASPI != 1)
#   error "This module should only be included in Raspberry Pi builds"
#endif

#include <iostream>

#include "raspi_display.h"
#include "../common/error.h"
#include "../game/ost_main.h"
#include "../gles2/gles2_renderer.h"
#include "../linux/linux_input.h"

namespace {

ostrich::Main Game;

ostrich::DisplayRaspi	Display;
ostrich::IDisplay*		DisplayPtr = &Display;

ostrich::EGLRenderer 	Renderer;
ostrich::IRenderer*  	RendererPtr = &Renderer;

ostrich::InputLinux     Input;
ostrich::IInput*        InputPtr = &Input;

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    int returncode = 0;

    //magpie::InitMemoryTracker();

    try {
        returncode = Game.Start(DisplayPtr, RendererPtr, InputPtr);
    }
    catch (const std::exception &e) {
        std::cerr << u8"Standard exception during runtime: " << e.what() <<
            ost_char::g_NewLine;
        returncode = -400000;
    }
    catch (...) {
        std::cerr << u8"Unknown exception during runtime" << ost_char::g_NewLine;
        returncode = -500000;
    }

    Game.Destroy();

    //magpie::PrintMemoryTracker();
    //magpie::DestroyMemoryTracker();
    return returncode;
}
