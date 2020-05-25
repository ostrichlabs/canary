/*
==========================================
Copyright (c) 2020 Ostrich Labs

Interface for retrieving input information from Windows
==========================================
*/

#include "win_input.h"
#include <Windows.h>
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
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_XBUTTONDOWN:
            case WM_XBUTTONUP:
                m_EventSender.Send(ostrich::SystemMessage::Construct(ostrich::SystemMsgType::SYS_QUIT,
                    m_Classname));
                break;
            default:
                break;
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