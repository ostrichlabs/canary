/*
==========================================
Copyright (c) 2020 Ostrich Labs

A udev device
It's pretty much POD but with some helper methods that aren't super helpful
But since after initialization the device should be read-only it doesn't matter much
==========================================
*/

#ifndef UDEV_DEVICE_H_
#define UDEV_DEVICE_H_

#include <libudev.h>
#include <list>

namespace ostrich {

/////////////////////////////////////////////////
//
class UDevDevice {
public:

    enum class Type : int32_t {
        NONE = 0,
        KEYBOARD,
        MOUSE,
        MAX
    };

    UDevDevice() noexcept : m_Path(nullptr), m_FileHandle(-1), m_Type(Type::NONE) {}
    ~UDevDevice() { }
    UDevDevice(UDevDevice &&) = default;
    UDevDevice(const UDevDevice &) = default;
    UDevDevice &operator=(UDevDevice &&) = default;
    UDevDevice &operator=(const UDevDevice &) = default;

    bool Initialize(udev_device *device, const char *path);
    void Destroy();

    const char *getPath() noexcept { return m_Path; }
    int getFileHandle() noexcept { return m_FileHandle; }
    Type getType() noexcept { return m_Type; }
    int32_t getTypeAsInt() noexcept { return static_cast<int32_t>(m_Type); }

private:

    bool Identify(udev_device *device);
    bool OpenFile(const char *path);

    const char *m_Path;
    int m_FileHandle;
    Type m_Type;
};

}

#endif /* LINUX_UDEVDEVICE_H_ */
