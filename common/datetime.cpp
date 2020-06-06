/*
==========================================
Copyright (c) 2020 Ostrich Labs

Functions and classes for retrieving the date and time
==========================================
*/

#include "datetime.h"
#include <ctime>
#include <iomanip>
#include <ratio>
#include <sstream>
#include "ost_common.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::timer::time_point ostrich::timer::now() {
    return ostrich::timer::clock::now();
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int32_t ostrich::timer::interval(const ostrich::timer::time_point &start, const ostrich::timer::time_point &end) {
    return static_cast<int32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
double ostrich::timer::interval_d(const ostrich::timer::time_point &start, const ostrich::timer::time_point &end) {
    return std::chrono::duration<double, std::milli>(end - start).count();
    //return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::string ostrich::datetime::timestamp() {
    time_t now_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    return ostrich::datetime::timetostring(now_t);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::string ostrich::datetime::timetostring(time_t &t) {
    std::tm now_tm;
#if (OST_WINDOWS == 1) // god damn you windows
    ::localtime_s(&now_tm, &t);
#else
    now_tm = *::localtime(&t);
#endif
    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%F %T");
    return oss.str();
}
