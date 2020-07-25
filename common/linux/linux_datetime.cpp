/*
==========================================
Copyright (c) 2020 Ostrich Labs

Linux/Raspi implementations of datetime
==========================================
*/

#include "../ost_common.h"

#if ((OST_LINUX != 1) && (OST_RASPI != 1))
#   error "This module should only be included in Raspberry Pi or Linux builds"
#endif

#include "../datetime.h"

#include <ctime>

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::datetime::localtime(::time_t *time, ::tm *timedata) {
    ::localtime_r(time, timedata);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::string ostrich::datetime::timestamp_ms() {
    ::timespec ts = { };

    if (::clock_gettime(CLOCK_REALTIME, &ts) != 0)
        return "";

    std::tm timedata = { };

    ostrich::datetime::localtime(&ts.tv_sec, &timedata);

    // adjust wacky ranges to match the Ostrich Standard
    timedata.tm_mon += 1;
    timedata.tm_year += 1900;

    return ostrich::datetime::tmtostring(timedata, (ts.tv_nsec / 1'000'000));
}
