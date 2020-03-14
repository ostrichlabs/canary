/*
==========================================
filesystem.h

Copyright (c) 2020 Ostrich Labs

Standard file system functions.
==========================================
*/

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <fstream>
#include <string_view>
#include "ost_common.h"

namespace ostrich {

namespace filemode {

/////////////////////////////////////////////////
// Constants for file mode
// Use these instead of defining the masks at the call site
constexpr std::ios_base::openmode g_READONLY = std::ios::in | std::ios::binary;
constexpr std::ios_base::openmode g_WRITEONLY = std::ios::out | std::ios::binary;
constexpr std::ios_base::openmode g_WRITETRUNCATE = std::ios::out | std::ios::trunc | std::ios::binary;
constexpr std::ios_base::openmode g_WRITEAPPEND = std::ios::out | std::ios::app | std::ios::binary;

} // namespace filemode

// Wrapper function for opening a file that performs any Unicode conversions
// necessary for the underlying platform
void OpenFile(const std::string_view filename, std::ios_base::openmode mode, std::fstream &filehandle);

} // namespace ostrich

namespace ost_filemode = ostrich::filemode;

#endif /* FILESYSTEM_H_ */
