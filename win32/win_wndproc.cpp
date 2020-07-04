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
#include "../game/keydef.h"

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
        return static_cast<int32_t>(vkey);
    }

    // F keys
    if ((vkey >= VK_F1) && (vkey <= VK_F12)) {
        return static_cast<int32_t>((static_cast<int32_t>(vkey) + (ostrich::KeyToInt32(ostrich::Keys::OSTKEY_F1) - VK_F1)));
    }

    // Number keys on keypad
    if ((vkey >= VK_NUMPAD0) && (vkey <= VK_NUMPAD9)) {
        return static_cast<int32_t>((vkey + (ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_0) - VK_NUMPAD0)));
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
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_CAPSLOCK);
        }
        case VK_RETURN:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_ENTER);
        }
        case VK_BACK:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_BACKSPACE);
        }
    
        // Arrow keys
        case VK_UP:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_UPARROW);
        }
        case VK_LEFT:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_LEFTARROW);
        }
        case VK_DOWN:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_DOWNARROW);
        }
        case VK_RIGHT:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_RIGHTARROW);
        }
    
        // Modifier keys
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_SHIFT);
        }
        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_CTRL);
        }
        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_ALT);
        }

    // Those utility keys above the arrow keys
        case VK_PAUSE:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_PAUSE);
        }
        case VK_INSERT:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_INSERT);
        }
        case VK_DELETE:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_DELETE);
        }
        case VK_HOME:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_HOME);
        }
        case VK_END:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_END);
        }
        case VK_PRIOR:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_PAGEUP);
        }
        case VK_NEXT:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_PAGEDOWN);
        }
    
    // Keypad keys
        case VK_ADD:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_PLUS);
        }
        case VK_SUBTRACT:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_MINUS);
        }
        case VK_MULTIPLY:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_STAR);
        }
        case VK_DIVIDE:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_SLASH);
        }
        case VK_DECIMAL:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_DELETE);
        }
        case VK_NUMLOCK:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_NUMLOCK);
        }
        case VK_SCROLL:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_SCROLLOCK);
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
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_NULL);
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
            l_EventSender.Send(ostrich::Message::CreateSystemMessage(1, OST_FUNCTION_SIGNATURE));
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
            int32_t buttons = ostrich::Message::MOUSE_NONE;

            if (wParam & MK_LBUTTON)
                buttons |= ostrich::Message::MOUSE_LBUTTON;

            if (wParam & MK_RBUTTON)
                buttons |= ostrich::Message::MOUSE_RBUTTON;

            if (wParam & MK_MBUTTON)
                buttons |= ostrich::Message::MOUSE_MBUTTON;

            if (buttons != ostrich::Message::MOUSE_NONE) {
                l_EventSender.Send(ostrich::Message::CreateButtonMessage(buttons, OST_FUNCTION_SIGNATURE));
            }
            break;
        }
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            int32_t vkey = ::TranslateKey(static_cast<int32_t>(wParam));
            l_EventSender.Send(ostrich::Message::CreateKeyMessage(vkey, true, OST_FUNCTION_SIGNATURE));
            break;
        }
        case WM_SYSKEYUP:
        case WM_KEYUP:
        {
            int32_t vkey = ::TranslateKey(static_cast<int32_t>(wParam));
            l_EventSender.Send(ostrich::Message::CreateKeyMessage(vkey, false, OST_FUNCTION_SIGNATURE));
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
