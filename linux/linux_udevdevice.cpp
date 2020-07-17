/*
==========================================
Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "../common/ost_common.h"

#if (OST_RASPI != 1)
#    error "This module should only be included in Raspberry Pi (or Linux I guess) builds"
#endif

#include "linux_udevdevice.h"
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::UDevDevice::Initialize(udev_device *device, const char *path) {
    if ((m_Path != nullptr) || (m_FileHandle != -1) || (device == nullptr) || (path == nullptr)) {
        return false;
    }

    if (!this->Identify(device)) {
        this->Destroy();
        return false;
    }

    if (!this->OpenFile(path)) {
        this->Destroy();
        return false;
    }

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
bool ostrich::UDevDevice::OpenFile(const char *path) {
    int handle = ::open(path, O_RDONLY | O_NONBLOCK);
    if (handle != -1) {
        m_Path = path;
        m_FileHandle = handle;
        return true;
    }

    return false;
}
