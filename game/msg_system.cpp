/*
==========================================
Copyright (c) 2020 Ostrich Labs

System message
==========================================
*/

#include "msg_system.h"
#include <string>

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::string ostrich::SystemMessage::toString() const {
    std::string msg;

    msg = getTimestamp();
    msg += u8" [SYSTEM] ";

    if (m_Type == ostrich::SystemMsgType::SYS_NULL) {
        msg += u8"SYS_NULL";
    } else if (m_Type == ostrich::SystemMsgType::SYS_QUIT) {
        msg += u8"SYS_QUIT";
    } else {
        msg += u8"Unknown: ";
        msg += std::to_string(static_cast<int32_t>(m_Type));
    }

    msg += u8"Sent by \"";
    msg += getSenderMethod();
    msg += u8"\"";

    return msg;
}