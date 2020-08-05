/*
==========================================
Copyright (c) 2020 Ostrich Labs

Windows message pump

Requires an interface to the EventQueue to add events based on messages received
==========================================
*/

#ifndef WIN_WNDPROC_H_
#define WIN_WNDPROC_H_

#include "../game/eventqueue.h"

#include <Windows.h>

namespace ostrich {

void InitWndProc(EventSender eventsender);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

} // namespace ostrich

#endif /* WIN_WNDPROC_H_ */