/*
==========================================
Copyright (c) 2020 Ostrich Labs
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
    std::ostringstream oss;

    oss << timedata.tm_year << u8'-';
    if (timedata.tm_mon < 10)
        oss << u8'0';
    oss << timedata.tm_mon << u8'-';
    if (timedata.tm_mday < 10)
        oss << u8'0';
    oss << timedata.tm_mday << ost_char::g_Space;
    if (timedata.tm_hour < 10)
        oss << u8'0';
    oss << timedata.tm_hour << u8':';
    if (timedata.tm_min < 10)
        oss << u8'0';
    oss << timedata.tm_min << u8':';
    if (timedata.tm_sec < 10)
        oss << u8'0';
    oss << timedata.tm_sec;
    if (milli > -1) {
        oss << u8'.';
        if (milli < 100)
            oss << u8'0';
        if (milli < 10)
            oss << u8'0';
        oss << milli;
    }

    return oss.str();
}
