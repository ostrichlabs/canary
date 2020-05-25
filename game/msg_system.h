/*
==========================================
Copyright (c) 2020 Ostrich Labs

System message - some system related thing is happening, has happened, or needs to be acted on
==========================================
*/

#ifndef MSG_SYSTEM_H_
#define MSG_SYSTEM_H_

#include <memory>
#include "i_message.h"

namespace ostrich {

/////////////////////////////////////////////////
//
enum class SystemMsgType : int32_t {
    SYS_NULL = 0,
    SYS_QUIT,
    MAX_TYPES
};

/////////////////////////////////////////////////
//
class SystemMessage : public IMessage {
public:

    SystemMessage() = delete;
    virtual ~SystemMessage() {}
    SystemMessage(SystemMessage &&) = default;
    SystemMessage(const SystemMessage &) = default;
    SystemMessage &operator=(SystemMessage &&) = default;
    SystemMessage &operator=(const SystemMessage &) = default;

    virtual std::string toString() const override;

    SystemMsgType getType() const noexcept { return m_Type; }

//
// Factory constructors
//

    static std::shared_ptr<IMessage> Construct(SystemMsgType type, const char *sender) {
        return std::shared_ptr<IMessage>(new SystemMessage(type, sender));
    }

private:

    SystemMessage(SystemMsgType type, const char *sender) :
        IMessage(MessageType::MSG_SYSTEM, sender),
        m_Type(type)
    {}

    SystemMsgType m_Type;
};

} // namespace ostrich

#endif /* MSG_SYSTEM_H_ */