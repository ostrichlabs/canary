/*
==========================================
keydef.h

Copyright (c) 2019 Ostrich Labs

Definitions of internal keys and key state
==========================================
*/

#ifndef KEYDEF_H_
#define KEYDEF_H_

#include <cstdint>

namespace ostrich {

/////////////////////////////////////////////////
// mouse and keyboard keys, some of which don't have a UTF-8 equivalent
enum class Button : int32_t {
	NONE = 0,
    MOUSE1,
	MOUSE2,
	MOUSE3,
    CTRL,
    ALT,
    SHIFT,
    TAB,
    KEY,
	BUTTONCOUNT
};

/////////////////////////////////////////////////
//
enum class ButtonState : int32_t {
	NONE = 0,
	UP,
    DOWN,
	MOVE,
	STATECOUNT
};

} // namespace ostrich

#endif /* KEYDEF_H_ */
