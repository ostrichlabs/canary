/*
==========================================
Copyright (c) 2020 Ostrich Labs

Input message
==========================================
*/

#include "msg_input.h"
#include <string>

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::string ostrich::InputMessage::toString() const {
    std::string msg;

    msg = this->getTimestamp();

    switch (m_Type) {
        case ostrich::KeyType::KEYTYPE_KB:
        {
            msg += u8" [KEYBOARD] Key activity: ";
            msg += std::to_string(m_Key);
            msg += u8" in state ";
            if (m_Keydown)
                msg += u8" DOWN.";
            else
                msg += u8" UP.";
            break;
        }
        case ostrich::KeyType::KEYTYPE_MOUSE:
        {
            msg += u8" [MOUSE] Mouse activity: Location (";
            msg += std::to_string(m_XPos);
            msg += u8",";
            msg += std::to_string(m_YPos);
            msg += u8"); button status ";
            msg += std::to_string(m_Key);
            break;
        }
        default:
        {
            msg += u8" [INPUT] Unknown input type : ";
            msg += std::to_string(static_cast<int32_t>(m_Type));
            break;
        }
    }
    
    return msg;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::string ostrich::InputMessage::toVerboseString() const {
    std::string msg;

    msg = this->getTimestamp();

    switch (m_Type) {
        case ostrich::KeyType::KEYTYPE_KB:
        {
            msg += u8" [KEYBOARD] ";
            break;
        }
        case ostrich::KeyType::KEYTYPE_MOUSE:
        {
            msg += u8" [MOUSE] ";
            break;
        }
        default:
        {
            msg += u8" [INPUT : ";
            msg += std::to_string(static_cast<int32_t>(m_Type));
            msg += u8" ] ";
            break;
        }
    }

    msg += u8" Key: <";
    msg += std::to_string(m_Key);
    msg += u8"> XPos: <";
    msg += std::to_string(m_XPos);
    msg += u8"> YPos: <";
    msg += std::to_string(m_YPos);
    msg += u8"> Keydown: <";
    msg += ((m_Keydown) ? u8"true" : u8"false");
    msg += u8">";

    return msg;
}