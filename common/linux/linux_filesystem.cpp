/*
==========================================
Copyright (c) 2020 Ostrich Labs

Standard file system functions, Linux version
==========================================
*/

#include "../ost_common.h"

#if (OST_RASPI != 1 && OST_LINUX != 1)
#    error "This module should only be included in Raspberry Pi or Linux builds"
#endif

#include "../filesystem.h"

namespace {

/////////////////////////////////////////////////
//
const char *FILEModes[] = {
    "rb",   // OPEN_READONLY
    "wb",   // OPEN_WRITEONLY
    "wb+",  // OPEN_WRITETRUNC
    "ab+"   // OPEN_WRITEAPPEND
};

} // anonymous namespace

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::OpenFile(const std::string_view filename, ostrich::FileMode mode, FILE **filehandle) {
    if (filehandle && *filehandle)
        ::fclose(*filehandle);
    *filehandle = ::fopen(filename.data(), ::FILEModes[static_cast<int32_t>(mode)]);
    return (*filehandle != nullptr);
}
