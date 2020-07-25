/*
==========================================
Copyright (c) 2020 Ostrich Labs

Standard file system functions, Windows version
==========================================
*/

#include "../ost_common.h"

#if (OST_WINDOWS != 1)
#    error "This module should only be included in Windows builds"
#endif

#include <Windows.h>
#include "../filesystem.h"
#include "../utility.h"

namespace {

/////////////////////////////////////////////////
//
const wchar_t *FILEModesW[] = {
    L"rb",  // OPEN_READONLY
    L"wb",  // OPEN_WRITEONLY
    L"wb+", // OPEN_WRITETRUNC
    L"ab+"  // OPEN_WRITEAPPEND
};

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool OpenWide(const std::string_view filename, const wchar_t *mode, FILE **filehandle) {
    std::wstring widename;
    ostrich::UTF8toUTF16(std::string(filename.data()), widename);
    return (::_wfopen_s(filehandle, widename.c_str(), mode) == 0);
}

} // anonymous namespace

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::OpenFile(const std::string_view filename, ostrich::FileMode mode, FILE **filehandle) {
    if (*filehandle != nullptr)
        ::fclose(*filehandle);
    return ::OpenWide(filename, ::FILEModesW[static_cast<int32_t>(mode)], filehandle);
}