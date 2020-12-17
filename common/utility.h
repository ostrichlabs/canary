/*
==========================================
Copyright (c) 2020 Ostrich Labs

Utility functions that don't quite belong anywhere else

TODO: Needs to be unit tested
==========================================
*/

#ifndef OSTRICH_UTILITY_H_
#define OSTRICH_UTILITY_H_

#include <string>

namespace ostrich {

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

} // namespace ostrich

#endif /* OSTRICH_UTILITY_H_ */
