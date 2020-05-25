/*
==========================================
Copyright (c) 2020 Ostrich Labs

Base Message interface for the messaging system.

Derived classes should define toString() as well as any data/accessors, but messages should be immutable.
==========================================
*/

#ifndef I_MESSAGE_H_
#define I_MESSAGE_H_

#include "../common/ost_common.h"

#include <string_view>
#include <ctime>

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
//
class IMessage {
public:

    // protected constructor must set type and message data is immutable, so do not allow default construction
    IMessage() = delete;
    virtual ~IMessage() {}
    IMessage(IMessage &&) = default;
    IMessage(const IMessage &) = default;
    IMessage &operator=(IMessage &&) = default;
    IMessage &operator=(const IMessage &) = default;

    virtual std::string toString() const = 0;

    MessageType getMessageType() const noexcept { return m_MessageType; }
    std::string_view getSenderMethod() const noexcept { return m_SenderMethod; }
    std::time_t getRawTimestamp() const noexcept { return m_Timestamp; }

protected:

    IMessage(MessageType messagetype, const char *sender) : m_MessageType(messagetype), m_SenderMethod(sender)
    { m_Timestamp = ::time(nullptr); }

    MessageType         m_MessageType;
    std::string_view    m_SenderMethod;
    std::time_t			m_Timestamp;
};

} // namespace ostrich

#endif /* I_MESSAGE_H_ */