/*
==========================================
Copyright (c) 2020-2021 Ostrich Labs

Message - x happened, or y needs to happen

Should be lightweight, so copying is cheap.
Methods should provide data in context based on type.
Then when you want, say, mouse coords, you get just mouse coords.
==========================================
*/

#ifndef OSTRICH_MESSAGE_H_
#define OSTRICH_MESSAGE_H_

#include <string>
#include <string_view>
#include <utility>
#include "../common/datetime.h"
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

    /////////////////////////////////////////////////
    // Official Supported Message Types
    // Add new types here as they are used
    enum class Type : int32_t {
        NULLTYPE = 0,

        SYSTEM, // system commands

        STATE,  // state machine commands

        INPUT_START = 100,  // first input message
        INPUT_KEY = 100,    // someone pressed a key
        INPUT_BUTTON,       // someone pressed a button
        INPUT_MOUSEPOS,     // reported mouse cursor
        INPUT_LAST = 200,

        MAXTYPES
    };

    /////////////////////////////////////////////////
    // Mouse button constants
    static const int MOUSE_NONE = 0;
    static const int MOUSE_LBUTTON = 0x01;
    static const int MOUSE_RBUTTON = 0x02;
    static const int MOUSE_MBUTTON = 0x04;

    /////////////////////////////////////////////////
    // Constructors are all private; use the static factory methods to create Messages
    // Destructor can do nothing because all data is either simple or has its own destructor
    // Data is all either simple or copyable, so copy/move constructors/operators are default
    virtual ~Message() {}
    Message(Message &&) = default;
    Message(const Message &) = default;
    Message &operator=(Message &&) = default;
    Message &operator=(const Message &) = default;

    /////////////////////////////////////////////////
    // Create a system message.
    //
    // in:
    //      systemcode - a system-defined code from the list of defines in this file
    //      addldata - miscellaneous extra data if necessary; its actual contents are dependent on the message
    //      sender - a string literal describing the message sender
    // returns:
    //      A constructed SYSTEM-type message.
    static Message CreateSystemMessage(int32_t systemcode, int32_t addldata, const char *sender)
    { return Message(Type::SYSTEM, systemcode, addldata, sender); }

    /////////////////////////////////////////////////
    // Create a Keyboard input message.
    // May also double for joystick/controller buttons.
    //
    // in:
    //      keycode - an int representation of an ostrich::Keys enum (TODO: should this take a ostrich::Keys variable directly and cast it here?)
    //      keydown - true if the key was pressed; false if the key was previously down but is no longer
    //      sender - a string literal describing the message sender
    // returns:
    //      A constructed KEY-type message.
    static Message CreateKeyMessage(int32_t keycode, bool keydown, const char *sender)
    { return Message(Type::INPUT_KEY, keycode, keydown, sender); }

    /////////////////////////////////////////////////
    // Create a Button input message.
    // Buttons are handled differently from other keys mostly because Windows handles them differently.
    // This is a good candidate for junking if it's possible to just use Key messages.
    //
    // in:
    //      buttoncode - a bitmap of the mouse buttons' status
    //      sender - a string literal describing the message sender
    // returns:
    //      A constructed BUTTON-type message.
    static Message CreateButtonMessage(int32_t buttoncode, const char *sender)
    { return Message(Type::INPUT_BUTTON, buttoncode, sender); }

    /////////////////////////////////////////////////
    // Create a Mouse Position message.
    // Note that mouse messages will need to be changed, because window systems will report coordinates differently.
    //
    // in:
    //      xpos - the mouse's current xpos as reported by the window system
    //      ypos - the mouse's current ypos as reported by the window system
    //      sender - a string literal describing the message sender
    // returns:
    //      A constructed MOUSEPOS-type message.
    static Message CreateMousePosMessage(int32_t xpos, int32_t ypos, const char *sender)
    { return Message(Type::INPUT_MOUSEPOS, xpos, ypos, sender); }

    /////////////////////////////////////////////////
    // accessor methods
    // generic to each message
    // data pointer is currently unused
    /////////////////////////////////////////////////

    Type getType() const noexcept { return m_Type; }
    int32_t getTypeAsInt() const noexcept{ return static_cast<int32_t>(m_Type); }

    void *getDataPointer() const noexcept { return m_DataPtr; }
    std::string_view getTimestamp() const noexcept { return std::string_view(m_Timestamp); }
    const char *getSender() const noexcept { return m_Sender; }
    
    /////////////////////////////////////////////////
    // contextual accessor methods
    // I'm not trying to get fancy with these; if you have a x message, use x accessors.
    // That should ensure you get the data you want regardless of its location.
    /////////////////////////////////////////////////

    int32_t getSystemCode() const noexcept { return m_Data1; }
    int32_t getSystemAddlData() const noexcept { return m_Data2; }
    std::pair<int32_t, bool> getKeyStatus() const noexcept { return std::make_pair(m_Data1, ((m_Data2 == 1) ? true : false)); }
    int32_t getButtonStatus() const noexcept { return m_Data1; }
    std::pair<int32_t, int32_t> getMouseCoords() const noexcept { return std::make_pair(m_Data1, m_Data2); }

    /////////////////////////////////////////////////
    // generic data accessor methods
    // in case you just want the two data fields and don't care about context
    /////////////////////////////////////////////////

    int32_t getData1() const noexcept { return m_Data1; }
    int32_t getData2() const noexcept { return m_Data2; }

private:

    /////////////////////////////////////////////////
    // Default constructor
    // Creates a NULLTYPE message with 0 or null data
    Message() : m_Type(Type::NULLTYPE), m_Data1(0), m_Data2(0), m_DataPtr(nullptr), m_Sender(nullptr) {}

    /////////////////////////////////////////////////
    // Semi-default constructor
    // Creates a NULLTYPE message with 0 or null data, except for the sender
    Message(const char *sender) : m_Type(Type::NULLTYPE), m_Data1(0), m_Data2(0), m_DataPtr(nullptr), m_Sender(sender) {}

    /////////////////////////////////////////////////
    // Delegate constructor
    // Used as a helper for the more specific constructors; should fill the entire message object
    Message(Type type, int32_t data1, int32_t data2, void *dataptr, const char *sender) :
        m_Type(type), m_Data1(data1), m_Data2(data2), m_DataPtr(dataptr), m_Sender(sender), m_Timestamp(ostrich::datetime::timestamp_ms())
    {}

    /////////////////////////////////////////////////
    // Single integer constructor
    // Currently used with BUTTON type messages
    Message(Type type, int32_t buttoncode, const char *sender) :
        Message(type, buttoncode, 0, nullptr, sender) {}

    /////////////////////////////////////////////////
    // Integer + boolean constructor
    // The boolean is converted to a known 1/0 value rather than trusting the compiler to be consistent
    // Currently used with KEY type messages
    Message(Type type, int32_t keycode, bool keydown, const char *sender) :
        Message(type, keycode, (keydown ? 1 : 0), nullptr, sender) {}

    /////////////////////////////////////////////////
    // Dual integer constructor
    // Currently used with MOUSEPOS and SYSTEM type messages
    Message(Type type, int32_t data1, int32_t data2, const char *sender) :
        Message(type, data1, data2, nullptr, sender) {}

    Type m_Type;
    int32_t m_Data1;
    int32_t m_Data2;
    void *m_DataPtr;

    const char *m_Sender;
    std::string m_Timestamp;
};

} // namespace ostrich

#endif /* OSTRICH_MESSAGE_H_ */
