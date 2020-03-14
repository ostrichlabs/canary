/*
==========================================
utility.h

Copyright (c) 2020 Ostrich Labs

Utility functions that don't quite belong anywhere else

TODO: Spin off UTF-8 and UTF-16 functions to a Windows module after the printf stuff is figured out
==========================================
*/

#ifndef UTILITY_H_
#define UTILITY_H_

#include <list>
#include <string>

namespace ostrich {

/////////////////////////////////////////////////
// UTF Conversion Functions
/////////////////////////////////////////////////

// convert a UTF-8 string to UTF-16
//
// uses standard functionality that is deprecated in C++17
void UTF8toUTF16(const std::string &source, std::wstring &destination);

// convert a UTF-16 string to UTF-8
//
// uses standard functionality that is deprecated in C++17
void UTF16toUTF8(const std::wstring &source, std::string &destination);

/////////////////////////////////////////////////
// Helper BuildString functions
//
// None of these are intended to be called on their own, but required for BuildString() to work
/////////////////////////////////////////////////

// Replaces instances of % with strings from arglist
std::string ReplaceTokens(const std::string &source, std::list<std::string> &arglist);

// converts arg to a string and pushes into list
// for use in ReplaceTokens
template <typename Tvalue>
void ArgsToStrings(std::list<std::string> &targetlist, const Tvalue arg) {
    targetlist.push_back(std::to_string(arg));
}

// specialization for std::string to avoid compiler complaints
void ArgsToStrings(std::list<std::string> &targetlist, const std::string &arg);

// specialization for const char* to avoid ambiguity
void ArgsToStrings(std::list<std::string> &targetlist, const char *arg);

// convert a parameter pack to a list of strings
template <typename Tvalue, typename... Targs>
void ArgsToStrings(std::list<std::string> &targetlist, const Tvalue arg, const Targs... arglist) {
    targetlist.push_back(std::to_string(arg));
    ArgsToStrings(targetlist, arglist...);
}

// specializations to avoid ambiguity
template <typename... Targs>
void ArgsToStrings(std::list<std::string> &targetlist, const std::string &arg, const Targs... arglist) {
    targetlist.push_back(arg);
    ArgsToStrings(targetlist, arglist...);
}

template <typename... Targs>
void ArgsToStrings(std::list<std::string>& targetlist, std::string_view arg, const Targs... arglist) {
    targetlist.emplace_back(arg);
    ArgsToStrings(targetlist, arglist...);
}

template <typename... Targs>
void ArgsToStrings(std::list<std::string> &targetlist, const char *arg, const Targs... arglist) {
    targetlist.emplace_back(arg);
    ArgsToStrings(targetlist, arglist...);
}

/////////////////////////////////////////////////
// BuildString()
//
// create a string with variables in place of % arguments
//
// USE THIS FOR ACTUAL WORK
/////////////////////////////////////////////////
template <typename... Targs>
std::string BuildString(const std::string &source, Targs... args) {
    std::list<std::string> arglist;
    ostrich::ArgsToStrings(arglist, args...);
    return ostrich::ReplaceTokens(source, arglist);
}

} // namespace ostrich

#endif /* UTILITY_H_ */
