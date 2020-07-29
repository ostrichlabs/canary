/*
==========================================
Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "filesystem.h"

#if (OST_WINDOWS == 1)
#   include <Windows.h>
#endif

namespace {

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