/*
==========================================
Copyright (c) 2020 Ostrich Labs

Windows implementations of datetime
==========================================
*/

#include "../ost_common.h"

#if (OST_WINDOWS != 1)
#   error "This module should only be included in Windows builds"
#endif

#include "../datetime.h"

#include <Windows.h>
#include <ctime>
#include <sstream>

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::datetime::localtime(::time_t *time, ::tm *timedata) {
    ::localtime_s(timedata, time);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::string ostrich::datetime::timestamp_ms() {
    SYSTEMTIME time = { };

    ::GetLocalTime(&time);

    ::tm timedata = { };
    timedata.tm_year = time.wYear;
    timedata.tm_mday = time.wDay;
    timedata.tm_mon = time.wMonth;
    timedata.tm_hour = time.wHour;
    timedata.tm_min = time.wMinute;
    timedata.tm_sec = time.wSecond;

    return ostrich::datetime::tmtostring(timedata, time.wMilliseconds);
}
