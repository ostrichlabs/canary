/*
==========================================
Copyright (c) 2020-2021 Ostrich Labs

Utility functions that don't quite belong anywhere else
==========================================
*/

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "ost_common.h"

#if (OST_WINDOWS == 1)
#   pragma warning(disable : 4996) // yes I know codecvt is deprecated, gimme something standard to use and I'll switch...
#endif

#include "utility.h"

#include <codecvt>
#include <functional>
#include <locale>

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::size_t ostrich::utility::HashString(std::string_view target) {
    return std::hash<std::string_view>{}(target);
}

/////////////////////////////////////////////////
// uses standard functionality that is deprecated in C++17
// give me better standard library support and I will use it
/////////////////////////////////////////////////
void ostrich::utility::UTF8toUTF16(const std::string &source, std::wstring &destination) {
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    destination = converter.from_bytes(source);
}


/////////////////////////////////////////////////
// uses standard functionality that is deprecated in C++17
// give me better standard library support and I will use it
/////////////////////////////////////////////////
void ostrich::utility::UTF16toUTF8(const std::wstring &source, std::string &destination) {
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    destination = converter.to_bytes(source);
}
