/*
==========================================
console.h

Copyright (c) 2020 Ostrich Labs

A logging tool.

Future versions for different projects may also allow runtime commands and cvars
==========================================
*/

#ifndef CONSOLE_H_
#define CONSOLE_H_

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

    // not really intended for general use
    const std::list<std::string> *GetMessageLogPointer() const { return &m_MessageLog; }

    void WriteMessage(const std::string &msg);
    void WriteMessage(std::string_view msg);
    void WriteMessage(const char *msg);

    void WriteLogToFile();

    ConsolePrinter CreatePrinter() noexcept;

private:

    // called at the end of WriteMessage() to enforce log size limits
    void TrimLog();

    std::list<std::string>  m_MessageLog;
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

private:

    Console *m_Parent;

};

} // namespace ostrich

#endif /* OST_CONSOLE_H_ */
