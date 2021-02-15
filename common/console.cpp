/*
==========================================
Copyright (c) 2020 Ostrich Labs

A logging console.
==========================================
*/

#include "console.h"

#include "filesystem.h"
#include "ost_common.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Console::Initialize() {
    if (!m_DebugMessageLog.Open(u8"debug.log", ostrich::FileMode::OPEN_WRITETRUNCATE)) {
        m_MessageLog.push_back(u8"Unable to open debug.log");
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Console::Destroy() {
    this->WriteLogToFile();
    m_MessageLog.clear();
    m_DebugMessageLog.Close();
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Console::WriteMessage(const std::string &msg) {
    if (!msg.empty()) {
        m_MessageLog.push_back(msg);
        this->TrimLog();
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Console::WriteMessage(std::string_view msg) {
    if (!msg.empty()) {
        m_MessageLog.emplace_back(msg);
        this->TrimLog();
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Console::DebugMessage(const std::string &msg) {
    if (!msg.empty() && m_DebugMessageLog.isOpen()) {
        std::fstream &handle = m_DebugMessageLog.getFStream();
        handle.write(msg.c_str(), msg.length());
        handle.put(ost_char::g_NewLine);
        handle.flush();
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Console::DebugMessage(std::string_view msg) {
    if (!msg.empty() && m_DebugMessageLog.isOpen()) {
        std::fstream &handle = m_DebugMessageLog.getFStream();
        handle.write(msg.data(), msg.length());
        handle.put(ost_char::g_NewLine);
        handle.flush();
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Console::WriteLogToFile() {
    if (m_MessageLog.size() > 0) {
        ostrich::File logfile;
        if (!logfile.Open(u8"console.log", ostrich::FileMode::OPEN_WRITETRUNCATE)) {
            return;
        }

        std::fstream &handle = logfile.getFStream();
        for (auto itr = m_MessageLog.begin(); itr != m_MessageLog.end(); std::advance(itr, 1)) {
            handle.write((*itr).c_str(), (*itr).length());
            handle.put(ost_char::g_NewLine);
        }
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::ConsolePrinter ostrich::Console::CreatePrinter() noexcept {
    return ostrich::ConsolePrinter(this);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Console::TrimLog() {
    const uint32_t maxlogsize = INT32_MAX;
    if (m_MessageLog.size() >= maxlogsize)
        m_MessageLog.pop_front();
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::ConsolePrinter::WriteMessage(std::string_view msg, std::initializer_list<std::string> args) {
    if (m_Parent) {
        std::string buffer;
        this->CompileBuffer(buffer, msg, args);
        this->WriteMessage(buffer);
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::ConsolePrinter::DebugMessage(std::string_view msg, std::initializer_list<std::string> args) {
    if (m_Parent) {
        std::string buffer;
        this->CompileBuffer(buffer, msg, args);
        this->DebugMessage(buffer);
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::ConsolePrinter::CompileBuffer(std::string &target, std::string_view msg, std::initializer_list<std::string> args) {
    std::string buffer;
    auto listitr = args.begin();
    auto msgitr = msg.begin();
    while (msgitr != msg.end()) {
        if ((*msgitr) == '%') {
            if (std::next(msgitr) != msg.end() && ((*std::next(msgitr)) == '%')) {
                buffer.push_back('%');
                std::advance(msgitr, 1);
            }
            else if (listitr == args.end()) { // if more % than args, append %
                buffer.push_back('%');
            }
            else {
                buffer += *listitr;
                std::advance(listitr, 1);
            }
        }
        else {
            buffer.push_back((*msgitr));
        }
        if (msgitr != msg.end())
            std::advance(msgitr, 1);
    }

    target.assign(buffer);
}
