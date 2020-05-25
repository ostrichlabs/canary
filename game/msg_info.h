/*
==========================================
Copyright (c) 2020 Ostrich Labs

Info message - informational only. Probably printed to the console.
==========================================
*/

#ifndef MSG_INFO_H_
#define MSG_INFO_H_

#include <memory>
#include "i_message.h"

namespace ostrich {

/////////////////////////////////////////////////
//
enum class InfoType : int32_t {
    INFO_NULL = 0,
    INFO_DEBUG,
    INFO_DEFAULT,
    MAX_TYPES
};

/////////////////////////////////////////////////
//
class InfoMessage : public IMessage {
public:

    InfoMessage() = delete;
    virtual ~InfoMessage() {}
    InfoMessage(InfoMessage &&) = default;
    InfoMessage(const InfoMessage &) = default;
    InfoMessage &operator=(InfoMessage &&) = default;
    InfoMessage &operator=(const InfoMessage &) = default;

    virtual std::string toString() const override;

    InfoType getInfoLevel() const noexcept { return m_InfoLevel; }
    int32_t getInfoCode() const noexcept { return m_InfoCode; }
    std::string_view getText() const noexcept { return m_Text; }

//
// Factory constructors
//

    static std::shared_ptr<IMessage> ConstructDebugMessage(int32_t infocode, std::string_view text, const char *sender) {
        return std::shared_ptr<IMessage>(new InfoMessage(InfoType::INFO_DEBUG, infocode, text, sender));
    }

    static std::shared_ptr<IMessage> ConstructDefaultMessage(int32_t infocode, std::string_view text, const char *sender) {
        return std::shared_ptr<IMessage>(new InfoMessage(InfoType::INFO_DEFAULT, infocode, text, sender));
    }

private:

    InfoMessage(InfoType infolevel, int32_t infocode, std::string_view text, const char *sender) :
        IMessage(MessageType::MSG_INFO, sender),
        m_InfoLevel(infolevel), m_InfoCode(infocode), m_Text(text)
    {}

    InfoType m_InfoLevel;
    int32_t m_InfoCode;
    std::string m_Text;
};

} // namespace ostrich

#endif /* MSG_INFO_H_ */