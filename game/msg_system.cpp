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

    msg = this->getTimestamp();
    msg += u8" [SYSTEM] ";

    switch (m_Type) {
        case ostrich::SystemMsgType::SYS_QUIT:
        {
            msg += u8"SYS_QUIT";
            break;
        }
        default:
        {
            msg += u8"Unknown: ";
            msg += std::to_string(static_cast<int32_t>(m_Type));
            break;
        }
    }

    msg += u8"Sent by \"";
    msg += this->getSenderMethod();
    msg += u8"\"";

    return msg;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::string ostrich::SystemMessage::toVerboseString() const {
    return this->toString();
}