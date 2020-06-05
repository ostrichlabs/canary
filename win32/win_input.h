/*
==========================================
Copyright (c) 2020 Ostrich Labs

Interface for retrieving input information from Windows
==========================================
*/

#ifndef WIN_INPUT_H_
#define WIN_INPUT_H_

#include "../common/ost_common.h"

#if (OST_WINDOWS != 1)
#   error "This module should only be included in Windows builds"
#endif

#include "../game/eventqueue.h"
#include "../game/i_input.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class InputWindows : public IInput {
public:

    InputWindows() noexcept : m_isActive(false) {}
    virtual ~InputWindows() { m_isActive = false; }
    InputWindows(InputWindows &&) = delete;
    InputWindows(const InputWindows &) = delete;
    InputWindows &operator=(InputWindows &&) = delete;
    InputWindows &operator=(const InputWindows &) = delete;

    bool isActive() const noexcept override { return m_isActive; }

    int Initialize(ConsolePrinter consoleprinter, EventSender eventsender) override;
    void Destroy() override;

    void ProcessKBM() override;
    void ProcessOSMessages() override;

private:

    const char * const m_Classname = u8"ostrich::InputWindows";

    ConsolePrinter m_ConsolePrinter;
    EventSender m_EventSender;

    bool m_isActive;
};

} // namespace ostrich

#endif /* WIN_INPUT_H */
