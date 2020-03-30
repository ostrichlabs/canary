/*
==========================================
win_input.cpp

Copyright (c) 2020 Ostrich Labs

Interface for retrieving input information from Windows
==========================================
*/

#include "win_input.h"
#include <Windows.h>
#include "../common/error.h"

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
    MSG msg = { };

    while (::PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE | PM_QS_INPUT) > 0) {
        m_EventSender.Push(ostrich::Message(ostrich::MessageType::MSG_DEBUG,
            ostrich::SubMessageType::MSG_DEBUG_FROMPROCESSKBM, msg.message, m_Classname));
        if (msg.message == WM_CHAR) {
            
        }
        switch (msg.message) {
            case WM_QUIT:
                m_EventSender.Push(ostrich::Message(ostrich::MessageType::MSG_SYSTEM, ostrich::SubMessageType::MSG_SYS_QUIT, 0, m_Classname));
                break;
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_XBUTTONDOWN:
            case WM_XBUTTONUP:
            default:
                break;
        }
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputWindows::ProcessOSMessages() {
    MSG msg = { };

// TODO: add custom message processing for certain messages
// leave the traditional loop for things I don't care about
    while (::PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE | PM_QS_POSTMESSAGE) > 0) {
        if (msg.message != WM_TIMER) {
            m_EventSender.Push(ostrich::Message(ostrich::MessageType::MSG_DEBUG,
                ostrich::SubMessageType::MSG_DEBUG_FROMPROCESSOSMSG, msg.message, m_Classname));
        }
        if (msg.message == WM_KEYDOWN) {
            m_EventSender.Push(ostrich::Message(ostrich::MessageType::MSG_SYSTEM, ostrich::SubMessageType::MSG_SYS_QUIT, 0, m_Classname));
        }
        ::TranslateMessage(&msg);
        ::DispatchMessageW(&msg);
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