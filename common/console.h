/*
==========================================
Copyright (c) 2020 Ostrich Labs

A logging console

Strings are expected to be UTF-8 encoded

Future versions for different projects may also allow simple scripting
==========================================
*/

#ifndef OSTRICH_CONSOLE_H_
#define OSTRICH_CONSOLE_H_

#include <list>
#include <string>
#include <string_view>
#include "filesystem.h"
#include "utility.h"

namespace ostrich {

/////////////////////////////////////////////////
// forward declarations
class ConsolePrinter;

/////////////////////////////////////////////////
// For now, a simple logger type
// Currently uses console.log and debug.log hardcoded file names
class Console {
public:

    /////////////////////////////////////////////////
    // Constructor is effectively default; all data has default constructors
    // Destructor can do nothing because all data is self-destructible (no allocation within class itself)
    // Copy/move constructors/operators are deleted for performance reasons
    Console() noexcept {}
    virtual ~Console() {}
    Console(Console &&) = delete;
    Console(const Console &) = delete;
    Console &operator=(Console &&) = delete;
    Console &operator=(const Console &) = delete;

    /////////////////////////////////////////////////
    // Initialize the object
    // Any open files or data will be destroyed first
    //
    // returns:
    //      void
    void Initialize();

    /////////////////////////////////////////////////
    // Clean up the object
    // The message log is written to a file and the debug log is closed
    //
    // returns:
    //      void
    void Destroy();

    /////////////////////////////////////////////////
    // Write (copy) a message to the main message log
    // This version takes a const reference to a C++ string
    //
    // Simple strings get passed straight to the internal Console
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the message log
    // returns:
    //      void
    void WriteMessage(const std::string &msg);

    /////////////////////////////////////////////////
    // Write (copy) a message to the main message log
    // This version takes a string_view
    //
    // Simple strings get passed straight to the internal Console
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the message log
    // returns:
    //      void
    void WriteMessage(std::string_view msg);

    /////////////////////////////////////////////////
    // Write (copy) a message to the main message log
    // This version takes a string literal or C-style string
    //
    // Simple strings get passed straight to the internal Console
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the message log
    // returns:
    //      void
    void WriteMessage(const char *msg) { this->WriteMessage(std::string_view(msg)); }

    /////////////////////////////////////////////////
    // Write a message to the debug log file
    // This version takes a const reference to a C++ string
    //
    // The debug log is an open file, so messages are written as they come in rather than all at once at the end
    //
    // Simple strings get passed straight to the internal Console
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the debug log
    // returns:
    //      void
    void DebugMessage(const std::string &msg);

    /////////////////////////////////////////////////
    // Write a message to the debug log file
    // This version takes a string_view
    //
    // The debug log is an open file, so messages are written as they come in rather than all at once at the end
    //
    // Simple strings get passed straight to the internal Console
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the debug log
    // returns:
    //      void
    void DebugMessage(std::string_view msg);

    /////////////////////////////////////////////////
    // Write a message to the debug log file
    // This version takes a string literal or C-style string
    //
    // The debug log is an open file, so messages are written as they come in rather than all at once at the end
    //
    // Simple strings get passed straight to the internal Console
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the debug log
    // returns:
    //      void
    void DebugMessage(const char *msg) { this->DebugMessage(std::string_view(msg)); }

    /////////////////////////////////////////////////
    // Force the console to write its contents to a file
    // Does not clear the message log afterwards
    //
    // returns:
    //      void
    void WriteLogToFile();

    /////////////////////////////////////////////////
    // Create a ConsolePrinter object using this
    //
    // returns:
    //      A ConsolePrinter object containing a pointer to the current object
    ConsolePrinter CreatePrinter() noexcept;

private:

    /////////////////////////////////////////////////
    // Trims entries off the end of the message log to enforce log size limits
    // Currently uses a hardcoded limit of INT32_MAX; future versions might allow smaller limits
    //
    // returns:
    //      void
    void TrimLog();

    std::list<std::string> m_MessageLog;
    ostrich::File m_DebugMessageLog;
};

/////////////////////////////////////////////////
// A proxy class only exposing WriteMessage() methods of a Console
// Has some extra methods to allow printf-like insertion of variables into strings
// C++20 has <format> that makes all of those methods unnecessary, but no compiler supports it as of Dec 2020, sooooooooooo
class ConsolePrinter {
public:

    /////////////////////////////////////////////////
    // Default constructor creates a broken object
    // Destructor can do nothing because data is not owned by the object
    // Copy/move constructors/operators are default; there's no harm in allowing it
    ConsolePrinter() noexcept : m_Parent(nullptr) {}
    virtual ~ConsolePrinter() {}
    ConsolePrinter(ConsolePrinter &&) = default;
    ConsolePrinter(const ConsolePrinter &) = default;
    ConsolePrinter &operator=(ConsolePrinter &&) = default;
    ConsolePrinter &operator=(const ConsolePrinter &) = default;

    /////////////////////////////////////////////////
    // Explicit constructor attaching a Console
    explicit ConsolePrinter(Console *parent) noexcept : m_Parent(parent) {}

    /////////////////////////////////////////////////
    // Mimics the explicit constructor
    // Allows changing the target of the object - though I don't precisely know why you'd do this
    //
    // in:
    //      parent - A pointer to a Console object
    // returns:
    //      true/false depending on whether the passed pointer is null
    bool AttachParent(Console *parent) noexcept { m_Parent = parent; return this->isValid(); }

    /////////////////////////////////////////////////
    // Check if the object has a valid Console pointer
    //
    // returns:
    //      true/false depending on whether the Console pointer is null
    bool isValid() const noexcept { if (m_Parent) return true; return false; }

    /////////////////////////////////////////////////
    // Write (copy) a message to the main message log
    // This version takes a const reference to a C++ string
    //
    // Simple strings get passed straight to the internal Console
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the message log
    // returns:
    //      void
    void WriteMessage(const std::string &msg) { if (m_Parent) m_Parent->WriteMessage(msg); }

    /////////////////////////////////////////////////
    // Write (copy) a message to the main message log
    // This version takes a string_view
    //
    // Simple strings get passed straight to the internal Console
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the message log
    // returns:
    //      void
    void WriteMessage(std::string_view msg) { if (m_Parent) m_Parent->WriteMessage(msg); }

    /////////////////////////////////////////////////
    // Write (copy) a message to the main message log
    // This version takes a string literal or C-style string
    //
    // Simple strings get passed straight to the internal Console
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the message log
    // returns:
    //      void
    void WriteMessage(const char *msg) { if (m_Parent) m_Parent->WriteMessage(msg); }

    /////////////////////////////////////////////////
    // Write a message to the debug log file
    // This version takes a const reference to a C++ string
    //
    // The debug log is an open file, so messages are written as they come in rather than all at once at the end
    //
    // Simple strings get passed straight to the internal Console
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the debug log
    // returns:
    //      void
    void DebugMessage(const std::string &msg) { if (m_Parent) m_Parent->DebugMessage(msg); }

    /////////////////////////////////////////////////
    // Write a message to the debug log file
    // This version takes a string_view
    //
    // The debug log is an open file, so messages are written as they come in rather than all at once at the end
    //
    // Simple strings get passed straight to the internal Console
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the debug log
    // returns:
    //      void
    void DebugMessage(std::string_view msg) { if (m_Parent) m_Parent->DebugMessage(msg); }

    /////////////////////////////////////////////////
    // Write a message to the debug log file
    // This version takes a string literal or C-style string
    //
    // The debug log is an open file, so messages are written as they come in rather than all at once at the end
    //
    // Simple strings get passed straight to the internal Console
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the debug log
    // returns:
    //      void
    void DebugMessage(const char *msg) { if (m_Parent) m_Parent->DebugMessage(msg); }

    /////////////////////////////////////////////////
    // Write (copy) a message to the main message log
    // This version takes a const reference to a C++ string
    //
    // Allows passing extra data in a {} enclosed list; all data must be strings or converted to strings at the time of the call
    // Each % is replaced with a string from the argument list, in order (to insert a %, put %%)
    // If the list is empty when a % is found, leave it as-is
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the message log
    //      args - A list of strings to use as arguments in the message
    // returns:
    //      void
    void WriteMessage(std::string_view msg, std::initializer_list<std::string> args);

    /////////////////////////////////////////////////
    // Write (copy) a message to the main message log
    // This version takes a string_view
    //
    // Allows passing extra data in a {} enclosed list; all data must be strings or converted to strings at the time of the call
    // Each % is replaced with a string from the argument list, in order (to insert a %, put %%)
    // If the list is empty when a % is found, leave it as-is
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the message log
    //      args - A list of strings to use as arguments in the message
    // returns:
    //      void
    void WriteMessage(const std::string &msg, std::initializer_list<std::string> args) { this->WriteMessage(std::string_view(msg), args); }

    /////////////////////////////////////////////////
    // Write (copy) a message to the main message log
    // This version takes a string literal or C-style string
    //
    // Allows passing extra data in a {} enclosed list; all data must be strings or converted to strings at the time of the call
    // Each % is replaced with a string from the argument list, in order (to insert a %, put %%)
    // If the list is empty when a % is found, leave it as-is
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the message log
    //      args - A list of strings to use as arguments in the message
    // returns:
    //      void
    void WriteMessage(const char *msg, std::initializer_list<std::string> args) { this->WriteMessage(std::string_view(msg), args); }

    /////////////////////////////////////////////////
    // Write a message to the debug log file
    // This version takes a const reference to a C++ string
    //
    // The debug log is an open file, so messages are written as they come in rather than all at once at the end
    //
    // Allows passing extra data in a {} enclosed list; all data must be strings or converted to strings at the time of the call
    // Each % is replaced with a string from the argument list, in order (to insert a %, put %%)
    // If the list is empty when a % is found, leave it as-is
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the message log
    //      args - A list of strings to use as arguments in the message
    // returns:
    //      void
    void DebugMessage(std::string_view msg, std::initializer_list<std::string> args);

    /////////////////////////////////////////////////
    // Write a message to the debug log file
    // This version takes a string_view
    //
    // The debug log is an open file, so messages are written as they come in rather than all at once at the end
    //
    // Allows passing extra data in a {} enclosed list; all data must be strings or converted to strings at the time of the call
    // Each % is replaced with a string from the argument list, in order (to insert a %, put %%)
    // If the list is empty when a % is found, leave it as-is
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the message log
    //      args - A list of strings to use as arguments in the message
    // returns:
    //      void
    void DebugMessage(const std::string &msg, std::initializer_list<std::string> args) { this->DebugMessage(std::string_view(msg), args); }

    /////////////////////////////////////////////////
    // Write a message to the debug log file
    // This version takes a string literal or C-style string
    //
    // The debug log is an open file, so messages are written as they come in rather than all at once at the end
    //
    // Allows passing extra data in a {} enclosed list; all data must be strings or converted to strings at the time of the call
    // Each % is replaced with a string from the argument list, in order (to insert a %, put %%)
    // If the list is empty when a % is found, leave it as-is
    //
    // in:
    //      msg - The UTF-8 formatted string to write to the message log
    //      args - A list of strings to use as arguments in the message
    // returns:
    //      void
    void DebugMessage(const char *msg, std::initializer_list<std::string> args) { this->DebugMessage(std::string_view(msg), args); }

private:

    /////////////////////////////////////////////////
    // Replace % in message with provided arguments
    //
    // in:
    //      msg - original unparsed message
    //      args - A list of strings to use as arguments in the message
    // out:
    //      target - a newly-constructed C++ string containing the fully parsed message
    void CompileBuffer(std::string &target, std::string_view msg, std::initializer_list<std::string> args);

    Console *m_Parent;
};

} // namespace ostrich

#endif /* OSTRICH_CONSOLE_H_ */
