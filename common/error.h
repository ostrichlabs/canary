/*
==========================================
Copyright (c) 2020 Ostrich Labs

Error and exception definitions

TODO: when a game is localized/has a string database, the error strings should be loaded from that database
==========================================
*/

#ifndef OSTRICH_ERROR_H_
#define OSTRICH_ERROR_H_

#include <utility>

namespace ostrich {

/////////////////////////////////////////////////
// base ostrich exception - thrower provides name
class Exception {
public:

    /////////////////////////////////////////////////
    // Default constructor creates an empty Exception with no explanation
    // Destructor can do nothing because the data is a simple string literal
    // Copy/move constructors/operators are default for the same reason
    Exception() noexcept : m_What(nullptr) {}
    virtual ~Exception() {}
    Exception(Exception &&) = default;
    Exception(const Exception &) = default;
    Exception &operator=(Exception &&) = default;
    Exception &operator=(const Exception &) = default;

    /////////////////////////////////////////////////
    // Regular constructor taking a string
    // Explicit to force the use of a string literal and not a C++ string that might allocate
    explicit Exception(const char *what) noexcept : m_What(what) {}

    /////////////////////////////////////////////////
    // accessor method
    /////////////////////////////////////////////////

    const char *what() const noexcept { return m_What; }

protected:

    const char *m_What;
};

/////////////////////////////////////////////////
// When a proxy - EventSender, ConsolePrinter, etc. - passed to a subsystem is invalid
class ProxyException : public Exception {
public:

    /////////////////////////////////////////////////
    // Default constructor is deleted; ProxyException is too specific
    // Destructor can do nothing because the data is a simple string literal
    // Copy/move constructors/operators are default for the same reason
    ProxyException() = delete;
    virtual ~ProxyException() {}
    ProxyException(ProxyException &&) = default;
    ProxyException(const ProxyException &) = default;
    ProxyException &operator=(ProxyException &&) = default;
    ProxyException &operator=(const ProxyException &) = default;

    /////////////////////////////////////////////////
    // Regular constructor taking a string detailing where the exception was thrown
    // Explicit to force the use of a string literal and not a C++ string that might allocate
    explicit ProxyException(const char *where) noexcept :
        Exception(u8"ProxyException"), m_Where(where) { }

    /////////////////////////////////////////////////
    // accessor method
    /////////////////////////////////////////////////

    const char *where() const noexcept { return m_Where; }

private:

    const char *m_Where;
};

/////////////////////////////////////////////////
// terminable errors during initialization - code is dependent on the subsystem
// also allows for a separate error code from, say, a function call
class InitException : public Exception {
public:

    /////////////////////////////////////////////////
    // Default constructor is deleted; InitException is too specific
    // Destructor can do nothing because the data is simple
    // Copy/move constructors/operators are default for the same reason
    InitException() = delete;
    virtual ~InitException() {}
    InitException(InitException &&) = default;
    InitException(const InitException &) = default;
    InitException &operator=(InitException &&) = default;
    InitException &operator=(const InitException &) = default;

    /////////////////////////////////////////////////
    // Regular constructor taking a string literal detailing where the exception was thrown and an error code
    InitException(const char *where, int ourcode) noexcept :
        Exception(u8"InitException"), m_Where(where), m_OurCode(ourcode), m_ReturnedCode(std::make_pair(false, 0)) {}

    /////////////////////////////////////////////////
    // Regular constructor taking a string literal detailing where the exception was thrown, an error code, and a returned code from another subsystem
    InitException(const char *where, int ourcode, std::pair<bool, int> returnedcode) noexcept :
        Exception(u8"InitException"), m_Where(where), m_OurCode(ourcode), m_ReturnedCode(returnedcode) { }

    /////////////////////////////////////////////////
    // accessor methods
    // ReturnedCode has two - one for each part of the pair
    /////////////////////////////////////////////////

    const char *where() const noexcept { return m_Where; }
    int getOurCode() const noexcept { return m_OurCode; }
    bool hasReturnedCode() const noexcept { return m_ReturnedCode.first;  }
    int getReturnedCode() const noexcept { return m_ReturnedCode.second; }

private:

    const char *m_Where;
    int m_OurCode;
    std::pair<bool, int> m_ReturnedCode;
};

}

#endif /* OSTRICH_ERROR_H_ */
