/*
==========================================
ost_message.h

Copyright (c) 2019 Ostrich Labs

An Ostrich Message, modeled after Windows messages - with some small modifications

This class can be inherited in order to pass other kinds of messages that may require more data
==========================================
*/

#ifndef OST_MESSAGE_H_
#define OST_MESSAGE_H_

#include <ctime>
#include <string_view>
#include "../common/ost_common.h"

namespace ostrich {

/////////////////////////////////////////////////
// Remember to update Message::getTypeAsString() when adding new messages!!!
enum class MessageType : int32_t {
    MSG_NULL = 0,
    MSG_SYSTEM, // a system command - quit, restart, etc.
	MSG_INPUT,	// input from keyboard, mouse, etc.
    MSG_INFO,   // informational only - print a message to the console
    MSG_DEBUG,  // same as MSG_INFO, but should only print if debugging is on
    MSG_MAX
};

/////////////////////////////////////////////////
//
enum class SubMessageType : int32_t {
    MSG_NULL = 0,
    MSG_SYS_QUIT,   // time to close the game!
	MSG_INPUT_KEYBOARD,	// a key's state changed
	MSG_INPUT_MOUSE, // the mouse moved
    MSG_DEBUG_SIGNAL, // raised signal
    MSG_DEBUG_FROMPROCESSKBM,   // from ProcessKBM()
    MSG_DEBUG_FROMPROCESSOSMSG, // from ProcessOSMessages()
    MSG_SUBTYPE_MAX,
};

/////////////////////////////////////////////////
// Generic message for generic information passing
class Message {
public:
// constructors must set types and message data is immutable, so do not allow default construction
	Message() = delete;
    virtual ~Message() {}
    Message(Message &&) = default;
    Message(const Message &) = default;
    Message &operator=(Message &&) = default;
    Message &operator=(const Message &) = default;

    Message(MessageType msgtype, SubMessageType subtype, int32_t addldata, const char *sender) :
        m_Type(msgtype), m_SubType(subtype), m_AddlData(addldata), m_Sender(sender), m_Timestamp(0)
    	{ m_Timestamp = ::time(nullptr); }

    MessageType getType() const noexcept { return m_Type; }
    SubMessageType getSubtype() const noexcept { return m_SubType; }
    int32_t getAddlData() const noexcept { return m_AddlData; }

    std::string_view getSender() const noexcept { return m_Sender; }
    std::time_t getRawTimestamp() const noexcept { return m_Timestamp; }

    int32_t getTypeAsInt() const noexcept { return static_cast<int32_t>(m_Type); }
    int32_t getSubtypeAsInt() const noexcept { return static_cast<int32_t>(m_SubType); }

    std::string_view getTypeAsString() const noexcept;
    std::string_view getSubtypeAsString() const noexcept;

protected:

    MessageType     m_Type;
    SubMessageType  m_SubType;
    int32_t			m_AddlData;

    std::string_view 	m_Sender;
    std::time_t			m_Timestamp;

};

} // namespace ostrich

#endif /* OST_MESSAGE_H_ */
