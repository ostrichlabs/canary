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
//
// Canary doesn't differentiate between left and right shift/alt/ctrl
enum class Keys : int32_t {
    OSTKEY_NULL = 0,

    OSTKEY_TAB = 9,
    OSTKEY_ENTER = 19,
    OSTKEY_ESCAPE = 27,
    OSTKEY_SPACE = 32,

    OSTKEY_BACKSPACE = 127,

    OSTKEY_COMMAND = 128,

    OSTKEY_CAPSLOCK,
    OSTKEY_PAUSE,

    OSTKEY_UPARROW,
    OSTKEY_DOWNARROW,
    OSTKEY_LEFTARROW,
    OSTKEY_RIGHTARROW,

    OSTKEY_ALT,
    OSTKEY_CTRL,
    OSTKEY_SHIFT,
    OSTKEY_INSERT,
    OSTKEY_DELETE,
    OSTKEY_PAGEDOWN,
    OSTKEY_PAGEUP,
    OSTKEY_HOME,
    OSTKEY_END,

    OSTKEY_F1,
    OSTKEY_F2,
    OSTKEY_F3,
    OSTKEY_F4,
    OSTKEY_F5,
    OSTKEY_F6,
    OSTKEY_F7,
    OSTKEY_F8,
    OSTKEY_F9,
    OSTKEY_F10,
    OSTKEY_F11,
    OSTKEY_F12,

    OSTKEY_KEYPAD_0,
    OSTKEY_KEYPAD_1,
    OSTKEY_KEYPAD_2,
    OSTKEY_KEYPAD_3,
    OSTKEY_KEYPAD_4,
    OSTKEY_KEYPAD_5,
    OSTKEY_KEYPAD_6,
    OSTKEY_KEYPAD_7,
    OSTKEY_KEYPAD_8,
    OSTKEY_KEYPAD_9,
    OSTKEY_KEYPAD_DELETE,
    OSTKEY_KEYPAD_PLUS,
    OSTKEY_KEYPAD_MINUS,
    OSTKEY_KEYPAD_STAR,
    OSTKEY_KEYPAD_SLASH,
    OSTKEY_KEYPAD_NUMLOCK,

    OSTKEY_SCROLLOCK,

    OSTKEY_MOUSE1,
    OSTKEY_MOUSE2,
    OSTKEY_MOUSE3,
    OSTKEY_MOUSE4,
    OSTKEY_MOUSE5,

    OSTKEY_MOUSEWHEELUP,
    OSTKEY_MOUSEWHEELDOWN,

    OSTKEY_LAST = 255
};

constexpr int32_t KeyToInt32(Keys key) {
    return static_cast<int32_t>(key);
}

} // namespace ostrich

#endif /* KEYDEF_H_ */
