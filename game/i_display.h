/*
==========================================
Copyright (c) 2020 Ostrich Labs

Common interface for initializing and accessing the display
==========================================
*/

#ifndef I_DISPLAY_H_
#define I_DISPLAY_H_

#include "../common/console.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class IDisplay {
public:

    IDisplay() noexcept {}
    virtual ~IDisplay() {}

    virtual bool isActive() const = 0;

    virtual int Initialize(ConsolePrinter) = 0;
    virtual int Destroy() = 0;

    virtual bool SwapBuffers() = 0;

private:

    virtual int InitWindow() = 0;
    virtual int InitRenderer() = 0;
};

} // namespace ostrich

#endif /* I_DISPLAY_H_ */
