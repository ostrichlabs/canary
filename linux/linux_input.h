/*
==========================================
Copyright (c) 2020 Ostrich Labs

Interface for retrieving input information via udev
Meant for use on Linux platforms not using X11 for some reason (like the raspi)
==========================================
*/

#ifndef INPUT_RASPI_H_
#define INPUT_RASPI_H_

#include "../common/ost_common.h"

#if (OST_RASPI != 1)
#    error "This module should only be included in Raspberry Pi builds"
#endif

#include <libudev.h>
#include <list>
#include "../game/eventqueue.h"
#include "../game/i_input.h"
#include "../game/keydef.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class InputRaspi : public IInput {
public:

    InputRaspi() noexcept : m_isActive(false), m_udev(nullptr), m_Monitor(nullptr) {}
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
    bool InitUDev();
    void ScanDevices();
    void AddDevice(udev_device *device);
    void ReleaseDevices();

    int32_t TranslateKey(int32_t vkey);

    ConsolePrinter m_ConsolePrinter;
    EventSender m_EventSender;

    bool m_isActive;

    udev *m_udev;
    udev_monitor *m_Monitor;
    std::list<udev_device *> m_Devices;
};

} // namespace ostrich

#endif /* INPUT_RASPI_H_ */
