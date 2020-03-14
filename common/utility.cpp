/*
==========================================
sys_utility.cpp

Copyright (c) 2019 Ostrich Labs

Utility functions that don't quite belong anywhere else
==========================================
*/

#include "ost_common.h"
#include "sys_utility.h"

#if (OST_WINDOWS == 1)
#   pragma warning(disable : 4996) // yes I know codecvt is deprecated, gimme something standard to use and I'll switch...
#endif

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

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::string ostrich::ReplaceTokens(const std::string &source, std::list<std::string> &arglist) {
    std::string buffer;
    auto argitr = arglist.cbegin();
    auto msgitr = source.cbegin();
    while (msgitr != source.end()) {
        if ((*msgitr) == '%') {
            if (std::next(msgitr) != source.end() && ((*std::next(msgitr)) == '%')) {
                buffer.push_back('%');
                std::advance(msgitr, 1);
            }
            else if (argitr == arglist.end()) { // if more % than args, append %
                buffer.push_back('%');
            }
            else {
                buffer.append((*argitr));
                std::advance(argitr, 1);
            }
        }
        else {
            buffer.push_back((*msgitr));
        }
        if (msgitr != source.end())
            std::advance(msgitr, 1);
    }
    return buffer;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::ArgsToStrings(std::list<std::string> &targetlist, const std::string &arg) {
    targetlist.push_back(arg);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::ArgsToStrings(std::list<std::string> &targetlist, const char *arg) {
    targetlist.push_back(arg);
}
