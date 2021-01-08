/*
==========================================
Copyright (c) 2020 Ostrich Labs

Common interface for initializing and accessing the display
==========================================
*/

#ifndef OSTRICH_I_DISPLAY_H_
#define OSTRICH_I_DISPLAY_H_

#include "../common/console.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class IDisplay {
public:

    /////////////////////////////////////////////////
    // Empty constructor and destructor; interface has no data.
    // Copy/move constructors/operators are implicitly defaulted. ... that's probably fine for this yeah? (TODO: add them anyway)
    IDisplay() noexcept {}
    virtual ~IDisplay() {}

    /////////////////////////////////////////////////
    // Initialize the display.
    // Actual implementation depends on the platform, but typically each platform only has one windowing system.
    //
    // in:
    //      consoleprinter - an initialized ConsolePrinter for logging
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    virtual int Initialize(ConsolePrinter consoleprinter) = 0;

    /////////////////////////////////////////////////
    // Cleans up any allocated data or created objects as required by the display.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    virtual int Destroy() = 0;

    /////////////////////////////////////////////////
    // Check if the object is valid (by checking the m_isActive flag).
    // I suppose I could have made the m_isActive flag a part of the interface, but I didn't (TODO: should I?)
    //
    // returns:
    //      m_isActive flag
    virtual bool isActive() const = 0;

    /////////////////////////////////////////////////
    // Call the display's swap buffer function.
    //
    // returns:
    //      true/false if operation was successful
    virtual bool SwapBuffers() = 0;

protected:

    /////////////////////////////////////////////////
    // Helper method to initialize the platform's window.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    virtual int InitWindow() = 0;

    /////////////////////////////////////////////////
    // Helper method to initialize the platform's renderer.
    // GL4/ES2 (and probably DX11) are tied to the window they render to, so there's some work to do in IDisplay.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    virtual int InitRenderer() = 0;
};

} // namespace ostrich

#endif /* OSTRICH_I_DISPLAY_H_ */
