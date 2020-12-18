/*
==========================================
Copyright (c) 2020 Ostrich Labs

Main game object
==========================================
*/

#ifndef OSTRICH_OST_MAIN_H_
#define OSTRICH_OST_MAIN_H_

#include "eventqueue.h"
#include "i_display.h"
#include "i_input.h"
#include "i_renderer.h"
#include "../common/console.h"
#include "../common/ost_common.h"
#include "../minesweeper/ms_statemachine.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class Main {
public:

    /////////////////////////////////////////////////
    // Constructor is actually defined in the cpp (TODO: does it need to be?)
    // Destructor is actually defined in the cpp, but due to some timing issues currently does nothing (TODO: should be investigated)
    // Copy/move constructors/operators are deleted, as this object shouldn't move around. It's effectively the core of the exe.
	Main() noexcept;
    ~Main();
    Main(Main &&) = delete;
    Main(const Main &) = delete;
    Main &operator=(Main &&) = delete;
    Main &operator=(const Main &) = delete;

    /////////////////////////////////////////////////
    // Start the game. In program flow terms, this is basically main()
    // Once the program calls this method, it won't return until it's over.
    // Platform-specific modules provide pointers to subsystem objects based on the platform (or, maybe, command line parameters)
    //
    // in:
    //      display - a pointer to an IDisplay-derived object
    //      renderer - a pointer to an IRenderer-derived object
    //      input - a pointer to an IInput-derived object
    // returns:
    //      the result of Initialize()
    int Start(IDisplay *display, IRenderer *renderer, IInput *input);

    /////////////////////////////////////////////////
    // Clean up the game after a successful (or unsuccessful) run.
    // Should be called manually after Start() returns.
    // TODO: Does it need to be called outside the class, or should the class Destroy() itself?
    //
    // returns:
    //      void
    void Destroy();

private:

    /////////////////////////////////////////////////
    // Initialize each subsystem.
    // Should not throw any exceptions from initialization; return a code if something goes wrong.
    // See errorcodes.h for a list
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int Initialize();

    /////////////////////////////////////////////////
    // Run the main game loop.
    // Exactly when to exit the game loop will depend on game logic.
    //
    // returns:
    //      void
    void Run();

    /////////////////////////////////////////////////
    // Invoke the IInput object to process OS and device input messages.
    //
    // returns:
    //      void
    void ProcessInput();

    /////////////////////////////////////////////////
    // Event Queue message pump.
    // Updates the state manager and processes system messages.
    //
    // returns:
    //      true if the game should stop running (is "done")
    bool UpdateState();

    /////////////////////////////////////////////////
    // Process system-type messages that must be handled by Main itself.
    //
    // returns:
    //      true if the game should stop running (is "done")
    bool ProcessSystemMessage(const Message &msg);

    /////////////////////////////////////////////////
    // Notifies the renderer to draw the current scene based on scene data from the state manager.
    // Currently does very little apart from call the IRenderer and swap buffers, but will become more complex as features are added.
    //
    // in:
    //      extrapolation - how far into the next frame we are, in milliseconds (lag/msperframe)
    void RenderScene(int32_t extrapolation);

    bool m_isActive;

    const char *const m_Classname = u8"ostrich::Main"; // for exception reporting

    IInput *m_Input;
    IDisplay *m_Display;
    IRenderer *m_Renderer;

    Console m_Console;
    ConsolePrinter m_ConsolePrinter;

    EventQueue m_EventQueue;

    // game dependent - lives in the game's folder
    // TODO: has to be a better way to do this dynamically
    ms::StateMachine m_GameState;
};

} // namespace ostrich

#endif /* OSTRICH_OST_MAIN_H_ */
