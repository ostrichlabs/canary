/*
==========================================
Copyright (c) 2020 Ostrich Labs

Message - x happened, or y needs to happen

Should be lightweight, so copying is cheap.
Methods should provide data in context based on type.
Then when you want, say, mouse coords, you get just mouse coords.
==========================================
*/

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <utility>
#include "../common/ost_common.h"

/////////////////////////////////////////////////
// system message defines
// since the Message class is multipurpose, defines are better suited over enums
#define OST_SYSTEMMSG_NULL      0x0000
#define OST_SYSTEMMSG_QUIT      0x0001  // a sign to hard quit from the game regardless of game state
#define OST_SYSTEMMSG_SIGNAL    0x0002  // a signal was raised

namespace ostrich {

/////////////////////////////////////////////////
//
class Message {
public:

// add new message types as they are used
    enum class Type : int32_t {
        NULLTYPE = 0,

        SYSTEM,         // system commands

        INPUT_START = 10,   // first input message
        INPUT_KEY = 10,     // someone pressed a key
        INPUT_BUTTON,       // someone pressed a button
        INPUT_MOUSEPOS,     // reported mouse cursor
        INPUT_LAST = 20,

        MAXTYPES
    };

// mouse button constants
    static const int MOUSE_NONE = 0;
    static const int MOUSE_LBUTTON = 0x01;
    static const int MOUSE_RBUTTON = 0x02;
    static const int MOUSE_MBUTTON = 0x04;

    virtual ~Message() {}
    Message(Message &&) = default;
    Message(const Message &) = default;
    Message &operator=(Message &&) = default;
    Message &operator=(const Message &) = default;

    static Message CreateNullMessage() { return Message(); }
    static Message CreateNullMessage(const char *sender) { return Message(sender); }
    static Message CreateSystemMessage(int32_t systemcode, int32_t addldata, const char *sender)
    { return Message(Type::SYSTEM, systemcode, addldata, sender); }
    static Message CreateKeyMessage(int32_t keycode, bool keydown, const char *sender)
    { return Message(Type::INPUT_KEY, keycode, keydown, sender); }
    static Message CreateButtonMessage(int32_t buttoncode, const char *sender)
    { return Message(Type::INPUT_BUTTON, buttoncode, sender); }
    static Message CreateMousePosMessage(int32_t xpos, int32_t ypos, const char *sender)
    { return Message(Type::INPUT_MOUSEPOS, xpos, ypos, sender); }

    Type getType() const noexcept { return m_Type; }
    int32_t getTypeAsInt() const noexcept{ return static_cast<int32_t>(m_Type); }
    
    /* message-specific getters */

    int32_t getSystemCode() const noexcept { return m_Data1; }
    int32_t getSystemAddlData() const noexcept { return m_Data2; }
    std::pair<int32_t, bool> getKeyStatus() const noexcept { return std::make_pair(m_Data1, ((m_Data2 == 1) ? true : false)); }
    int32_t getButtonStatus() const noexcept { return m_Data1; }
    std::pair<int32_t, int32_t> getMouseCoords() const noexcept { return std::make_pair(m_Data1, m_Data2); }

    /* generic getters */

    int32_t getData1() const noexcept { return m_Data1; }
    int32_t getData2() const noexcept { return m_Data2; }

    void *getDataPointer() const noexcept { return m_DataPtr; }
    const char *getSender() const noexcept { return m_Sender; }

private:

    Message() : m_Type(Type::NULLTYPE), m_Data1(0), m_Data2(0), m_DataPtr(nullptr), m_Sender(nullptr) {}

    Message(const char *sender) : m_Type(Type::NULLTYPE), m_Data1(0), m_Data2(0), m_DataPtr(nullptr), m_Sender(sender) {}

    // Delegate constructor
    Message(Type type, int32_t data1, int32_t data2, void *dataptr, const char *sender) :
        m_Type(type), m_Data1(data1), m_Data2(data2), m_DataPtr(dataptr), m_Sender(sender) {}

    // Button message constructor
    Message(Type type, int32_t buttoncode, const char *sender) :
        Message(type, buttoncode, 0, nullptr, sender) {}

    // Key message constructor
    Message(Type type, int32_t keycode, bool keydown, const char *sender) :
        Message(type, keycode, (keydown ? 1 : 0), nullptr, sender) {}

    // Mouse coords, system message constructor
    Message(Type type, int32_t data1, int32_t data2, const char *sender) :
        Message(type, data1, data2, nullptr, sender) {}

    Type m_Type;
    int32_t m_Data1;
    int32_t m_Data2;
    void *m_DataPtr;

    const char *m_Sender;
};

} // namespace ostrich

#endif /* MESSAGE_H_ */
