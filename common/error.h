/*
==========================================
Copyright (c) 2020 Ostrich Labs

Error and exception definitions

TODO: when a game is localized/has a string database, the error strings should be loaded from that database
==========================================
*/

#ifndef ERROR_H_
#define ERROR_H_

#include <utility>

namespace ostrich {

/////////////////////////////////////////////////
// base ostrich exception - thrower provides name
class Exception {
public:

    Exception() noexcept : m_What(nullptr) {}
    virtual ~Exception() {}
    Exception(Exception &&) = default;
    Exception(const Exception &) = default;
    Exception &operator=(Exception &&) = default;
    Exception &operator=(const Exception &) = default;

    explicit Exception(const char *what) noexcept : m_What(what) {}

    const char *what() const noexcept { return m_What; }

protected:

    const char *m_What;
};

/////////////////////////////////////////////////
// When a proxy - EventSender, ConsolePrinter, etc. - passed to a subsystem is invalid
class ProxyException : public Exception {
public:

    ProxyException() = delete;
    virtual ~ProxyException() {}
    ProxyException(ProxyException &&) = default;
    ProxyException(const ProxyException &) = default;
    ProxyException &operator=(ProxyException &&) = default;
    ProxyException &operator=(const ProxyException &) = default;

    explicit ProxyException(const char *where) noexcept :
        Exception(u8"ProxyException"), m_Where(where) { }

    const char *where() const noexcept { return m_Where; }

private:

    const char *m_Where;
};

/////////////////////////////////////////////////
// terminable errors during initialization - code is dependent on the subsystem
// also allows for a separate error code from, say, a function call
class InitException : public Exception {
public:

    InitException() = delete;
    virtual ~InitException() {}
    InitException(InitException &&) = default;
    InitException(const InitException &) = default;
    InitException &operator=(InitException &&) = default;
    InitException &operator=(const InitException &) = default;

    InitException(const char *where, int ourcode) noexcept :
        Exception(u8"InitException"), m_Where(where), m_OurCode(ourcode), m_ReturnedCode(std::make_pair(false, 0)) {}
    InitException(const char *where, int ourcode, std::pair<bool, int> returnedcode) noexcept :
        Exception(u8"InitException"), m_Where(where), m_OurCode(ourcode), m_ReturnedCode(returnedcode) { }

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

#endif /* ERROR_H_ */
