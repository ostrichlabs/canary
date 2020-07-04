/*
==========================================
Copyright (c) 2020 Ostrich Labs

Interface for retrieving input information via udev and evdev
==========================================
*/

#include "linux_input.h"
#include <cerrno>
#include <cstring>
#include <string>
//#include <fcntl.h>
#include <unistd.h>
//#include <linux/kd.h>
//#include <linux/keyboard.h>
#include "../common/error.h"
#include "../game/keydef.h"
#include "../game/message.h"


/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::InputLinux::Initialize(ConsolePrinter consoleprinter, EventSender eventsender) {
    if (this->isActive()) {
        return -1;
    }

    m_ConsolePrinter = consoleprinter;
    m_EventSender = eventsender;
    if ((!m_ConsolePrinter.isValid()) || (!m_EventSender.isValid()))
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    if (!this->InitUDev())
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 1);

    m_ConsolePrinter.DebugMessage(u8"Added % devices", { std::to_string(m_Devices.size()) });

    m_isActive = true;
    return 0;
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
                        int32_t key = this->TranslateKey(input[i].code);
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
// for base Linux this will be signal handling, so, nothing to do here for now
/////////////////////////////////////////////////
void ostrich::InputLinux::ProcessOSMessages() {

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::InputLinux::InitUDev() {
    m_udev = ::udev_new();
    if (!m_udev) {
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 2);
    }

    m_Monitor = ::udev_monitor_new_from_netlink(m_udev, "udev");
    if (!m_Monitor) {
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 3);
    }

    ::udev_monitor_filter_add_match_subsystem_devtype(m_Monitor, "input", NULL);
    ::udev_monitor_enable_receiving(m_Monitor);

    this->ScanDevices();
    if (m_Devices.size() == 0) {
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 4);
    }

    return true;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputLinux::ScanDevices() {
    if (m_udev) {
        udev_enumerate *enumerate = nullptr;

        enumerate = ::udev_enumerate_new(m_udev);
        if (!enumerate) {
            throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 5);
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
int32_t ostrich::InputLinux::TranslateKey(__u16 vkey) {

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
