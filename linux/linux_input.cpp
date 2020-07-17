/*
==========================================
Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "../common/ost_common.h"

#if (OST_RASPI != 1)
#    error "This module should only be included in Raspberry Pi (or Linux I guess) builds"
#endif

#include "linux_input.h"
#include <cerrno>
#include <cstring>
#include <string>
//#include <fcntl.h>
#include <unistd.h>
//#include <linux/kd.h>
//#include <linux/keyboard.h>
#include "../common/error.h"
#include "../game/errorcodes.h"
#include "../game/keydef.h"
#include "../game/message.h"

volatile int ostrich::InputLinux::ms_LastRaisedSignal = 0;
::siginfo_t *ostrich::InputLinux::ms_SignalInfo = nullptr;

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int32_t ostrich::linux::TranslateKey(__u16 vkey) {
    // Using a table for most of these because the Linux event codes are all over the goddamn place
    static int32_t keytable[] =
    { 0, ostrich::KeyToInt32(ostrich::Keys::OSTKEY_ESCAPE),
      '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_BACKSPACE), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_TAB),
      'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_ENTER), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_CTRL),
      'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_SHIFT), '\\',
      'Z', 'X' ,'C' ,'V' ,'B', 'N', 'M', ',', '.', '/',
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_SHIFT), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_STAR),
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_ALT), ' ', ostrich::KeyToInt32(ostrich::Keys::OSTKEY_CAPSLOCK),
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_F1), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_F2),
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_F3), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_F4),
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_F5), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_F6),
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_F7), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_F8),
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_F9), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_F10),
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_NUMLOCK), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_SCROLLOCK),
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_7), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_8),
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_9), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_MINUS),
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_4), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_5),
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_6), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_PLUS),
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_1), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_2),
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_3), ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_0),
      ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_DELETE)
    };

    // table keys, which is most of them
    if (vkey <= KEY_KPDOT) {
        return keytable[vkey];
    }

    // mouse buttons 1-3
    if ((vkey >= BTN_LEFT) && (vkey <= BTN_MIDDLE)) {
        return static_cast<int32_t>((vkey) + (ostrich::KeyToInt32(ostrich::Keys::OSTKEY_MOUSE1) - BTN_LEFT));
    }

    // the wacky remainder
    switch (vkey) {
        case KEY_F11:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_F11);
        }
        case KEY_F12:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_F12);
        }

        case KEY_KPENTER: // treat keypad enter as regular enter
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_ENTER);
        }
        case KEY_RIGHTCTRL:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_CTRL);
        }
        case KEY_KPSLASH:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_SLASH);
        }
        case KEY_RIGHTALT:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_ALT);
        }

        // Arrow keys
        case KEY_UP:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_UPARROW);
        }
        case KEY_LEFT:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_LEFTARROW);
        }
        case KEY_DOWN:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_DOWNARROW);
        }
        case KEY_RIGHT:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_RIGHTARROW);
        }

        // Those utility keys above the arrow keys
        case KEY_PAUSE:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_PAUSE);
        }
        case KEY_INSERT:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_INSERT);
        }
        case KEY_DELETE:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_DELETE);
        }
        case KEY_HOME:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_HOME);
        }
        case KEY_END:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_END);
        }
        case KEY_PAGEUP:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_PAGEUP);
        }
        case KEY_PAGEDOWN:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_PAGEDOWN);
        }

    }

    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::InputLinux::Initialize(ostrich::ConsolePrinter consoleprinter, ostrich::EventSender eventsender) {
    if (this->isActive()) {
        return OST_ERROR_ISACTIVE;
    }

    m_ConsolePrinter = consoleprinter;
    m_EventSender = eventsender;
    if ((!m_ConsolePrinter.isValid()) || (!m_EventSender.isValid()))
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    int result = InputLinux::InitializeSignalHandler();
    if (result != OST_ERROR_OK)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, result);

    result = this->InitUDev();
    if (result != OST_ERROR_OK)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, result);

    m_ConsolePrinter.DebugMessage(u8"Added % devices", { std::to_string(m_Devices.size()) });

    m_isActive = true;
    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputLinux::Destroy() {
    if (m_isActive) {
        this->ClearDevices();
        if (m_Monitor) {
            ::udev_monitor_unref(m_Monitor);
            m_Monitor = nullptr;
        }
        if (m_udev) {
            ::udev_unref(m_udev);
            m_udev = nullptr;
        }
        m_EventSender.AttachParent(nullptr);
        m_ConsolePrinter.AttachParent(nullptr);
        m_isActive = false;
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputLinux::ProcessKBM() {
    input_event input[32];
    ssize_t bytesread = 0;
    bool done = false;
    //m_ConsolePrinter.DebugMessage(u8"In InputLinux::ProcessKBM()");
    for (auto itr = m_Devices.begin(); itr != m_Devices.end(); std::advance(itr, 1)) {
        done = false;
        int filehandle = (*itr).getFileHandle();
        while (!done) {
            bytesread = ::read(filehandle, input, sizeof(input));
            if (bytesread <= 0) {
                done = true;
            }
            else {
                int count = bytesread / sizeof(input[0]);
                m_ConsolePrinter.DebugMessage(u8"Read % bytes, making % events",
                    { std::to_string(bytesread), std::to_string(count)});
                for (int i = 0; i < count; i++) {
                    if (input[i].type == EV_KEY) { // keyboard or mouse buttons
                        m_ConsolePrinter.DebugMessage(u8"EV_KEY: code \"%\" value \"%\"",
                            { std::to_string(input[i].code), std::to_string(input[i].value) });
                        int32_t key = ostrich::linux::TranslateKey(input[i].code);
                        bool keystate = (input[i].value == 1) ? true : false;
                        m_EventSender.Send(ostrich::Message::CreateKeyMessage(key, keystate, OST_FUNCTION_SIGNATURE));
                    }
                    else if (input[i].type == EV_REL) { // mouse moved
                        m_ConsolePrinter.DebugMessage(u8"EV_REL: code \"%\" value \"%\"",
                            { std::to_string(input[i].code), std::to_string(input[i].value) });
                    }
                }
            }
        }
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputLinux::ProcessOSMessages() {
    if (InputLinux::ms_LastRaisedSignal != 0) {
        int32_t code = 0; // TODO: reserved for when system messages get their own defines
        if (InputLinux::ms_SignalInfo) {
            code = InputLinux::ms_SignalInfo->si_code;
        }
        m_EventSender.Send(ostrich::Message::CreateSystemMessage(OST_SYSTEMMSG_SIGNAL,
            InputLinux::ms_LastRaisedSignal, OST_FUNCTION_SIGNATURE));
        InputLinux::ms_LastRaisedSignal = 0;
        InputLinux::ms_SignalInfo = nullptr;
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::InputLinux::InitUDev() {
    m_udev = ::udev_new();
    if (!m_udev) {
        return OST_ERROR_UDEVINIT;
    }

    m_Monitor = ::udev_monitor_new_from_netlink(m_udev, "udev");
    if (!m_Monitor) {
        return OST_ERROR_UDEVMONITOR;
    }

    ::udev_monitor_filter_add_match_subsystem_devtype(m_Monitor, "input", NULL);
    ::udev_monitor_enable_receiving(m_Monitor);

    this->ScanDevices();
    if (m_Devices.size() == 0) {
        return OST_ERROR_UDEVNODEVICES;
    }

    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::InputLinux::ScanDevices() {
    if (m_udev) {
        udev_enumerate *enumerate = nullptr;

        enumerate = ::udev_enumerate_new(m_udev);
        if (!enumerate) {
            return OST_ERROR_UDEVENUM;
        }

        ::udev_enumerate_add_match_subsystem(enumerate, "input");
        ::udev_enumerate_scan_devices(enumerate);

        m_ConsolePrinter.DebugMessage(u8"Enumerating devices");
        udev_list_entry *entrylist = ::udev_enumerate_get_list_entry(enumerate);
        udev_list_entry *entry = nullptr;
        for (entry = entrylist;
             entry != nullptr;
             entry = ::udev_list_entry_get_next(entry)) {
            const char *syspath = ::udev_list_entry_get_name(entry);
            udev_device *device = ::udev_device_new_from_syspath(m_udev, syspath);
            if (device != nullptr) {
                this->AddDevice(device);
                ::udev_device_unref(device);
            }
        }

        ::udev_enumerate_unref(enumerate);
    }

    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputLinux::AddDevice(udev_device *device) {
    const char *path = ::udev_device_get_devnode(device);
    if (path == nullptr) {
        return;
    }

    // detect if we've added this one already (somehow?)
    for (auto itr = m_Devices.begin(); itr != m_Devices.end(); std::advance(itr, 1)) {
        if (::strcmp((*itr).getPath(), path) == 0) {
            return;
        }
    }

    ostrich::UDevDevice newdevice;
    if (newdevice.Initialize(device, path)) {
        m_Devices.push_back(newdevice);
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputLinux::ClearDevices() {
    for (auto itr = m_Devices.begin(); itr != m_Devices.end(); std::advance(itr, 1)) {
        (*itr).Destroy();
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::InputLinux::InitializeSignalHandler() {
    InputLinux::ms_LastRaisedSignal = 0;
    InputLinux::ms_SignalInfo = nullptr;

    struct sigaction action = { }, oldaction = { };
    action.sa_sigaction = ostrich::InputLinux::SignalHandler;
    ::sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;

    ::sigaction(SIGINT, nullptr, &oldaction);
    if (oldaction.sa_handler != SIG_IGN)
        ::sigaction(SIGINT, &action, nullptr);
    else
        return OST_ERROR_HANDLERSIGINT;

    ::sigaction(SIGTERM, nullptr, &oldaction);
    if (oldaction.sa_handler != SIG_IGN)
        ::sigaction(SIGTERM, &action, nullptr);
    else
        return OST_ERROR_HANDLERSIGTERM;

    ::sigaction(SIGHUP, nullptr, &oldaction);
    if (oldaction.sa_handler != SIG_IGN)
        ::sigaction(SIGHUP, &action, nullptr);
    else
        return OST_ERROR_HANDLERSIGHUP;

    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputLinux::SignalHandler(int signum, siginfo_t *info, void *ucontext) {
    InputLinux::ms_LastRaisedSignal = signum;
    InputLinux::ms_SignalInfo = info;
}
