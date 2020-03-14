/*
==========================================
input_raspi.cpp

Copyright (c) 2019 Ostrich Labs

Interface for retrieving input information from the Raspberry Pi
==========================================
*/

#include "input_raspi.h"
#include <cerrno>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <linux/kd.h>
#include <linux/keyboard.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include "ost_message.h"
#include "../system/sys_error.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::InputRaspi::Initialize(ConsolePrinter consoleprinter, EventSender eventsender) {
	if (this->isActive()) {
		return -1;
	}

	m_ConsolePrinter = consoleprinter;
	m_EventSender = eventsender;
	if ((!m_ConsolePrinter.isValid()) || (!m_EventSender.isValid()))
		throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

	// these should throw exceptions on failure; false means we are already initialized
	// if we have a keyboard and/or mouse and somehow aren't active, we have a bug
    if (!this->InitKeyboard()) {
    	return -1;
    }
    if (!this->InitMouse()) {
    	consoleprinter.WriteMessage(u8"Failed to initialize mouse; is it plugged in?");
    }

    m_isActive = true;
    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputRaspi::Destroy() {
	if (m_isActive) {
		if (m_MouseFD > 0) {
			::close(m_MouseFD);
		}
		if (m_KeyboardFD > 0) {
			::close(m_KeyboardFD);
		}
		m_MouseFD = 0;
		m_KeyboardFD = 0;
		m_EventSender.AttachParent(nullptr);
		m_ConsolePrinter.AttachParent(nullptr);
		m_isActive = false;
	}
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::InputRaspi::ProcessKBM() {
	input_event kbinput, mouseinput;
	int kbbytes = 0, mousebytes = 0;
	bool done = false;
	while (!done) {
		kbbytes = ::read(m_KeyboardFD, &kbinput, sizeof(kbinput));
		if (kbbytes > 0) {
			if (kbinput.type & EV_KEY) {
				m_EventSender.Push(ostrich::Message(ostrich::MessageType::MSG_DEBUG,
								ostrich::SubMessageType::MSG_DEBUG_FROMPROCESSKBM, kbinput.code,
								m_Classname));
			}
			// TODO: translate to internal message
			// tho for now if a key is pressed send a quit
			m_EventSender.Push(ostrich::Message(ostrich::MessageType::MSG_SYSTEM,
				ostrich::SubMessageType::MSG_SYS_QUIT, 0, m_Classname));
			done = true;
		}
		mousebytes = ::read(m_MouseFD, &mouseinput, sizeof(mouseinput));
		if (mousebytes > 0) {
			// TODO: translate to internal message
		}
		if ((kbbytes <= 0) && (mousebytes <= 0))
			done = true;
	}
}

/////////////////////////////////////////////////
// for Raspi this will be signal handling, so, nothing to do here for now
/////////////////////////////////////////////////
void ostrich::InputRaspi::ProcessOSMessages() {

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::InputRaspi::InitKeyboard() {
    if (m_KeyboardFD > 0)
        return false;

    errno = 0;
    m_KeyboardFD = ::open(u8"/dev/tty", O_RDONLY | O_NONBLOCK);
    if (m_KeyboardFD == -1)
    	throw ostrich::InitException(OST_FUNCTION_SIGNATURE, errno);

    ::ioctl(m_KeyboardFD, KDSKBMODE, K_UNICODE);

    return true;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::InputRaspi::InitMouse() {
    if (m_MouseFD > 0)
        return false;

    errno = 0;
    m_MouseFD = ::open(u8"/dev/input/mice", O_RDONLY);
    if (m_MouseFD == -1)
    	return false;

    return true;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::Button ostrich::InputRaspi::TranslateKey(int32_t vkey) {
	return ostrich::Button::NONE;
}
