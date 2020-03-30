/*
==========================================
Copyright (c) 2020 Ostrich Labs

A logging console.
==========================================
*/

#include "console.h"
#include "filesystem.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Console::Initialize() {
    // TODO: LogSizeMax and WriteLogOnExit should be hardcoded constants
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Console::Destroy() {
    this->WriteLogToFile();
    m_MessageLog.clear();
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
void ostrich::Console::WriteMessage(const char *msg) {
    if (msg != nullptr) {
        m_MessageLog.emplace_back(msg);
        this->TrimLog();
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Console::WriteLogToFile() {
    std::fstream logfile;
    ostrich::OpenFile(u8"console.log", ost_filemode::g_WRITETRUNCATE, logfile);
    if (!logfile.is_open())
        return;

    for (auto itr = m_MessageLog.begin(); itr != m_MessageLog.end(); std::advance(itr, 1)) {
        logfile.write((*itr).c_str(), (*itr).length());
        logfile.put(ost_char::g_NewLine);
    }

    logfile.close();
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::ConsolePrinter ostrich::Console::CreatePrinter() noexcept {
    return ostrich::ConsolePrinter(this);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Console::TrimLog() {
    const uint32_t maxlogsize = INT32_MAX; // TODO: when the console becomes A Console, change this
    if (m_MessageLog.size() >= maxlogsize)
        m_MessageLog.pop_front();
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::ConsolePrinter::WriteMessage(std::string_view msg, std::initializer_list<std::string> args) {
    if (m_Parent) {
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

        this->WriteMessage(buffer);
    }
}