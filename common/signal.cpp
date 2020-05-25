/*
==========================================
Copyright (c) 2020 Ostrich Labs

Signal handling
==========================================
*/

#include "signal.h"
#include <csignal>
#include "../game/msg_info.h"
#include "../game/msg_system.h"

namespace {

ostrich::EventSender l_EventSender;

/////////////////////////////////////////////////
// TODO: SIGILL/SIGSEGV/SIGFPE support?
/////////////////////////////////////////////////
extern "C" void SignalHandler(int signum) {
    l_EventSender.Send(ostrich::InfoMessage::ConstructDebugMessage(signum, u8"Signal raised", OST_FUNCTION_SIGNATURE));

    switch (signum) {
        case SIGABRT:
        case SIGINT:
        case SIGTERM:
            l_EventSender.Send(ostrich::SystemMessage::Construct(ostrich::SystemMsgType::SYS_QUIT, OST_FUNCTION_SIGNATURE));
            break;
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