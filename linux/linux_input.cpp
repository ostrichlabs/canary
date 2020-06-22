/*
==========================================
Copyright (c) 2020 Ostrich Labs

Interface for retrieving input information via udev and evdev
==========================================
*/

#include "linux_input.h"
#include <cerrno>
#include <string>
#include <fcntl.h>
#include <unistd.h>
//#include <linux/kd.h>
//#include <linux/keyboard.h>
#include <linux/input.h>
#include <libudev.h>
#include "../common/error.h"
#include "../game/message.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::UDevDevice::Initialize(udev_device *device, const char *path) {
    if ((m_Path != nullptr) || (m_FileHandle != -1) || (device == nullptr) || (path == nullptr)) {
        return false;
    }

    if (!this->Identify(device)) {
        return false;
    }

    if (!this->OpenFile()) {
        return false;
    }

    // after all that, we're certain the path is good so keep it
    m_Path = path;

    return true;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::UDevDevice::Destroy() {
    if (m_FileHandle != -1) {
        ::close(m_FileHandle);
    }
    m_Path = nullptr;
    m_Type = ostrich::UDevDevice::Type::NONE;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::UDevDevice::Identify(udev_device *device) {
    const char *subsystem = ::udev_device_get_subsystem(device);
    if (::strcmp(subsystem, u8"input") != 0) {
        return false;
    }

    bool identified = false;
    const char *property = nullptr;
    property = ::udev_device_get_property_value(device, u8"ID_INPUT_MOUSE");
    if ((property != nullptr) && (property[0] == '1')) {
        identified = true;
        m_Type = ostrich::UDevDevice::Type::MOUSE;
    }

    if (!identified) {
        property = ::udev_device_get_property_value(device, u8"ID_INPUT_KEY");
        if ((property != nullptr) && (property[0] == '1')) {
            identified = true;
            m_Type = ostrich::UDevDevice::Type::KEYBOARD;
        }
    }

    // fallback in case none of that worked
    if (!identified) {
        property = ::udev_device_get_property_value(device, u8"ID_CLASS");
        if (property != nullptr) {
            if (::strcmp(property, u8"mouse") == 0) {
                identified = true;
                m_Type = ostrich::UDevDevice::Type::MOUSE;
            }
            else if (::strcmp(property, u8"kbd") == 0) {
                identified = true;
                m_Type = ostrich::UDevDevice::Type::KEYBOARD;
            }
        }
    }

    return identified;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::UDevDevice::OpenFile() {
    int handle = ::open(m_Path, O_RDONLY | O_NONBLOCK);
    if (handle != -1) {
        m_FileHandle = handle;
        return false;
    }

    return true;
}

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

    m_ConsolePrinter.DebugMessage(u8"Added % devices", { std::to_string(m_Devices.size()) });

    m_isActive = true;
    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputLinux::Destroy() {
    if (m_isActive) {
        m_Devices.clear();
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
    m_ConsolePrinter.DebugMessage(u8"In InputLinux::ProcessKBM()");
    for (auto itr = m_Devices.begin(); itr != m_Devices.end(); std::advance(itr, 1)) {
        done = false;
        while (!done) {
            bytesread = ::read((*itr).getFileHandle(), input, sizeof(input));
            if (bytesread <= 0) {
                done = true;
                continue;
            }
            int count = bytesread / sizeof(input[0]);
            m_ConsolePrinter.DebugMessage(u8"Read % bytes, making % events",
                    { std::to_string(bytesread), std::to_string(count)});
            for (int i = 0; i < count; i++) {
                if (input[i].type == EV_KEY) { // keyboard or mouse buttons
                    m_EventSender.Send(ostrich::Message::CreateSystemMessage(1, m_Classname));
                }
                else if (input[i].type == EV_REL) { // mouse moved

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
int32_t ostrich::InputLinux::TranslateKey(int32_t vkey) {
    return 0;
}
