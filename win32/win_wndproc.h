/*
==========================================
Copyright (c) 2020 Ostrich Labs

Windows message pump

Requires an interface to the EventQueue to add events based on messages received
==========================================
*/

#ifndef OSTRICH_WIN_WNDPROC_H_
#define OSTRICH_WIN_WNDPROC_H_

#include "../game/eventqueue.h"

#include <Windows.h>

namespace ostrich {

/////////////////////////////////////////////////
// Attach an EventSender object to the Windows message pump module.
// Obviously this will only work for a single message pump.
// Future incarnations might need something more complex.
//
// in:
//      eventsender - an initialized EventSender for sending messages to an EventQueue
// returns:
//      void
void InitWndProc(EventSender eventsender);

/////////////////////////////////////////////////
// Windows message pump, standard definition
//
// in:
// returns:
//      
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

} // namespace ostrich

#endif /* OSTRICH_WIN_WNDPROC_H_ */