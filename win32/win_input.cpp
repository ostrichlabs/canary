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
#include "../game/msg_info.h"
#include "../game/msg_input.h"
#include "../game/msg_system.h"

/////////////////////////////////////////////////
// Windows initialization is simple; it's all in the Windows API
/////////////////////////////////////////////////
int ostrich::InputWindows::Initialize(ostrich::ConsolePrinter consoleprinter, ostrich::EventSender eventsender) {
    if (m_isActive)
        return -1;

    m_ConsolePrinter = consoleprinter;
    m_EventSender = eventsender;

    if ((!m_ConsolePrinter.isValid()) || (!m_EventSender.isValid()))
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    m_isActive = true;
    return 0;
}

/////////////////////////////////////////////////
void ostrich::InputWindows::Destroy() {
    if (m_isActive)
        m_isActive = false;
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

                if (winmsg.lParam & MK_LBUTTON)
                    buttons |= OST_MOUSE_LBUTTON;

                if (winmsg.lParam & MK_RBUTTON)
                    buttons |= OST_MOUSE_RBUTTON;

                if (winmsg.lParam & MK_MBUTTON)
                    buttons |= OST_MOUSE_MBUTTON;

                m_EventSender.Send(ostrich::InputMessage::ConstructMouse(buttons,
                    GET_X_LPARAM(winmsg.lParam), GET_Y_LPARAM(winmsg.lParam), m_Classname));
                break;
            }
            case WM_SYSKEYDOWN:
            case WM_KEYDOWN:
            {
                m_EventSender.Send(ostrich::InputMessage::ConstructKB(static_cast<int32_t>(winmsg.wParam),
                    true, m_Classname));
                break;
            }
            case WM_SYSKEYUP:
            case WM_KEYUP:
            {
                m_EventSender.Send(ostrich::InputMessage::ConstructKB(static_cast<int32_t>(winmsg.wParam),
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
        if (winmsg.message != WM_TIMER) {
            m_EventSender.Send(ostrich::InfoMessage::ConstructDebugMessage(winmsg.message,
                u8"From InputWindows::ProcessOSMessages", m_Classname));
        }
        ::TranslateMessage(&winmsg);
        ::DispatchMessageW(&winmsg);
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::pair<ostrich::Button, int32_t> ostrich::InputWindows::TranslateKey(int32_t vkey) {
    switch (vkey) {
        case VK_SPACE:
        default:
            return std::pair(ostrich::Button::NONE, 0);
    }
    //return std::pair(ostrich::Button::NONE, 0);
}