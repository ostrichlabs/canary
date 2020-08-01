/*
==========================================
Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "../common/ost_common.h"

#if (OST_LINUX != 1)
#    error "This module should only be included in Linux builds using X11"
#endif

#include "x11_input.h"
#include <GL/glx.h>
#include "../common/error.h"
#include "../game/errorcodes.h"
#include "../game/keydef.h"
#include "../game/message.h"

volatile int ostrich::InputX11::ms_LastRaisedSignal = 0;
::siginfo_t *ostrich::InputX11::ms_SignalInfo = nullptr;

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int32_t ostrich::x11::TranslateKey(XKeyEvent *ev) {
    int vkey = ::XLookupKeysym(ev, 0); // TODO: maybe use XLookupString later for debugging?

    if (vkey == NoSymbol)
        return 0;

    if (vkey == XK_KP_Space)
        return ' ';

    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::InputX11::Initialize(ostrich::ConsolePrinter consoleprinter, ostrich::EventSender eventsender) {
    if (this->isActive()) {
        return OST_ERROR_ISACTIVE;
    }

    m_ConsolePrinter = consoleprinter;
    m_EventSender = eventsender;
    if ((!m_ConsolePrinter.isValid()) || (!m_EventSender.isValid()))
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    m_Display = ::glXGetCurrentDisplay();
    if (m_Display == nullptr) {
        return OST_ERROR_GLXGETDISPLAY;
    }

    m_isActive = true;
    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputX11::Destroy() {
    if (m_isActive) {
        m_EventSender.AttachParent(nullptr);
        m_ConsolePrinter.AttachParent(nullptr);
        m_isActive = false;
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputX11::ProcessKBM() {
    if (!this->isActive()) {
        return;
    }

    XEvent event = {};
    while(::XPending(m_Display) > 0) {
        ::XNextEvent(m_Display, &event);
        switch (event.type) {
            case KeyPress:
            {
                int32_t vkey = ostrich::x11::TranslateKey(&event.xkey);
                m_EventSender.Send(ostrich::Message::CreateKeyMessage(vkey, true, OST_FUNCTION_SIGNATURE));
                break;
            }
            case KeyRelease:
            {
                int32_t vkey = ostrich::x11::TranslateKey(&event.xkey);
                m_EventSender.Send(ostrich::Message::CreateKeyMessage(vkey, false, OST_FUNCTION_SIGNATURE));
                break;
            }
            case ButtonPress:
            {
                break;
            }
            case ButtonRelease:
            {
                break;
            }
            case MotionNotify:
            {
                break;
            }
            case VisibilityNotify:
            {
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputX11::ProcessOSMessages() {
    if (InputX11::ms_LastRaisedSignal != 0) {
        int32_t code = 0; // TODO: reserved for when system messages get their own defines
        if (InputX11::ms_SignalInfo) {
            code = InputX11::ms_SignalInfo->si_code;
        }
        m_EventSender.Send(ostrich::Message::CreateSystemMessage(OST_SYSTEMMSG_SIGNAL,
            InputX11::ms_LastRaisedSignal, OST_FUNCTION_SIGNATURE));
        InputX11::ms_LastRaisedSignal = 0;
        InputX11::ms_SignalInfo = nullptr;
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::InputX11::InitializeSignalHandler() {
    InputX11::ms_LastRaisedSignal = 0;
    InputX11::ms_SignalInfo = nullptr;

    struct sigaction action = { }, oldaction = { };
    action.sa_sigaction = ostrich::InputX11::SignalHandler;
    ::sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;

    ::sigaction(SIGINT, nullptr, &oldaction);
    if (oldaction.sa_handler != SIG_IGN)
        ::sigaction(SIGINT, &action, nullptr);
    else
        return OST_ERROR_HANDLERSIGINT;

    ::sigaction(SIGTERM, nullptr, &oldaction);
    if (oldaction.sa_handler != SIG_IGN)
        ::sigaction(SIGTERM, &action, nullptr);
    else
        return OST_ERROR_HANDLERSIGTERM;

    ::sigaction(SIGHUP, nullptr, &oldaction);
    if (oldaction.sa_handler != SIG_IGN)
        ::sigaction(SIGHUP, &action, nullptr);
    else
        return OST_ERROR_HANDLERSIGHUP;

    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputX11::SignalHandler(int signum, siginfo_t *info, void *ucontext) {
    InputX11::ms_LastRaisedSignal = signum;
    InputX11::ms_SignalInfo = info;
}
