/*
==========================================
Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "ost_message.h"
#include <string>

namespace {

const std::string g_MsgTypeUnknown = u8"MSG_UNKNOWN";

const std::string g_MsgTypeStrings[] = {
    u8"MSG_NULL",
    u8"MSG_SYSTEM",
	u8"MSG_INPUT",
    u8"MSG_INFO",
    u8"MSG_DEBUG",
    u8"MSG_MAX"
};

const std::string g_MsgSubtypeStrings[] = {
    u8"MSG_NULL",
    u8"MSG_SYS_QUIT",
	u8"MSG_INPUT_KEYBOARD",
	u8"MSG_INPUT_MOUSE",
    u8"MSG_DEBUG_SIGNAL",
    u8"MSG_DEBUG_FROMPROCESSKBM",
    u8"MSG_DEBUG_FROMPROCESSOSMSG",
    u8"MSG_SUBTYPE_MAX"
};

} // namespace anonymous

#if (OST_WINDOWS == 1)
#   pragma warning(push)
#   pragma warning(4 : 4061)
#   pragma warning(4 : 4062)
#endif

/////////////////////////////////////////////////
// uses a switch to compel the compiler to warn if there are missing switches
/////////////////////////////////////////////////
const std::string &ostrich::Message::getTypeAsString() const noexcept {
    switch (m_Type) {
        case ostrich::MessageType::MSG_NULL:
        case ostrich::MessageType::MSG_SYSTEM:
        case ostrich::MessageType::MSG_INPUT:
        case ostrich::MessageType::MSG_INFO:
        case ostrich::MessageType::MSG_DEBUG:
        case ostrich::MessageType::MSG_MAX:
            return g_MsgTypeStrings[static_cast<int32_t>(m_Type)];
        default:
            return g_MsgTypeUnknown;
    }
}

/////////////////////////////////////////////////
// uses a switch to compel the compiler to warn if there are missing switches
/////////////////////////////////////////////////
const std::string &ostrich::Message::getSubtypeAsString() const noexcept {
    switch (m_SubType) {
        case ostrich::SubMessageType::MSG_NULL:
        case ostrich::SubMessageType::MSG_SYS_QUIT:
        case ostrich::SubMessageType::MSG_INPUT_KEYBOARD:
        case ostrich::SubMessageType::MSG_INPUT_MOUSE:
        case ostrich::SubMessageType::MSG_DEBUG_SIGNAL:
        case ostrich::SubMessageType::MSG_DEBUG_FROMPROCESSKBM:
        case ostrich::SubMessageType::MSG_DEBUG_FROMPROCESSOSMSG:
        case ostrich::SubMessageType::MSG_SUBTYPE_MAX:
            return g_MsgSubtypeStrings[static_cast<int32_t>(m_SubType)];
        default:
            return g_MsgTypeUnknown;
    }
}

#if (OST_WINDOWS == 1)
#   pragma warning(pop)
#endif
