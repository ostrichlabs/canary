/*
==========================================
Copyright (c) 2020 Ostrich Labs

IInput implementation for Linux, using X11
==========================================
*/

#ifndef X11_INPUT_H_
#define X11_INPUT_H_

#include "../game/i_input.h"
#include <csignal>
#include <X11/Xlib.h>

namespace ostrich {

namespace x11 {

/////////////////////////////////////////////////
// Translates X11 virtual keys to internal ostrich format
int32_t TranslateKey(int32_t vkey);

} // namespace x11

/////////////////////////////////////////////////
//
class InputX11 : public IInput {
public:

    InputX11() noexcept : m_isActive(false), m_Display(nullptr) { }
    virtual ~InputX11() { }
    InputX11(InputX11 &&) = delete;
    InputX11(const InputX11 &) = delete;
    InputX11 &operator=(InputX11 &&) = delete;
    InputX11 &operator=(const InputX11 &) = delete;

    bool isActive() const noexcept override { return m_isActive; }

    int Initialize(ConsolePrinter consoleprinter, EventSender eventsender) override;
    void Destroy() override;

    void ProcessKBM() override;
    void ProcessOSMessages() override;

private:

    const char * const m_Classname = u8"ostrich::InputX11";

    static volatile int ms_LastRaisedSignal;
    static siginfo_t *ms_SignalInfo;
    static int InitializeSignalHandler();
    static void SignalHandler(int signum, siginfo_t *info, void *ucontext);

    ConsolePrinter m_ConsolePrinter;
    EventSender m_EventSender;

    bool m_isActive;

    Display *m_Display;
};

} // namespace ostrich

#endif /* X11_INPUT_H_ */
