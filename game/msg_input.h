/*
==========================================
Copyright (c) 2020 Ostrich Labs

Input message
==========================================
*/

#ifndef MSG_INPUT_H_
#define MSG_INPUT_H_

#include <memory>
#include "i_message.h"
#include "keydef.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class InputMessage : public IMessage {
public:

    InputMessage() = delete;
    virtual ~InputMessage() {}
    InputMessage(InputMessage &&) = default;
    InputMessage(const InputMessage &) = default;
    InputMessage &operator=(InputMessage &&) = default;
    InputMessage &operator=(const InputMessage &) = default;

    virtual std::string toString() const override { return std::string(u8"IMPLEMENT ME"); }

    Button getButton() const noexcept { return m_Button; }
    ButtonState getButtonState() const noexcept { return m_ButtonState; }
    int32_t getKeyValue() const noexcept { return m_KeyValue; }
    int32_t getXPos() const noexcept { return m_XPos; }
    int32_t getYPos() const noexcept { return m_YPos; }

//
// Factory constructors
//

    static std::shared_ptr<IMessage> Construct(Button button, ButtonState buttonstate, const char *sender) {
        return std::shared_ptr<IMessage>(new InputMessage(button, buttonstate, sender));
    }

    static std::shared_ptr<IMessage> Construct(Button button, ButtonState buttonstate, int32_t xpos, int32_t ypos, const char *sender) {
        return std::shared_ptr<IMessage>(new InputMessage(button, buttonstate, xpos, ypos, sender));
    }

private:

    InputMessage(Button button, ButtonState buttonstate, const char *sender) :
        IMessage(MessageType::MSG_INPUT, sender),
        m_Button(button), m_ButtonState(buttonstate), m_KeyValue(0), m_XPos(-1), m_YPos(-1)
    {}

    InputMessage(Button button, ButtonState buttonstate, int32_t xpos, int32_t ypos, const char *sender) :
        IMessage(MessageType::MSG_INPUT, sender),
        m_Button(button), m_ButtonState(buttonstate), m_KeyValue(0), m_XPos(xpos), m_YPos(ypos)
    {}

    Button m_Button;
    ButtonState m_ButtonState;
    int32_t m_KeyValue;
    int32_t m_XPos;
    int32_t m_YPos;
};

} // namespace ostrich

#endif /* MSG_INPUT_H_ */