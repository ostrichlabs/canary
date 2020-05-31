/*
==========================================
Copyright (c) 2020 Ostrich Labs

Info message
==========================================
*/

#include "msg_info.h"
#include <string>

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::string ostrich::InfoMessage::toString() const {
    std::string msg;

    msg = this->getTimestamp();
    msg += ostrich::character::g_Space;

    switch (m_InfoLevel) {
        case ostrich::InfoType::INFO_DEFAULT:
        {
            msg += u8"[INFO]";
            break;
        }
        case ostrich::InfoType::INFO_DEBUG:
        {
            msg += u8"[DEBUG]";
            break;
        }
        default:
        {
            msg += u8"[UNKNOWN: ";
            msg += std::to_string(static_cast<int32_t>(m_InfoLevel));
            msg += u8']';
        }
    }

    msg += u8" Sent by \"";
    msg += this->getSenderMethod();
    msg += u8"\" with code (";
    msg += std::to_string(m_InfoCode);
    msg += u8") : ";
    msg += m_Text;

    return msg;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::string ostrich::InfoMessage::toVerboseString() const {
    return toString();
}