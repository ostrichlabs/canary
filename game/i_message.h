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
#include "../common/datetime.h"

namespace ostrich {

/////////////////////////////////////////////////
//
enum class MessageType : int32_t {
    MSG_NULL = 0,
    MSG_SYSTEM, // a system command - quit, restart, etc.
    MSG_INPUT,	// input from keyboard, mouse, etc.
    MSG_INFO,   // informational only - print a message to the console
    MSG_MAX
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

    // TODO: add toVerboseString()

    MessageType getMessageType() const noexcept { return m_MessageType; }
    std::string_view getSenderMethod() const noexcept { return m_SenderMethod; }
    std::string_view getTimestamp() const noexcept { return std::string_view(m_Timestamp); }

protected:

    IMessage(MessageType messagetype, const char *sender) : m_MessageType(messagetype), m_SenderMethod(sender),
        m_Timestamp(ostrich::datetime::timestamp())
    {}

    MessageType         m_MessageType;
    std::string_view    m_SenderMethod;
    std::string         m_Timestamp;
};

} // namespace ostrich

#endif /* I_MESSAGE_H_ */