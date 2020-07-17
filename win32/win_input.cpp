/*
==========================================
Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "../common/ost_common.h"

#if (OST_WINDOWS != 1)
#   error "This module should only be included in Windows builds"
#endif

#include "win_input.h"
#include <Windows.h>
#include <windowsx.h>
#include "win_wndproc.h"
#include "../common/error.h"
#include "../game/errorcodes.h"
#include "../game/message.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::InputWindows::Initialize(ostrich::ConsolePrinter consoleprinter, ostrich::EventSender eventsender) {
    if (m_isActive)
        return OST_ERROR_ISACTIVE;

    m_ConsolePrinter = consoleprinter;
    m_EventSender = eventsender;

    if ((!m_ConsolePrinter.isValid()) || (!m_EventSender.isValid()))
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    // Initialize our WndProc - just use a copy of the input event sender
    ostrich::InitWndProc(eventsender);

    // TODO: Put mouse initialization stuff in its own method?
    // We may have to redo it every time the window changes
    HWND hwnd = ::GetActiveWindow();
    ::SetCapture(hwnd);
    while (::ShowCursor(FALSE) >= 0)
        ;

    m_isActive = true;
    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
void ostrich::InputWindows::Destroy() {
    if (m_isActive) {
        ::ReleaseCapture();
        m_isActive = false;
    }
}

/////////////////////////////////////////////////
// get mouse coordinates, then translate to the window
// keyboard input is handled via the traditional message pump
/////////////////////////////////////////////////
void ostrich::InputWindows::ProcessKBM() {
    POINT point = { };

    ::GetCursorPos(&point);

    // TODO: actually translate to the window

    m_EventSender.Send(ostrich::Message::CreateMousePosMessage(point.x, point.y, m_Classname));
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputWindows::ProcessOSMessages() {
    MSG winmsg = { };

    while (::PeekMessageW(&winmsg, NULL, 0, 0, PM_NOREMOVE) > 0) {
        if (!::GetMessageW(&winmsg, NULL, 0, 0)) {
            throw ostrich::Exception(u8"Windows tricked me!");
        }

        ::TranslateMessage(&winmsg);
        ::DispatchMessageW(&winmsg);
    }
}
