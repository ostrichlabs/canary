/*
==========================================
signal.h

Copyright (c) 2020 Ostrich Labs

Signal handling
==========================================
*/

#ifndef SIGNAL_H_
#define SIGNAL_H_

#include "../game/ost_events.h"

namespace ostrich {

namespace signal {

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int Initialize(EventSender es);

} // namespace signal

} // namespace ostrich

#endif /* SIGNAL_H_ */