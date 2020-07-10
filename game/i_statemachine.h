/*
==========================================
Copyright (c) 2020 Ostrich Labs

Interface for state machine objects
==========================================
*/

#ifndef I_STATEMACHINE_H_
#define I_STATEMACHINE_H_

#include "eventqueue.h"
#include "../common/console.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class IStateMachine {
public:

    IStateMachine() noexcept {}
    virtual ~IStateMachine() {}

    virtual bool isActive() const = 0;

    virtual int Initialize(ConsolePrinter, EventSender) = 0;
    virtual int Destroy() = 0;

private:
};

} // namespace ostrich

#endif /* I_STATEMACHINE_H_ */