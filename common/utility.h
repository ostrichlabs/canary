/*
==========================================
Copyright (c) 2020 Ostrich Labs

Utility functions that don't quite belong anywhere else
==========================================
*/

#ifndef OSTRICH_UTILITY_H_
#define OSTRICH_UTILITY_H_

#include <cstddef>
#include <string>
#include <string_view>

namespace ostrich {

namespace utility {

/////////////////////////////////////////////////
// Hashing
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Generate a hash value for a given string
// Using this as a wrapper in case I choose to change the implementation
// Note: The exact length of size_t is implementation defined,
//  but should be large enough for our purposes even on 32-bit platforms
//
// in:
//      target - a view into a UTF-8 encoded C++ string
// returns:
//      a generated hash value for the contents of target
std::size_t HashString(std::string_view target);

/////////////////////////////////////////////////
// UTF Conversion Functions
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// convert a UTF-8 string to UTF-16
//
// in:
//      source - a UTF-8 encoded C++ string
// out:
//      destination - a UTF-16 encoded C++ string; original contents are destroyed
// returns:
//      void
void UTF8toUTF16(const std::string &source, std::wstring &destination);

/////////////////////////////////////////////////
// convert a UTF-16 string to UTF-8
//
// in:
//      source - a UTF-16 encoded C++ string
// out:
//      destination - a UTF-8 encoded C++ string; original contents are destroyed
// returns:
//      void
void UTF16toUTF8(const std::wstring &source, std::string &destination);

} // namespace utility

} // namespace ostrich

#endif /* OSTRICH_UTILITY_H_ */
