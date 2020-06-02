/*
==========================================
Copyright (c) 2020 Ostrich Labs

Definitions of internal keys and key state
==========================================
*/

#ifndef KEYDEF_H_
#define KEYDEF_H_

#include <cstdint>

namespace ostrich {

/////////////////////////////////////////////////
// mouse and keyboard keys, some of which don't have a UTF-8 equivalent
enum class Keys : int32_t {
    KEY_NULL = 0,

    KEY_TAB = 9,
    KEY_ENTER = 19,
    KEY_ESCAPE = 27,
    KEY_SPACE = 32,

    KEY_BACKSPACE = 127,

    KEY_COMMAND = 128,

    KEY_CAPSLOCK,
    KEY_PAUSE,

    KEY_UPARROW,
    KEY_DOWNARROW,
    KEY_LEFTARROW,
    KEY_RIGHTARROW,

    KEY_ALT,
    KEY_CTRL,
    KEY_SHIFT,
    KEY_INSERT,
    KEY_DELETE,
    KEY_PAGEDOWN,
    KEY_PAGEUP,
    KEY_HOME,
    KEY_END,

    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,

    KEY_KEYPAD_0,
    KEY_KEYPAD_1,
    KEY_KEYPAD_2,
    KEY_KEYPAD_3,
    KEY_KEYPAD_4,
    KEY_KEYPAD_5,
    KEY_KEYPAD_6,
    KEY_KEYPAD_7,
    KEY_KEYPAD_8,
    KEY_KEYPAD_9,
    KEY_KEYPAD_DELETE,
    KEY_KEYPAD_PLUS,
    KEY_KEYPAD_MINUS,
    KEY_KEYPAD_STAR,
    KEY_KEYPAD_SLASH,
    KEY_KEYPAD_NUMLOCK,

    KEY_MOUSE1,
    KEY_MOUSE2,
    KEY_MOUSE3,
    KEY_MOUSE4,
    KEY_MOUSE5,

    KEY_MOUSEWHEELUP,
    KEY_MOUSEWHEELDOWN,

    KEY_LAST
};

} // namespace ostrich

#endif /* KEYDEF_H_ */
