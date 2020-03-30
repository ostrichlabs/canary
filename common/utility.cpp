/*
==========================================
Copyright (c) 2020 Ostrich Labs

Utility functions that don't quite belong anywhere else
==========================================
*/

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#if (OST_WINDOWS == 1)
#   pragma warning(disable : 4996) // yes I know codecvt is deprecated, gimme something standard to use and I'll switch...
#endif

#include "ost_common.h"
#include "utility.h"

#include <codecvt>
#include <locale>

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::UTF8toUTF16(const std::string &source, std::wstring &destination) {
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    destination = converter.from_bytes(source);
}


/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::UTF16toUTF8(const std::wstring &source, std::string &destination) {
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    destination = converter.to_bytes(source);
}
