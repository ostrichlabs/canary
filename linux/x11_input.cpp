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
KeySym ostrich::x11::GetVKey(XKeyEvent *ev) {
    return ::XLookupKeysym(ev, 0); // TODO: maybe use XLookupString later for debugging?;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int32_t ostrich::x11::TranslateKey(KeySym vkey) {

    // ranges of XKs that map directly to ASCII/UTF-8
    if (((vkey >= XK_0) && (vkey <= XK_9)) ||
        ((vkey >= XK_A) && (vkey <= XK_Z)) ||
        ((vkey >= XK_a) && (vkey <= XK_z))) {
        if ((vkey >= XK_a) && (vkey <= XK_z)) {
            return (vkey - 32); // return capital letters
        }
        else {
            return vkey;
        }
    }

    // F keys
    if ((vkey >= XK_F1) && (vkey <= XK_F12)) {
        return static_cast<int32_t>((static_cast<int32_t>(vkey) + (ostrich::KeyToInt32(ostrich::Keys::OSTKEY_F1) - XK_F1)));
    }

    // Number keys on keypad
    if ((vkey >= XK_KP_0) && (vkey <= XK_KP_9)) {
        return static_cast<int32_t>((static_cast<int32_t>(vkey) + (ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_0) - XK_KP_0)));
    }

    switch (vkey) {
        case XK_space:
        {
            return u8' ';
        }
        case XK_Escape:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_ESCAPE);
        }
        case XK_Tab:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_TAB);
        }
        case XK_Caps_Lock:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_CAPSLOCK);
        }
        case XK_Return:
        case XK_KP_Enter:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_ENTER);
        }
        case XK_BackSpace:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_BACKSPACE);
        }

        // Arrow keys
        case XK_Up:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_UPARROW);
        }
        case XK_Left:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_LEFTARROW);
        }
        case XK_Down:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_DOWNARROW);
        }
        case XK_Right:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_RIGHTARROW);
        }

        // Modifier keys
        case XK_Shift_L:
        case XK_Shift_R:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_SHIFT);
        }
        case XK_Control_L:
        case XK_Control_R:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_CTRL);
        }
        case XK_Alt_L:
        case XK_Alt_R:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_ALT);
        }

        // Those utility keys above the arrow keys
        case XK_Scroll_Lock:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_SCROLLOCK);
        }
        case XK_Pause:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_PAUSE);
        }
        case XK_Insert:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_INSERT);
        }
        case XK_Delete:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_DELETE);
        }
        case XK_Home:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_HOME);
        }
        case XK_End:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_END);
        }
        case XK_Page_Up:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_PAGEUP);
        }
        case XK_Page_Down:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_PAGEDOWN);
        }

        // Keypad keys
        case XK_KP_Add:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_PLUS);
        }
        case XK_KP_Subtract:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_MINUS);
        }
        case XK_KP_Multiply:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_STAR);
        }
        case XK_KP_Divide:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_SLASH);
        }
        case XK_KP_Decimal:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_DELETE);
        }
        case XK_Num_Lock:
        {
            return ostrich::KeyToInt32(ostrich::Keys::OSTKEY_KEYPAD_NUMLOCK);
        }

        // Those fancy extra characters on US keyboards
        case XK_semicolon:
        {
            return u8';';
        }
        case XK_slash:
        {
            return u8'/';
        }
        case XK_grave:
        {
            return u8'`';
        }
        case XK_bracketleft:
        {
            return u8'[';
        }
        case XK_backslash:
        {
            return u8'\\';
        }
        case XK_bracketright:
        {
            return u8']';
        }
        case XK_apostrophe:
        {
            return u8'\'';
        }

        // Those fancy extra characters on every keyboard
        case XK_equal:
        {
            return u8'=';
        }
        case XK_comma:
        {
            return u8',';
        }
        case XK_minus:
        {
            return u8'-';
        }
        case XK_period:
        {
            return u8'.';
        }

        default:
        {
            return static_cast<int32_t>(ostrich::Keys::OSTKEY_NULL);
        }
    }
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
    KeySym sym = 0;
    int32_t vkey = 0;
    while(::XPending(m_Display) > 0) {
        ::XNextEvent(m_Display, &event);
        switch (event.type) {
            case KeyPress:
            {
                sym = ostrich::x11::GetVKey(&event.xkey);
                vkey = ostrich::x11::TranslateKey(sym);
                if (vkey == 0) {
                    m_ConsolePrinter.DebugMessage(u8"Unknown vkey >%<", { std::to_string(sym) });
                }
                else {
                    m_EventSender.Send(ostrich::Message::CreateKeyMessage(vkey, true, OST_FUNCTION_SIGNATURE));
                }
                break;
            }
            case KeyRelease:
            {
                sym = ostrich::x11::GetVKey(&event.xkey);
                vkey = ostrich::x11::TranslateKey(sym);
                if (vkey == 0) {
                    m_ConsolePrinter.DebugMessage(u8"Unknown vkey >%<", { std::to_string(sym) });
                }
                else {
                    m_EventSender.Send(ostrich::Message::CreateKeyMessage(vkey, false, OST_FUNCTION_SIGNATURE));
                }
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
