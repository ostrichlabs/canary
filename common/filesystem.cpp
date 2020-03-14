/*
==========================================
filesystem.cpp

Copyright (c) 2020 Ostrich Labs

Standard file system functions.
==========================================
*/

#include "filesystem.h"

#if (OST_WINDOWS == 1)

#include "utility.h"

namespace {

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void OpenWide(const std::string_view filename, std::ios_base::openmode mode, std::fstream &filehandle) {
    std::wstring widename;
    ostrich::UTF8toUTF16(std::string(filename.data()), widename);
    filehandle.open(widename, mode);
}

} // anonymous namespace

#endif

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


