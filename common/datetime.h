/*
==========================================
Copyright (c) 2020 Ostrich Labs

Functions and classes for retrieving the date and time
==========================================
*/

#ifndef DATETIME_H_
#define DATETIME_H_

#include <chrono>
#include <string>

namespace ostrich {

namespace timer {

typedef std::chrono::steady_clock               clock;
typedef std::chrono::steady_clock::time_point   time_point;

/////////////////////////////////////////////////
// calls steady_clock::now()
time_point now();

/////////////////////////////////////////////////
// returns the interval between points in milliseconds
int32_t interval(const time_point &start, const time_point &end);

double interval_d(const time_point &start, const time_point &end);

} // namespace timer

namespace datetime {

/////////////////////////////////////////////////
// get a simple timestamp of now() as a string
std::string timestamp();

/////////////////////////////////////////////////
// format raw timestamp data into a string, specifying -1 for milli if it should be excluded
std::string tmtostring(::tm &timedata, int milli = -1);

/////////////////////////////////////////////////
// Platform-Specific Functions
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// calls the safe version of localtime, which for some dumb reason is different on each platform
void localtime(::time_t *time, ::tm *timedata);

/////////////////////////////////////////////////
// get a timestamp of now() as a string with millisecond precision on time
// there is no C++ standard way to do this, so it must be implemented for each platform
std::string timestamp_ms();

} // namespace datetime

} // namespace ostrich

#endif /* DATETIME_H_ */
