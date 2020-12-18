/*
==========================================
Copyright (c) 2020 Ostrich Labs

Interface for retrieving input information from the OS
Translates input to standard Ostrich format and adds to EventQueue

Input can be Keyboard/Mouse (KBM) or OS messages
==========================================
*/

#ifndef OSTRICH_I_INPUT_H_
#define OSTRICH_I_INPUT_H_

#include "eventqueue.h"
#include "../common/console.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class IInput {
public:

    /////////////////////////////////////////////////
    // Empty constructor and destructor; interface has no data.
    // Copy/move constructors/operators are implicitly defaulted. ... that's probably fine for this yeah? (TODO: add them anyway)
    IInput() noexcept {}
    virtual ~IInput() {}

    /////////////////////////////////////////////////
    // Initialize the input handler.
    // Actual implementation depends on the platform.
    //
    // in:
    //      consoleprinter - an initialized ConsolePrinter for logging
    //      eventsender - an initialized EventSender for passing input data to the queue
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    virtual int Initialize(ConsolePrinter consoleprinter, EventSender eventsender) = 0;

    /////////////////////////////////////////////////
    // Cleans up any allocated data or created objects as required by the input handler.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    virtual void Destroy() = 0;

    /////////////////////////////////////////////////
    // Check if the object is valid (by checking the m_isActive flag).
    // I suppose I could have made the m_isActive flag a part of the interface, but I didn't (TODO: should I?)
    //
    // returns:
    //      m_isActive flag
    virtual bool isActive() const noexcept = 0;

    /////////////////////////////////////////////////
    // Process any keyboard/mouse input.
    // This may include joystick/controller input depending on platform.
    // Have a feeling I will regret the name of this method.
    //
    // returns:
    //      void
    virtual void ProcessKBM() = 0;

    /////////////////////////////////////////////////
    // Process any miscellaneous messages from the OS.
    // Processed separately from user input to maintain the latter's higher priority; some platforms may treat both the same.
    //
    // returns:
    //      void
    virtual void ProcessOSMessages() = 0;

protected:

};

} // namespace ostrich

#endif /* OSTRICH_I_INPUT_H_ */
