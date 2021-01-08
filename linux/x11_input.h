/*
==========================================
Copyright (c) 2020 Ostrich Labs

IInput implementation for Linux, using X11
==========================================
*/

#ifndef OSTRICH_X11_INPUT_H_
#define OSTRICH_X11_INPUT_H_

#include <csignal>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include "../game/i_input.h"

namespace ostrich {

namespace x11 {

/////////////////////////////////////////////////
// Extract the KeySym from an XKeyEvent.
//
// in:
//      ev - pointer to an XKeyEvent
// returns:
//      an X11 virtual key
KeySym GetVKey(XKeyEvent *ev);

/////////////////////////////////////////////////
// Translates X11 virtual keys to internal ostrich format
//
// in:
//      vkey - a KeySym from an X11 event
// returns:
//      an ostrich virtual key
int32_t TranslateKey(KeySym vkey);

} // namespace x11

/////////////////////////////////////////////////
//
class InputX11 : public IInput {
public:

    /////////////////////////////////////////////////
    // Constructor is simple.
    // Destructor is defined, but it does nothing for now.
    // Copy/move constructors/operators are deleted to prevent accidentally creating two input handlers
    // (if you want to make another, you can do it manually)
    InputX11() noexcept : m_isActive(false), m_Display(nullptr) { }
    virtual ~InputX11() { }
    InputX11(InputX11 &&) = delete;
    InputX11(const InputX11 &) = delete;
    InputX11 &operator=(InputX11 &&) = delete;
    InputX11 &operator=(const InputX11 &) = delete;

    /////////////////////////////////////////////////
    // Initialize the input handler.
    // There's actually some setup to do here to access X11's input handling functions.
    //
    // in:
    //      consoleprinter - an initialized ConsolePrinter for logging
    //      eventsender - an initialized EventSender for passing input data to the queue
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int Initialize(ConsolePrinter consoleprinter, EventSender eventsender) override;

    /////////////////////////////////////////////////
    // Cleans up input setup.
    // Since nothing is really initialized (yet), this does very little.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    void Destroy() override;

    /////////////////////////////////////////////////
    // Check if the object is valid (by checking the m_isActive flag).
    // The flag should be false by default & after Destroy() and true after Initialize()
    //
    // returns:
    //      m_isActive flag
    bool isActive() const noexcept override { return m_isActive; }

    /////////////////////////////////////////////////
    // Process any keyboard/mouse input.
    // For X11, also processes some window-related messages.
    //
    // returns:
    //      void
    void ProcessKBM() override;

    /////////////////////////////////////////////////
    // Process any miscellaneous messages from the OS.
    // For X11, processes signals.
    //
    // returns:
    //      void
    void ProcessOSMessages() override;

private:

    const char * const m_Classname = u8"ostrich::InputX11";

    /////////////////////////////////////////////////
    // Initialize the *nix signal handler
    // Sets up SignalHandler() as the callback function.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    static int InitializeSignalHandler();

    /////////////////////////////////////////////////
    // Callback function for signal handling.
    // Sets the static variables to signal's info.
    //
    // in:
    //      signum - the raised signal
    //      info - pointer to signal information provided by *nix
    //      ucontext - unused
    // returns:
    //      void
    static void SignalHandler(int signum, siginfo_t *info, void *ucontext);

    static volatile int ms_LastRaisedSignal;
    static siginfo_t *ms_SignalInfo;

    ConsolePrinter m_ConsolePrinter;
    EventSender m_EventSender;

    bool m_isActive;

    Display *m_Display;
};

} // namespace ostrich

#endif /* OSTRICH_X11_INPUT_H_ */
