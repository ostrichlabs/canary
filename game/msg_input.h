/*
==========================================
Copyright (c) 2020 Ostrich Labs

Input message

Handles both keyboard and mouse. Presumably controller as well if I ever add it.
==========================================
*/

#ifndef MSG_INPUT_H_
#define MSG_INPUT_H_

#include <memory>
#include "i_message.h"
#include "keydef.h"

namespace ostrich {

/////////////////////////////////////////////////
// can't really be an enum because lol I'm not putting every unicode character in an enum
#define OST_MOUSE_NONE    0
#define OST_MOUSE_LBUTTON 1
#define OST_MOUSE_RBUTTON 2
#define OST_MOUSE_MBUTTON 4

/////////////////////////////////////////////////
//
enum class KeyType : int32_t {
    KEYTYPE_NULL = 0,
    KEYTYPE_KB,
    KEYTYPE_MOUSE,
    //KEYTYPE_CONTROLLER,
    KEYTYPE_MAX
};

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

    virtual std::string toString() const override;

    KeyType getType() const noexcept { return m_Type; }
    int32_t getKey() const noexcept { return m_Key; }
    int32_t getXPos() const noexcept { return m_XPos; }
    int32_t getYPos() const noexcept { return m_YPos; }
    bool    isKeyDown() const noexcept { return m_Keydown; }

//
// Factory constructors
//

    static std::shared_ptr<IMessage> ConstructKB(int32_t key, bool keydown, const char *sender) {
        return std::shared_ptr<IMessage>(new InputMessage(key, keydown, sender));
    }

    static std::shared_ptr<IMessage> ConstructMouse(int32_t button, int32_t xpos, int32_t ypos, const char *sender) {
        return std::shared_ptr<IMessage>(new InputMessage(button, xpos, ypos, sender));
    }

private:

    InputMessage(int32_t key, bool keydown, const char *sender) :
        IMessage(MessageType::MSG_INPUT, sender),
        m_Type(KeyType::KEYTYPE_KB), m_Key(key), m_XPos(-1), m_YPos(-1), m_Keydown(keydown)
    {}

    InputMessage(int32_t button, int32_t xpos, int32_t ypos, const char *sender) :
        IMessage(MessageType::MSG_INPUT, sender),
        m_Type(KeyType::KEYTYPE_MOUSE), m_Key(button), m_XPos(xpos), m_YPos(ypos)
    {
        m_Keydown = (button == OST_MOUSE_NONE) ? false : true;
    }

    KeyType m_Type;     // KB/Mouse/Controller
    int32_t m_Key;      // if KB, this is the raw key. if mouse, this is L/R/M mouse (use defines).
    int32_t m_XPos;     // -1 if KB
    int32_t m_YPos;     // -1 if KB
    bool m_Keydown;     // true if down
};

} // namespace ostrich

#endif /* MSG_INPUT_H_ */