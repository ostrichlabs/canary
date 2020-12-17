/*
==========================================
Copyright (c) 2020 Ostrich Labs

Functions and classes for retrieving the date and time
==========================================
*/

#ifndef OSTRICH_DATETIME_H_
#define OSTRICH_DATETIME_H_

#include <chrono>
#include <string>

namespace ostrich {

namespace timer {

/////////////////////////////////////////////////
// typedefs mapping standard C++ so I can change the type of clock from one place
// usable as long as it's a clock from chrono
typedef std::chrono::steady_clock               clock;
typedef std::chrono::steady_clock::time_point   time_point;

/////////////////////////////////////////////////
// Get a time point for right the heck now
// Usually enough to just call clock::now()
//
// returns:
//      a time_point corresponding to the time of the call
time_point now();

/////////////////////////////////////////////////
// Calculates the interval between two time_points in milliseconds
//
// in:
//      start - the first time_point, expected to be before end
//      end - the second time_point, expect to be after start
// returns:
//      The difference between start and end, as a 32-bit integer
int32_t interval(const time_point &start, const time_point &end);

/////////////////////////////////////////////////
// Calculates the interval between two time_points in milliseconds
//
// in:
//      start - the first time_point, expected to be before end
//      end - the second time_point, expect to be after start
// returns:
//      The difference between start and end, as a double
double interval_d(const time_point &start, const time_point &end);

} // namespace timer

namespace datetime {

/////////////////////////////////////////////////
// Get a simple timestamp of now() as a string
//
// returns:
//      A C++ string with a timestamp of now(), in MM-DD-YYYY HH:MM:SS format.
std::string timestamp();

/////////////////////////////////////////////////
// Format raw timestamp data into a string, specifying -1 for milli if it should be excluded
// Mostly used in the platform-specific datetime modules, since there's no C++ standard way to get ms in timestamps
//
// in:
//      timedata - a structure from <ctime> with raw timestamp data
//      milli - the millisecond count to include in the timestamp, or -1 to exclude ms from the timestamp
// returns:
//      A C++ string with a timestamp of now(), in MM-DD-YYYY HH:MM:SS.mmm format (without .mmm if excluded)
std::string tmtostring(::tm &timedata, int milli = -1);

/////////////////////////////////////////////////
// Platform-Specific Functions
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Calls the safe version of localtime(), which for some dumb reason is different on each platform
// Look to the definitions of localtime() for implemented platforms for more about their usage
//
// Whatever the case, this call should adapt to any version of localtime() required
// Any platform that doesn't need a parameter should just ignore it (via UNUSED_PARAMETER)
//
// in:
//      time - a pointer to a time_t structure
//      timedata - a pointer to a tm structure
// returns:
//      void
void localtime(::time_t *time, ::tm *timedata);

/////////////////////////////////////////////////
// Get a timestamp of now() as a string with millisecond precision on time
// There is no C++ standard way to do this, so it must be implemented for each platform
//
// returns:
//      A C++ string with a timestamp of now, in MM-DD-YYYY HH:MM:SS.mmm format.
std::string timestamp_ms();

} // namespace datetime

} // namespace ostrich

#endif /* OSTRICH_DATETIME_H_ */
