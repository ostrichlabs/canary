/*
==========================================
Copyright (c) 2020 Ostrich Labs

Signal handling
==========================================
*/

#include "signal.h"
#include <csignal>

namespace {

ostrich::EventSender l_EventSender;

/////////////////////////////////////////////////
// TODO: SIGILL/SIGSEGV/SIGFPE support?
/////////////////////////////////////////////////
extern "C" void SignalHandler(int signum) {
    switch (signum) {
        case SIGABRT:
        case SIGINT:
        case SIGTERM:
        {
            l_EventSender.Send(ostrich::Message::CreateSystemMessage(1, OST_FUNCTION_SIGNATURE));
            break;
        }
        default:
            break;
    }
}

} // anonymous namespace

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::signal::Initialize(ostrich::EventSender es) {
    if (es.isValid() && !l_EventSender.isValid()) {
        l_EventSender = es;

        ::signal(SIGABRT, SignalHandler);
        ::signal(SIGINT, SignalHandler);
        ::signal(SIGTERM, SignalHandler);

        return 0;
    }

    return -1;
}