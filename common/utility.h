/*
==========================================
Copyright (c) 2020 Ostrich Labs

Utility functions that don't quite belong anywhere else
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

} // namespace ostrich

#endif /* UTILITY_H_ */
