/*
==========================================
Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "ost_common.h"

#if (OST_WINDOWS == 1) // required for a check in File::Open()
#   include <Windows.h>
#endif

#include "filesystem.h"

namespace {

/////////////////////////////////////////////////
// translations of FileMode to fstream modes
// needs to be manually maintained (for now maybe I dunno)
static constexpr std::ios_base::openmode FStreamModes[] = {
    std::ios::in | std::ios::binary,                    // OPEN_READONLY
    std::ios::out | std::ios::binary,                   // OPEN_WRITEONLY
    std::ios::out | std::ios::trunc | std::ios::binary, // OPEN_WRITETRUNC
    std::ios::out | std::ios::app | std::ios::binary    // OPEN_WRITEAPPEND
};

} // anonymous namespace

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::File::Open(const std::string_view filename, ostrich::FileMode mode) {
    m_Path = std::filesystem::u8path(filename);

    if (m_Handle.is_open()) {
        m_Handle.close();
    }
    m_Handle.clear();

    m_Handle.open(m_Path, ::FStreamModes[static_cast<int32_t>(mode)]);

#if (OST_WINDOWS == 1)
    // an obnoxious setting of the global error state in Windows that I do not care about
    if (::GetLastError() == 0xb7) // ERROR_ALREADY_EXISTS
        ::SetLastError(0);
#endif

    return ((m_Handle.is_open()) && (!m_Handle.fail()));
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::File::Close() {
    if (m_Handle.is_open()) {
        m_Handle.close();
    }
    m_Path.clear();
}