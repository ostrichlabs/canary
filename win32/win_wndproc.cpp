/*
==========================================
Copyright (c) 2020 Ostrich Labs

Windows message pump
==========================================
*/

#include "win_wndproc.h"
#include <windowsx.h>
#include "../common/error.h"
#include "../common/ost_common.h"
#include "../game/msg_info.h"
#include "../game/msg_input.h"
#include "../game/msg_system.h"

namespace {

ostrich::EventSender l_EventSender;

/////////////////////////////////////////////////
// Translates VK_ values into internal/UTF-8 equivalents
// There are some questions about international keyboards for some keys
// But I will cross that bridge when I get to it
/////////////////////////////////////////////////
int32_t TranslateKey(int32_t vkey) {

    // ASCII/UTF-8 0-9 and A-Z
    if (((vkey >= 0x30) && (vkey <= 0x39)) ||
        ((vkey >= 0x41) && (vkey <= 0x5A))) {
        return vkey;
    }

    // F keys
    if ((vkey >= VK_F1) && (vkey <= VK_F12)) {
        return (vkey + (static_cast<int32_t>(ostrich::Keys::KEY_F1) - VK_F1));
    }

    // Number keys on keypad
    if ((vkey >= VK_NUMPAD0) && (vkey <= VK_NUMPAD9)) {
        return (vkey + (static_cast<int32_t>(ostrich::Keys::KEY_KEYPAD_0) - VK_NUMPAD0));
    }

    switch (vkey) {
        // these are preset to ASCII/UTF-8 equivalents
        case VK_TAB:
        case VK_ESCAPE:
        case VK_SPACE:
        {
            return vkey;
        }
        case VK_CAPITAL:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_CAPSLOCK);
        }
        case VK_RETURN:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_ENTER);
        }
        case VK_BACK:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_BACKSPACE);
        }
    
        // Arrow keys
        case VK_UP:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_UPARROW);
        }
        case VK_LEFT:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_LEFTARROW);
        }
        case VK_DOWN:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_DOWNARROW);
        }
        case VK_RIGHT:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_RIGHTARROW);
        }
    
        // Modifier keys
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_SHIFT);
        }
        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_CTRL);
        }
        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_ALT);
        }

    // Those utility keys above the arrow keys
        case VK_PAUSE:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_PAUSE);
        }
        case VK_INSERT:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_INSERT);
        }
        case VK_DELETE:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_DELETE);
        }
        case VK_HOME:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_HOME);
        }
        case VK_END:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_END);
        }
        case VK_PRIOR:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_PAGEUP);
        }
        case VK_NEXT:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_PAGEDOWN);
        }
    
    // Keypad keys
        case VK_ADD:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_KEYPAD_PLUS);
        }
        case VK_SUBTRACT:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_KEYPAD_MINUS);
        }
        case VK_MULTIPLY:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_KEYPAD_STAR);
        }
        case VK_DIVIDE:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_KEYPAD_SLASH);
        }
        case VK_DECIMAL:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_KEYPAD_DELETE);
        }
        case VK_NUMLOCK:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_KEYPAD_NUMLOCK);
        }
    
    // Those fancy extra characters on US keyboards
        case VK_OEM_1:
        {
            return u8';';
        }
        case VK_OEM_2:
        {
            return u8'/';
        }
        case VK_OEM_3:
        {
            return u8'`';
        }
        case VK_OEM_4:
        {
            return u8'[';
        }
        case VK_OEM_5:
        {
            return u8'\\';
        }
        case VK_OEM_6:
        {
            return u8']';
        }
        case VK_OEM_7:
        {
            return u8'\'';
        }
    
    // Those fancy extra characters on every keyboard
        case VK_OEM_PLUS:
        {
            return u8'+';
        }
        case VK_OEM_COMMA:
        {
            return u8',';
        }
        case VK_OEM_MINUS:
        {
            return u8'-';
        }
        case VK_OEM_PERIOD:
        {
            return u8'.';
        }
    
        default:
        {
            return static_cast<int32_t>(ostrich::Keys::KEY_NULL);
        }
    }
}

} // anonymous namespace

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InitWndProc(ostrich::EventSender eventsender) {
    l_EventSender = eventsender;

    if (!l_EventSender.isValid())
        throw ostrich::ProxyException(u8"ostrich::InitWndProc");
}

/////////////////////////////////////////////////
// Messages handling:
//
// WM_DESTROY - this is sent after OpenGL is initialized, because we create a window to get WGL
//              so we can't really respond to this message cleanly
//
// WM_CREATE, WM_ACTIVATE, WM_MOVE all need to be implemented
/////////////////////////////////////////////////
LRESULT CALLBACK ostrich::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_QUIT:
        {
            l_EventSender.Send(ostrich::SystemMessage::Construct(ostrich::SystemMsgType::SYS_QUIT,
                OST_FUNCTION_SIGNATURE));
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEMOVE:
        {
            int32_t buttons = OST_MOUSE_NONE;

            if (wParam & MK_LBUTTON)
                buttons |= OST_MOUSE_LBUTTON;

            if (wParam & MK_RBUTTON)
                buttons |= OST_MOUSE_RBUTTON;

            if (wParam & MK_MBUTTON)
                buttons |= OST_MOUSE_MBUTTON;

            if (buttons != OST_MOUSE_NONE) {
                l_EventSender.Send(ostrich::InputMessage::ConstructMouse(buttons,
                    -1, -1, OST_FUNCTION_SIGNATURE));
            }
            break;
        }
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            l_EventSender.Send(ostrich::InputMessage::ConstructKB(::TranslateKey(static_cast<int32_t>(wParam)),
                true, OST_FUNCTION_SIGNATURE));
            break;
        }
        case WM_SYSKEYUP:
        case WM_KEYUP:
        {
            l_EventSender.Send(ostrich::InputMessage::ConstructKB(::TranslateKey(static_cast<int32_t>(wParam)),
                false, OST_FUNCTION_SIGNATURE));
            break;
        }
        //case WM_DESTROY:
        case WM_CLOSE:
        {
            ::PostQuitMessage(0);
            break;
        }
        default:
        {
            break;
        }
    }

    return ::DefWindowProcW(hWnd, message, wParam, lParam);
}
