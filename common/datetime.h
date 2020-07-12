/*
==========================================
Copyright (c) 2020 Ostrich Labs

Functions and classes for retrieving the date and time
Includes high-resolution clock access, though it's mostly a wrapper for <chrono>

The expectation is that we have nanosecond-level granularity in high_performance_clock
Reading <chrono> for both Windows and the Raspberry Pi version of GCC says we do
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
// get a timestamp of now() as a string with millisecond precision on time
std::string timestamp_ms();

} // namespace datetime

} // namespace ostrich

#endif /* DATETIME_H_ */
