/*
==========================================
Copyright (c) 2020 Ostrich Labs

windows::TranslateKey() unit tests
==========================================
*/

#include "gtest/gtest.h"

#include <Windows.h>
#include "../../win32/win_input.h"
#include "../../game/keydef.h"

using namespace ostrich::windows;

TEST(ostrich, WinTranslateKeyTest) {
    // 0-9 and A-Z should be ASCII equivalent
    ASSERT_EQ(TranslateKey('0'), '0');
    ASSERT_EQ(TranslateKey('1'), '1');
    ASSERT_EQ(TranslateKey('2'), '2');
    ASSERT_EQ(TranslateKey('3'), '3');
    ASSERT_EQ(TranslateKey('4'), '4');
    ASSERT_EQ(TranslateKey('5'), '5');
    ASSERT_EQ(TranslateKey('6'), '6');
    ASSERT_EQ(TranslateKey('7'), '7');
    ASSERT_EQ(TranslateKey('8'), '8');
    ASSERT_EQ(TranslateKey('9'), '9');

    ASSERT_EQ(TranslateKey('A'), 'A');
    ASSERT_EQ(TranslateKey('B'), 'B');
    ASSERT_EQ(TranslateKey('C'), 'C');
    ASSERT_EQ(TranslateKey('D'), 'D');
    ASSERT_EQ(TranslateKey('E'), 'E');
    ASSERT_EQ(TranslateKey('F'), 'F');
    ASSERT_EQ(TranslateKey('G'), 'G');
    ASSERT_EQ(TranslateKey('H'), 'H');
    ASSERT_EQ(TranslateKey('I'), 'I');
    ASSERT_EQ(TranslateKey('J'), 'J');
    ASSERT_EQ(TranslateKey('K'), 'K');
    ASSERT_EQ(TranslateKey('L'), 'L');
    ASSERT_EQ(TranslateKey('M'), 'M');
    ASSERT_EQ(TranslateKey('N'), 'N');
    ASSERT_EQ(TranslateKey('O'), 'O');
    ASSERT_EQ(TranslateKey('P'), 'P');
    ASSERT_EQ(TranslateKey('Q'), 'Q');
    ASSERT_EQ(TranslateKey('R'), 'R');
    ASSERT_EQ(TranslateKey('S'), 'S');
    ASSERT_EQ(TranslateKey('T'), 'T');
    ASSERT_EQ(TranslateKey('U'), 'U');
    ASSERT_EQ(TranslateKey('V'), 'V');
    ASSERT_EQ(TranslateKey('W'), 'W');
    ASSERT_EQ(TranslateKey('X'), 'X');
    ASSERT_EQ(TranslateKey('Y'), 'Y');
    ASSERT_EQ(TranslateKey('Z'), 'Z');

    // F keys
    ASSERT_EQ(TranslateKey(VK_F1), static_cast<int32_t>(ostrich::Keys::OSTKEY_F1));
    ASSERT_EQ(TranslateKey(VK_F2), static_cast<int32_t>(ostrich::Keys::OSTKEY_F2));
    ASSERT_EQ(TranslateKey(VK_F3), static_cast<int32_t>(ostrich::Keys::OSTKEY_F3));
    ASSERT_EQ(TranslateKey(VK_F4), static_cast<int32_t>(ostrich::Keys::OSTKEY_F4));
    ASSERT_EQ(TranslateKey(VK_F5), static_cast<int32_t>(ostrich::Keys::OSTKEY_F5));
    ASSERT_EQ(TranslateKey(VK_F6), static_cast<int32_t>(ostrich::Keys::OSTKEY_F6));
    ASSERT_EQ(TranslateKey(VK_F7), static_cast<int32_t>(ostrich::Keys::OSTKEY_F7));
    ASSERT_EQ(TranslateKey(VK_F8), static_cast<int32_t>(ostrich::Keys::OSTKEY_F8));
    ASSERT_EQ(TranslateKey(VK_F9), static_cast<int32_t>(ostrich::Keys::OSTKEY_F9));
    ASSERT_EQ(TranslateKey(VK_F10), static_cast<int32_t>(ostrich::Keys::OSTKEY_F10));
    ASSERT_EQ(TranslateKey(VK_F11), static_cast<int32_t>(ostrich::Keys::OSTKEY_F11));
    ASSERT_EQ(TranslateKey(VK_F12), static_cast<int32_t>(ostrich::Keys::OSTKEY_F12));

    // Numpad keys
    ASSERT_EQ(TranslateKey(VK_NUMPAD0), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_0));
    ASSERT_EQ(TranslateKey(VK_NUMPAD1), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_1));
    ASSERT_EQ(TranslateKey(VK_NUMPAD2), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_2));
    ASSERT_EQ(TranslateKey(VK_NUMPAD3), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_3));
    ASSERT_EQ(TranslateKey(VK_NUMPAD4), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_4));
    ASSERT_EQ(TranslateKey(VK_NUMPAD5), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_5));
    ASSERT_EQ(TranslateKey(VK_NUMPAD6), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_6));
    ASSERT_EQ(TranslateKey(VK_NUMPAD7), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_7));
    ASSERT_EQ(TranslateKey(VK_NUMPAD8), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_8));
    ASSERT_EQ(TranslateKey(VK_NUMPAD9), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_9));

    ASSERT_EQ(TranslateKey(VK_TAB), static_cast<int32_t>(ostrich::Keys::OSTKEY_TAB));
    ASSERT_EQ(TranslateKey(VK_ESCAPE), static_cast<int32_t>(ostrich::Keys::OSTKEY_ESCAPE));
    ASSERT_EQ(TranslateKey(VK_SPACE), ' ');

    ASSERT_EQ(TranslateKey(VK_CAPITAL), static_cast<int32_t>(ostrich::Keys::OSTKEY_CAPSLOCK));
    ASSERT_EQ(TranslateKey(VK_RETURN), static_cast<int32_t>(ostrich::Keys::OSTKEY_ENTER));
    ASSERT_EQ(TranslateKey(VK_BACK), static_cast<int32_t>(ostrich::Keys::OSTKEY_BACKSPACE));

    ASSERT_EQ(TranslateKey(VK_UP), static_cast<int32_t>(ostrich::Keys::OSTKEY_UPARROW));
    ASSERT_EQ(TranslateKey(VK_DOWN), static_cast<int32_t>(ostrich::Keys::OSTKEY_DOWNARROW));
    ASSERT_EQ(TranslateKey(VK_LEFT), static_cast<int32_t>(ostrich::Keys::OSTKEY_LEFTARROW));
    ASSERT_EQ(TranslateKey(VK_RIGHT), static_cast<int32_t>(ostrich::Keys::OSTKEY_RIGHTARROW));

    // canary makes no distinction between left and right shift/alt/ctrl
    ASSERT_EQ(TranslateKey(VK_SHIFT), static_cast<int32_t>(ostrich::Keys::OSTKEY_SHIFT));
    ASSERT_EQ(TranslateKey(VK_LSHIFT), static_cast<int32_t>(ostrich::Keys::OSTKEY_SHIFT));
    ASSERT_EQ(TranslateKey(VK_RSHIFT), static_cast<int32_t>(ostrich::Keys::OSTKEY_SHIFT));

    ASSERT_EQ(TranslateKey(VK_CONTROL), static_cast<int32_t>(ostrich::Keys::OSTKEY_CTRL));
    ASSERT_EQ(TranslateKey(VK_LCONTROL), static_cast<int32_t>(ostrich::Keys::OSTKEY_CTRL));
    ASSERT_EQ(TranslateKey(VK_RCONTROL), static_cast<int32_t>(ostrich::Keys::OSTKEY_CTRL));

    ASSERT_EQ(TranslateKey(VK_MENU), static_cast<int32_t>(ostrich::Keys::OSTKEY_ALT));
    ASSERT_EQ(TranslateKey(VK_LMENU), static_cast<int32_t>(ostrich::Keys::OSTKEY_ALT));
    ASSERT_EQ(TranslateKey(VK_RMENU), static_cast<int32_t>(ostrich::Keys::OSTKEY_ALT));

    ASSERT_EQ(TranslateKey(VK_SCROLL), static_cast<int32_t>(ostrich::Keys::OSTKEY_SCROLLOCK));
    ASSERT_EQ(TranslateKey(VK_PAUSE), static_cast<int32_t>(ostrich::Keys::OSTKEY_PAUSE));
    ASSERT_EQ(TranslateKey(VK_INSERT), static_cast<int32_t>(ostrich::Keys::OSTKEY_INSERT));
    ASSERT_EQ(TranslateKey(VK_DELETE), static_cast<int32_t>(ostrich::Keys::OSTKEY_DELETE));
    ASSERT_EQ(TranslateKey(VK_HOME), static_cast<int32_t>(ostrich::Keys::OSTKEY_HOME));
    ASSERT_EQ(TranslateKey(VK_END), static_cast<int32_t>(ostrich::Keys::OSTKEY_END));
    ASSERT_EQ(TranslateKey(VK_PRIOR), static_cast<int32_t>(ostrich::Keys::OSTKEY_PAGEUP));
    ASSERT_EQ(TranslateKey(VK_NEXT), static_cast<int32_t>(ostrich::Keys::OSTKEY_PAGEDOWN));

    ASSERT_EQ(TranslateKey(VK_ADD), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_PLUS));
    ASSERT_EQ(TranslateKey(VK_SUBTRACT), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_MINUS));
    ASSERT_EQ(TranslateKey(VK_MULTIPLY), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_STAR));
    ASSERT_EQ(TranslateKey(VK_DIVIDE), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_SLASH));
    ASSERT_EQ(TranslateKey(VK_DECIMAL), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_DELETE));
    ASSERT_EQ(TranslateKey(VK_NUMLOCK), static_cast<int32_t>(ostrich::Keys::OSTKEY_KEYPAD_NUMLOCK));

    ASSERT_EQ(TranslateKey(VK_OEM_1), ';');
    ASSERT_EQ(TranslateKey(VK_OEM_2), '/');
    ASSERT_EQ(TranslateKey(VK_OEM_3), '`');
    ASSERT_EQ(TranslateKey(VK_OEM_4), '[');
    ASSERT_EQ(TranslateKey(VK_OEM_5), '\\');
    ASSERT_EQ(TranslateKey(VK_OEM_6), ']');
    ASSERT_EQ(TranslateKey(VK_OEM_7), '\'');

    ASSERT_EQ(TranslateKey(VK_OEM_PLUS), '+');
    ASSERT_EQ(TranslateKey(VK_OEM_COMMA), ',');
    ASSERT_EQ(TranslateKey(VK_OEM_MINUS), '-');
    ASSERT_EQ(TranslateKey(VK_OEM_PERIOD), '.');

    // finally, check that a wacky character returns 0, and that we don't support printscreen
    ASSERT_EQ(TranslateKey(VK_SNAPSHOT), 0);
    ASSERT_EQ(TranslateKey(999), 0);
}
