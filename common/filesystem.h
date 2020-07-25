/*
==========================================
Copyright (c) 2020 Ostrich Labs

Standard file system functions.
==========================================
*/

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <filesystem>
#include <fstream>
#include <cstdio>
#include <string_view>
#include "ost_common.h"

namespace ostrich {

/////////////////////////////////////////////////
// Constants for file mode
// Use these instead of the more implementation-specific stuff below
enum class FileMode : int32_t {
    OPEN_READONLY = 0,
    OPEN_WRITEONLY,
    OPEN_WRITETRUNCATE,
    OPEN_WRITEAPPEND,
    OPEN_MAXMODE
};

/////////////////////////////////////////////////
// Wrapper function for opening a file using C-style FILE handles
// Largely untested for now, because it's only meant for use with the PNG loader
bool OpenFile(const std::string_view filename, FileMode mode, FILE **filehandle);

/////////////////////////////////////////////////
// Collection of file objects
// Isn't really intended to be a wrapper, just a convenient pass-around
// If you want to do fileops, get the fstream reference.
// If you want file information, get the path reference.
// If you want neither, why are you using this object?
class File {
public:

    File() noexcept { }
    virtual ~File() { }
    File(File &&) = delete;
    File(const File &) = delete;
    File &operator=(File &&) = delete;
    File &operator=(const File &) = delete;

    bool Open(const std::string_view filename, ostrich::FileMode mode);
    void Close();

    bool isOpen() const noexcept { return m_Handle.is_open(); }

    std::fstream &getFStream() noexcept { return m_Handle; }
    const std::filesystem::path &getPath() const noexcept { return m_Path; }

private:

    std::fstream m_Handle;
    std::filesystem::path m_Path;
};

} // namespace ostrich

#endif /* FILESYSTEM_H_ */
