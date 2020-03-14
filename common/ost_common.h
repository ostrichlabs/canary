/*
==========================================
ost_common.h

Copyright (c) 2020 Ostrich Labs

Common definitions
==========================================
*/

#ifndef COMMON_H_
#define COMMON_H_

#include <cstddef>
#include <cstdint>

namespace ostrich {

namespace character {

/////////////////////////////////////////////////
// character defines
// using these avoids character/string literals everywhere
constexpr char g_Null           { '\0' };
constexpr char g_NewLine        { '\n' };
constexpr char g_CarriageReturn { '\r' };
constexpr char g_Space          { ' ' };
constexpr char g_SemiColon      { ';' };
constexpr char g_ForwardSlash   { '/' };
constexpr char g_Quote          { '\"' };
const char * const g_Whitespace = u8" \t\n\r";

} // namespace character

/////////////////////////////////////////////////
// global constants
//
// display constants - these should be variable (set by user via options) but for Canary this is fine
const int32_t g_ScreenWidth = 1920;
const int32_t g_ScreenHeight = 1080;

const int32_t g_PlatformWindows = 1;
const int32_t g_PlatformRaspi = 2;

} // namespace ostrich

namespace ost_char = ostrich::character;

#undef UNUSED_PARAMETER
#define UNUSED_PARAMETER(P) (void(sizeof(P)))

/////////////////////////////////////////////////
// Platform defines
// RASPI has to be defined at the command line for this to function properly
// Each platform needs to define g_Platform and g_PlatformString
// TODO: Add generic Linux?
#undef OST_RASPI
#undef OST_DEBUG_BUILD
#undef OST_WINDOWS
#undef OST_MSVC
#undef OST_MACHINE_X64
#undef OST_MACHINE_X32
#undef OST_FUNCTION_SIGNATURE

#if defined(RASPI) // Raspberry Pi

#   define OST_RASPI 1

#   if defined(GCC_DEBUG)
#       define OST_DEBUG_BUILD 1
#   endif

#   define OST_FUNCTION_SIGNATURE __PRETTY_FUNCTION__

namespace ostrich {
const int32_t g_Platform = g_PlatformRaspi;
const char *const g_PlatformString = u8"Raspberry Pi";
}

#elif ((_WIN32 == 1) && defined(_MSC_VER)) // Windows (any modern flavor)

#   define OST_WINDOWS 1
#   define OST_MSVC 1
#   if defined(_M_X64)
#       define OST_MACHINE_X64 1
#   elif defined(_M_IX86)
#       define OST_MACHINE_X32 1
#   endif

#   if (_DEBUG == 1)
#       define OST_DEBUG_BUILD 1
#   endif

#   define OST_FUNCTION_SIGNATURE __FUNCSIG__

namespace ostrich {
const int32_t g_Platform = g_PlatformWindows;
const char *const g_PlatformString = u8"Windows";
}

#endif

#if ((OST_RASPI != 1) && (OST_WINDOWS != 1))
#   error "You must define RASPI or use Microsoft Visual Studio to build this project"
#endif

/////////////////////////////////////////////////
// debug-specific defines can go here
#if (OST_DEBUG_BUILD == 1)
#endif

// TODO: Work on memory tracker
#define OST_USE_MEMORYTRACKER 1
//#include "sys_memory.h"

#endif /* COMMON_H_ */
