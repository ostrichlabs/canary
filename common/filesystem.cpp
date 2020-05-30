/*
==========================================
Copyright (c) 2020 Ostrich Labs

Standard file system functions.
==========================================
*/

#include "filesystem.h"

#if (OST_WINDOWS == 1)

#include <Windows.h>
#include "utility.h"

namespace {

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void OpenWide(const std::string_view filename, std::ios_base::openmode mode, std::fstream &filehandle) {
    std::wstring widename;
    ostrich::UTF8toUTF16(std::string(filename.data()), widename);
    filehandle.open(widename, mode);
    if (::GetLastError() == 0xb7) // ERROR_ALREADY_EXISTS
        ::SetLastError(0);
}

} // anonymous namespace

#endif // OST_WINDOWS

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::OpenFile(const std::string_view filename, std::ios_base::openmode mode, std::fstream &filehandle) {
    filehandle.close();

#if (OST_WINDOWS == 1)
    OpenWide(filename, mode, filehandle);
#else
    filehandle.open(filename.data(), mode);
#endif
}


