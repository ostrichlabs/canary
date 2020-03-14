/*
==========================================
raspi_entry.cpp

Copyright (c) 2019 Ostrich Labs

Entry point for the Raspberry Pi
==========================================
*/

#include "ost_common.h"

#if (OST_RASPI != 1)
#   error "This module should only be included in Raspberry Pi builds"
#endif

#include <iostream>

#include "display_raspi.h"
#include "ost_main.h"
#include "sys_error.h"
#include "../view/input_raspi.h"
#include "../view/renderer_egl.h"

namespace {

ostrich::Main Game;

ostrich::DisplayRaspi	Display;
ostrich::IDisplay*		DisplayPtr = &Display;

ostrich::EGLRenderer 	Renderer;
ostrich::IRenderer*  	RendererPtr = &Renderer;

ostrich::InputRaspi     Input;
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
    catch (const ostrich::Exception &e) {
        std::cerr << u8"ostrich::Exception during runtime: " << e.what() <<
        	ost_char::g_NewLine;
        returncode = -1;
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

    Game.Destroy();

    //magpie::PrintMemoryTracker();
    //magpie::DestroyMemoryTracker();
    return returncode;
}
