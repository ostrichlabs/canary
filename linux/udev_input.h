/*
==========================================
Copyright (c) 2020 Ostrich Labs

Interface for retrieving input information via udev and evdev
Meant for use on Linux platforms not using X11 or Wayland for some reason (like the raspi)
==========================================
*/

#ifndef UDEV_INPUT_H_
#define UDEV_INPUT_H_

#include <csignal>
#include <libudev.h>
#include <list>
#include <linux/input.h>
#include "udev_device.h"
#include "../game/eventqueue.h"
#include "../game/i_input.h"

namespace ostrich {

namespace linux {

/////////////////////////////////////////////////
// Translates Linux virtual keys to internal ostrich format
int32_t TranslateKey(__u16 vkey);

} // namespace linux

/////////////////////////////////////////////////
//
class InputUDev : public IInput {
public:

    InputUDev() noexcept : m_isActive(false), m_udev(nullptr), m_Monitor(nullptr)
    { }
    virtual ~InputUDev() { }
    InputUDev(InputUDev &&) = delete;
    InputUDev(const InputUDev &) = delete;
    InputUDev &operator=(InputUDev &&) = delete;
    InputUDev &operator=(const InputUDev &) = delete;

    bool isActive() const noexcept override { return m_isActive; }

    int Initialize(ConsolePrinter consoleprinter, EventSender eventsender) override;
    void Destroy() override;

    void ProcessKBM() override;
    void ProcessOSMessages() override;

private:

    const char * const m_Classname = u8"ostrich::InputLinux";

    static volatile int ms_LastRaisedSignal;
    static siginfo_t *ms_SignalInfo;
    static int InitializeSignalHandler();
    static void SignalHandler(int signum, siginfo_t *info, void *ucontext);

    // helper methods for initialization
    int InitUDev();
    int ScanDevices();
    void AddDevice(udev_device *device);
    void ClearDevices();

    ConsolePrinter m_ConsolePrinter;
    EventSender m_EventSender;

    bool m_isActive;

    udev *m_udev;
    udev_monitor *m_Monitor;
    std::list<UDevDevice> m_Devices;
};

} // namespace ostrich

#endif /* LINUX_INPUT_H_ */
