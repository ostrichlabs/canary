/*
==========================================
Copyright (c) 2019 Ostrich Labs

A Message-derived class specifically for input messages
==========================================
*/

#ifndef OST_INPUTMESSAGE_H_
#define OST_INPUTMESSAGE_H_

#include "keydef.h"
#include "ost_message.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class InputMessage : public Message {
public:
	InputMessage() = delete;
    virtual ~InputMessage() {}
    InputMessage(InputMessage &&) = default;
    InputMessage(const InputMessage &) = default;
    InputMessage &operator=(InputMessage &&) = default;
    InputMessage &operator=(const InputMessage &) = default;

    InputMessage(Button button, ButtonState buttonstate, const char *sender) :
    	Message(MessageType::MSG_INPUT, SubMessageType::MSG_INPUT_KEYBOARD, 0, sender),
		m_Button(button), m_ButtonState(buttonstate), m_KeyValue(0), m_XPos(-1), m_YPos(-1) {}

    InputMessage(Button button, ButtonState buttonstate, int32_t xpos, int32_t ypos, const char *sender) :
    	Message(MessageType::MSG_INPUT, SubMessageType::MSG_INPUT_MOUSE, 0, sender),
		m_Button(button), m_ButtonState(buttonstate), m_KeyValue(0), m_XPos(xpos), m_YPos(ypos) {}

    Button getButton() const noexcept { return m_Button; }
    ButtonState getButtonState() const noexcept { return m_ButtonState; }
    int32_t getKeyValue() const noexcept { return m_KeyValue; }
    int32_t getXPos() const noexcept { return m_XPos; }
    int32_t getYPos() const noexcept { return m_YPos; }

private:

    Button m_Button;
    ButtonState m_ButtonState;
    int32_t m_KeyValue;
    int32_t m_XPos;
    int32_t m_YPos;
};

} // namespace ostrich

#endif /* OST_INPUTMESSAGE_H_ */
