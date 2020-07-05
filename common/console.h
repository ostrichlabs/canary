/*
==========================================
Copyright (c) 2020 Ostrich Labs

A logging console

Future versions for different projects may also allow runtime commands and cvars

The main console can create ConsolePrinter objects - these can take multiple string arguments and insert them into another
==========================================
*/

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <array>
#include <fstream>
#include <list>
#include <string>
#include <string_view>
#include "utility.h"

namespace ostrich {

/////////////////////////////////////////////////
// forward declarations
class ConsolePrinter;

/////////////////////////////////////////////////
//
class Console {
public:

    Console() noexcept {}
    virtual ~Console() {}
    Console(Console &&) = delete;
    Console(const Console &) = delete;
    Console &operator=(Console &&) = delete;
    Console &operator=(const Console &) = delete;

    void Initialize();
    void Destroy();

    void WriteMessage(const std::string &msg);
    void WriteMessage(std::string_view msg);
    void WriteMessage(const char *msg) { this->WriteMessage(std::string_view(msg)); }

    void DebugMessage(const std::string &msg);
    void DebugMessage(std::string_view msg);
    void DebugMessage(const char *msg) { this->DebugMessage(std::string_view(msg)); }

    void WriteLogToFile();

    ConsolePrinter CreatePrinter() noexcept;

private:

    // called at the end of WriteMessage() to enforce log size limits
    // the debug log will have no such limits
    void TrimLog();

    std::list<std::string> m_MessageLog;
    std::fstream m_DebugMessageLog;
};

/////////////////////////////////////////////////
//
class ConsolePrinter {
public:

    ConsolePrinter() noexcept : m_Parent(nullptr) {}
    virtual ~ConsolePrinter() {}
    ConsolePrinter(ConsolePrinter &&) = default;
    ConsolePrinter(const ConsolePrinter &) = default;
    ConsolePrinter &operator=(ConsolePrinter &&) = default;
    ConsolePrinter &operator=(const ConsolePrinter &) = default;

    explicit ConsolePrinter(Console *parent) noexcept : m_Parent(parent) {}

    bool AttachParent(Console *parent) noexcept { m_Parent = parent; return this->isValid(); }

    bool isValid() const noexcept { if (m_Parent) return true; return false; }

    void WriteMessage(const std::string &msg) { if (m_Parent) m_Parent->WriteMessage(msg); }
    void WriteMessage(std::string_view msg) { if (m_Parent) m_Parent->WriteMessage(msg); }
    void WriteMessage(const char *msg) { if (m_Parent) m_Parent->WriteMessage(msg); }

    void DebugMessage(const std::string &msg) { if (m_Parent) m_Parent->DebugMessage(msg); }
    void DebugMessage(std::string_view msg) { if (m_Parent) m_Parent->DebugMessage(msg); }
    void DebugMessage(const char *msg) { if (m_Parent) m_Parent->DebugMessage(msg); }

    void WriteMessage(std::string_view msg, std::initializer_list<std::string> args);
    void WriteMessage(const std::string &msg, std::initializer_list<std::string> args) { this->WriteMessage(std::string_view(msg), args); }
    void WriteMessage(const char *msg, std::initializer_list<std::string> args) { this->WriteMessage(std::string_view(msg), args); }

    void DebugMessage(std::string_view msg, std::initializer_list<std::string> args);
    void DebugMessage(const std::string &msg, std::initializer_list<std::string> args) { this->DebugMessage(std::string_view(msg), args); }
    void DebugMessage(const char *msg, std::initializer_list<std::string> args) { this->DebugMessage(std::string_view(msg), args); }

    void CompileBuffer(std::string &target, std::string_view msg, std::initializer_list<std::string> args);

private:

    Console *m_Parent;
};

} // namespace ostrich

#endif /* OST_CONSOLE_H_ */
