/*
==========================================
Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "datetime.h"
#include <cstdio>
#include <ctime>
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
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::string ostrich::datetime::timestamp() {
    ::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    ::tm timedata = { };
    ostrich::datetime::localtime(&now, &timedata);
    return ostrich::datetime::tmtostring(timedata);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::string ostrich::datetime::tmtostring(::tm &timedata, int milli) {
    constexpr int BUFSIZE = 30;
    char buffer[BUFSIZE] = { };
    ::snprintf(buffer, BUFSIZE, "%04d-%02d-%02d %02d:%02d:%02d.%03d", timedata.tm_year, timedata.tm_mon, timedata.tm_mday,
        timedata.tm_hour, timedata.tm_min, timedata.tm_sec, milli);

    return buffer;
}