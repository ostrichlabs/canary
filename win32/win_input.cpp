/*
==========================================
Copyright (c) 2020 Ostrich Labs
 
Interface for retrieving input information from Windows
==========================================
*/

#include "win_input.h"
#include <Windows.h>
#include <windowsx.h>
#include "../common/error.h"
#include "../game/keydef.h"
#include "../game/msg_info.h"
#include "../game/msg_input.h"
#include "../game/msg_system.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::InputWindows::Initialize(ostrich::ConsolePrinter consoleprinter, ostrich::EventSender eventsender) {
    if (m_isActive)
        return -1;

    m_ConsolePrinter = consoleprinter;
    m_EventSender = eventsender;

    if ((!m_ConsolePrinter.isValid()) || (!m_EventSender.isValid()))
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    // TODO: Put mouse initialization stuff in its own method?
    // We may have to redo it every time the window changes
    HWND hwnd = ::GetActiveWindow();
    ::SetCapture(hwnd);
    while (::ShowCursor(FALSE) >= 0)
        ;

    m_isActive = true;
    return 0;
}

/////////////////////////////////////////////////
void ostrich::InputWindows::Destroy() {
    if (m_isActive) {
        ::ReleaseCapture();
        m_isActive = false;
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputWindows::ProcessKBM() {
    MSG winmsg = { };

    while (::PeekMessageW(&winmsg, NULL, 0, 0, PM_REMOVE | PM_QS_INPUT) > 0) {
        m_EventSender.Send(ostrich::InfoMessage::ConstructDebugMessage(winmsg.message,
            u8"From InputWindows::ProcessKBM", m_Classname));
        if (winmsg.message == WM_CHAR) {
            
        }
        switch (winmsg.message) {
            case WM_QUIT:
            {
                m_EventSender.Send(ostrich::SystemMessage::Construct(ostrich::SystemMsgType::SYS_QUIT,
                    m_Classname));
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

                if (winmsg.wParam & MK_LBUTTON)
                    buttons |= OST_MOUSE_LBUTTON;

                if (winmsg.wParam & MK_RBUTTON)
                    buttons |= OST_MOUSE_RBUTTON;

                if (winmsg.wParam & MK_MBUTTON)
                    buttons |= OST_MOUSE_MBUTTON;

                m_EventSender.Send(ostrich::InputMessage::ConstructMouse(buttons,
                    GET_X_LPARAM(winmsg.lParam), GET_Y_LPARAM(winmsg.lParam), m_Classname));
                break;
            }
            case WM_SYSKEYDOWN:
            case WM_KEYDOWN:
            {
                m_EventSender.Send(ostrich::InputMessage::ConstructKB(this->TranslateKey(static_cast<int32_t>(winmsg.wParam)),
                    true, m_Classname));
                break;
            }
            case WM_SYSKEYUP:
            case WM_KEYUP:
            {
                m_EventSender.Send(ostrich::InputMessage::ConstructKB(this->TranslateKey(static_cast<int32_t>(winmsg.wParam)),
                    false, m_Classname));
                break;
            }
            default:
            {
                m_EventSender.Send(ostrich::InfoMessage::ConstructDebugMessage(winmsg.message,
                    u8"Unhandled message in ProcessKBM", m_Classname));
                break;
            }
        }
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputWindows::ProcessOSMessages() {
    MSG winmsg = { };

// TODO: add custom message processing for certain messages
// leave the traditional loop for things I don't care about
    while (::PeekMessageW(&winmsg, NULL, 0, 0, PM_REMOVE | PM_QS_POSTMESSAGE) > 0) {

    // ignore WM_TIMER; I'm not setting timers, I do not know what keeps sending them
    // and they need to fuck off
        if (winmsg.message != WM_TIMER) {
            m_EventSender.Send(ostrich::InfoMessage::ConstructDebugMessage(winmsg.message,
                u8"From InputWindows::ProcessOSMessages", m_Classname));
        }

        switch (winmsg.message) {

            case WM_TIMER: // go awaaaaaaaaaaaaaaaaay
            {
                break;
            }
            default:
            {
                ::TranslateMessage(&winmsg);
                ::DispatchMessageW(&winmsg);
            }
        }
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int32_t ostrich::InputWindows::TranslateKey(int32_t vkey) {

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