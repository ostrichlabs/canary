/*
==========================================
Copyright (c) 2020 Ostrich Labs

Interface for retrieving input information via udev
==========================================
*/

#include "linux_input.h"
#include <cerrno>
#include <cstring>
#include <string>
//#include <fcntl.h>
//#include <unistd.h>
//#include <linux/kd.h>
//#include <linux/keyboard.h>
//#include <linux/input.h>
#include <libudev.h>
#include "../common/error.h"
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
        return -2;

    m_isActive = true;
    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputLinux::Destroy() {
    if (m_isActive) {
        this->ReleaseDevices();
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
    m_EventSender.Send(ostrich::Message::CreateSystemMessage(1, m_Classname));
    /*
    input_event kbinput = { }, mouseinput = { };
    ssize_t kbbytes = 0;
    ssize_t mousebytes = 0;
    bool done = false;
    m_ConsolePrinter.DebugMessage(u8"In InputRaspi::ProcessKBM()");
    while (!done) {
        kbbytes = ::read(m_KeyboardFD, &kbinput, sizeof(kbinput));
        m_ConsolePrinter.DebugMessage(u8"Read % bytes from /dev/tty",
            { std::to_string(kbbytes) });
        if (kbbytes == sizeof(kbinput)) {
            m_ConsolePrinter.DebugMessage(u8"kbinput: % type, % code, % value",
                { std::to_string(kbinput.type), std::to_string(kbinput.code), std::to_string(kbinput.value) });
            m_EventSender.Send(ostrich::Message::CreateSystemMessage(1, m_Classname));
            if (kbinput.type & EV_KEY) {
                m_ConsolePrinter.DebugMessage(u8"From InputRaspi::ProcessKBM(): %",
                        { std::to_string(kbinput.code) } );

                // TODO: translate to internal message
                // tho for now if a key is pressed send a quit
                m_EventSender.Send(ostrich::Message::CreateSystemMessage(1, m_Classname));
                done = true;
            }
        }
        if (m_MouseFD > 0) {
            mousebytes = ::read(m_MouseFD, &mouseinput, sizeof(mouseinput));
            if (mousebytes == sizeof(mouseinput)) {
            // TODO: translate to internal message
            }
        }
        if ((kbbytes <= 0) && (mousebytes <= 0))
            done = true;
    }*/
}

/////////////////////////////////////////////////
// for Raspi this will be signal handling, so, nothing to do here for now
/////////////////////////////////////////////////
void ostrich::InputLinux::ProcessOSMessages() {

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::InputLinux::InitUDev() {
    m_udev = ::udev_new();
    if (!m_udev) {
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 1);
    }

    m_Monitor = ::udev_monitor_new_from_netlink(m_udev, "udev");
    if (!m_Monitor) {
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 3);
    }

    ::udev_monitor_filter_add_match_subsystem_devtype(m_Monitor, "input", NULL);
    ::udev_monitor_enable_receiving(m_Monitor);

    this->ScanDevices();
    if (m_Devices.size() == 0) {
        return false;
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
            throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 2);
        }

        ::udev_enumerate_add_match_subsystem(enumerate, "input");
        ::udev_enumerate_scan_devices(enumerate);

        udev_list_entry *entrylist = ::udev_enumerate_get_list_entry(enumerate);
        udev_list_entry *entry = nullptr;
        m_ConsolePrinter.DebugMessage(u8"Enumerating devices");
        for (entry = entrylist;
             entry != nullptr;
             entry = ::udev_list_entry_get_next(entry)) {
            const char *path = ::udev_list_entry_get_name(entry);
            udev_device *device = ::udev_device_new_from_syspath(m_udev, path);
            if (device != nullptr) {
                this->AddDevice(device);
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

    const char *subsystem = ::udev_device_get_subsystem(device);
    if (::strcmp(subsystem, u8"input") != 0) {
        return;
    }

    bool identified = false;
    const char *property = nullptr;
    property = ::udev_device_get_property_value(device, u8"ID_INPUT_MOUSE");
    if ((property != nullptr) && (property[0] == '1')) {
        identified = true;
        m_Devices.push_back(device);
        m_ConsolePrinter.DebugMessage(u8"Detected mouse!");
    }

    if (!identified) {
        property = ::udev_device_get_property_value(device, u8"ID_INPUT_KEY");
        if ((property != nullptr) && (property[0] == '1')) {
            identified = true;
            m_Devices.push_back(device);
            m_ConsolePrinter.DebugMessage(u8"Detected keyboard!");
        }
    }

    // fallback in case none of that worked
    if (!identified) {
        m_ConsolePrinter.DebugMessage(u8"Device unidentified, trying fallback");
        property = ::udev_device_get_property_value(device, u8"ID_CLASS");
        if (property != nullptr) {
            if (::strcmp(property, u8"mouse") == 0) {
                m_Devices.push_back(device);
                m_ConsolePrinter.DebugMessage(u8"Detected mouse!");
            }
            else if (::strcmp(property, u8"kbd") == 0) {
                m_Devices.push_back(device);
                m_ConsolePrinter.DebugMessage(u8"Detected keyboard!");
            }
            else {
                m_ConsolePrinter.DebugMessage(u8"Device is (likely) neither keyboard or mouse - ignoring");
            }
        }
        else {
            m_ConsolePrinter.DebugMessage(u8"Device has no class?");
        }
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputLinux::ReleaseDevices() {
    for (auto itr = m_Devices.begin(); itr != m_Devices.end(); std::advance(itr, 1)) {
        ::udev_device_unref((*itr));
    }
    m_Devices.clear();
}


/////////////////////////////////////////////////
/////////////////////////////////////////////////
int32_t ostrich::InputLinux::TranslateKey(int32_t vkey) {
    return 0;
}
