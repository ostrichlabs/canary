/*
==========================================
Copyright (c) 2020 Ostrich Labs

Main game object
==========================================
*/

#ifndef OST_MAIN_H_
#define OST_MAIN_H_

#include "../common/ost_common.h"
#include "eventqueue.h"
#include "i_display.h"
#include "i_input.h"
#include "i_renderer.h"
#include "../common/console.h"
#include "../minesweeper/ms_statemachine.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class Main {
public:

	Main() noexcept;
    ~Main();
    Main(Main &&) = delete;
    Main(const Main &) = delete;
    Main &operator=(Main &&) = delete;
    Main &operator=(const Main &) = delete;

    int Start(IDisplay *display, IRenderer *renderer, IInput *input);

    int Initialize();
    void Destroy();

    void Run();

private:

    void ProcessInput();
    bool UpdateState();
    void RenderScene(int32_t extrapolation);

    bool m_isActive;

    const char *const m_Classname = u8"ostrich::Main";

    IInput *m_Input;
    IDisplay *m_Display;
    IRenderer *m_Renderer;

    Console m_Console;
    ConsolePrinter m_ConsolePrinter;

    EventQueue m_EventQueue;

    // game dependent - lives in the game's folder
    ms::StateMachine m_GameState;
};

} // namespace ostrich

#endif /* OST_MAIN_H_ */
