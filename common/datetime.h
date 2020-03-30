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

typedef std::chrono::high_resolution_clock              clock;
typedef std::chrono::high_resolution_clock::time_point  time_point;

/////////////////////////////////////////////////
// calls high_resolution_clock::now()
time_point now();

/////////////////////////////////////////////////
// returns the interval between points in milliseconds
int32_t interval(const time_point &start, const time_point &end);

} // namespace timer

namespace datetime {

/////////////////////////////////////////////////
// get a timestamp of now() as a string
std::string timestamp();

/////////////////////////////////////////////////
// convert a given timestamp to a string
std::string timetostring(time_t &t);

} // namespace datetime

} // namespace ostrich

#endif /* DATETIME_H_ */
