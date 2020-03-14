/*
==========================================
i_input.h

Copyright (c) 2020 Ostrich Labs

Interface for retrieving input information from the OS
Translates input to standard Ostrich format and adds to EventQueue

Input can be Keyboard/Mouse (KBM) or OS messages
==========================================
*/

#ifndef I_INPUT_H_
#define I_INPUT_H_

#include "ost_events.h"
#include "../common/console.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class IInput {
public:

    IInput() noexcept {}
    virtual ~IInput() {}

    virtual int Initialize(ConsolePrinter, EventSender) = 0;
    virtual void Destroy() = 0;

    virtual bool isActive() const noexcept = 0;

    virtual void ProcessKBM() = 0;
    virtual void ProcessOSMessages() = 0;

private:

};

} // namespace ostrich

#endif /* I_INPUT_H_ */
