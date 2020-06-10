/*
==========================================
Copyright (c) 2020 Ostrich Labs

Interface for retrieving input information from the Raspberry Pi
==========================================
*/

#ifndef INPUT_RASPI_H_
#define INPUT_RASPI_H_

#include "../common/ost_common.h"

#if (OST_RASPI != 1)
#    error "This module should only be included in Raspberry Pi builds"
#endif

#include "../game/eventqueue.h"
#include "../game/i_input.h"
#include "../game/keydef.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class InputRaspi : public IInput {
public:

    InputRaspi() noexcept : m_isActive(false), m_KeyboardFD(0), m_MouseFD(0), m_OldKBMode(0) {}
    virtual ~InputRaspi() { }
    InputRaspi(InputRaspi &&) = delete;
    InputRaspi(const InputRaspi &) = delete;
    InputRaspi &operator=(InputRaspi &&) = delete;
    InputRaspi &operator=(const InputRaspi &) = delete;

    bool isActive() const noexcept override { return m_isActive; }

    int Initialize(ConsolePrinter consoleprinter, EventSender eventsender) override;
    void Destroy() override;

    void ProcessKBM() override;
    void ProcessOSMessages() override;

private:

    const char * const m_Classname = u8"ostrich::InputRaspi";

    // helper methods for initialization
    bool InitKeyboard();
    bool InitMouse();

    int32_t TranslateKey(int32_t vkey);

    ConsolePrinter m_ConsolePrinter;
    EventSender m_EventSender;

    bool m_isActive;
    int m_KeyboardFD;
    int m_MouseFD;

    long m_OldKBMode;
};

} // namespace ostrich

#endif /* INPUT_RASPI_H_ */
