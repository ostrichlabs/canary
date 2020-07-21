/*
==========================================
Copyright (c) 2020 Ostrich Labs

Standard file system functions.
==========================================
*/

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

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
// these definitions, as well as the char[] versions in the platform modules, should match the FileMode enum

/////////////////////////////////////////////////
//
constexpr std::ios_base::openmode FStreamModes[] = {
    std::ios::in | std::ios::binary,                    // OPEN_READONLY
    std::ios::out | std::ios::binary,                   // OPEN_WRITEONLY
    std::ios::out | std::ios::trunc | std::ios::binary, // OPEN_WRITETRUNC
    std::ios::out | std::ios::app | std::ios::binary    // OPEN_WRITEAPPEND
};

/////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////

// Wrapper function for opening a file that performs any necessary Unicode conversions
void OpenFile(const std::string_view filename, FileMode mode, std::fstream &filehandle);

// Wrapper function for opening a file that performs any necessary Unicode conversions
bool OpenFile(const std::string_view filename, FileMode mode, FILE **filehandle);

} // namespace ostrich

#endif /* FILESYSTEM_H_ */
