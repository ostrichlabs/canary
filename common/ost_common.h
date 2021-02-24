/*
==========================================
Copyright (c) 2020-2021 Ostrich Labs

Common definitions
==========================================
*/

#ifndef OSTRICH_COMMON_H_
#define OSTRICH_COMMON_H_

#include <cstddef>
#include <cstdint>

namespace ostrich {

namespace character {

/////////////////////////////////////////////////
// character defines
// using these avoids character/string literals everywhere
constexpr char g_Null           { u8'\0' };
constexpr char g_NewLine        { u8'\n' };
constexpr char g_CarriageReturn { u8'\r' };
constexpr char g_Space          { u8' ' };
constexpr char g_SemiColon      { u8';' };
constexpr char g_ForwardSlash   { u8'/' };
constexpr char g_Quote          { u8'\"' };
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
const int32_t g_PlatformLinux = 3;

} // namespace ostrich

namespace ost_char = ostrich::character;

#define OST_UNUSED_PARAMETER(P) (void(sizeof(P)))

/////////////////////////////////////////////////
// Platform defines
// RASPI has to be defined at the command line for this to function properly
// For GCC debug builds, -DGCC_DEBUG should be specified
// Each platform needs to define g_Platform and g_PlatformString
#undef OST_LINUX
#undef OST_RASPI
#undef OST_WINDOWS
#undef OST_MSVC
#undef OST_MACHINE_X64
#undef OST_MACHINE_X32
#undef OST_FUNCTION_SIGNATURE

#undef OST_DEBUG_BUILD
#define OST_DEBUG_BUILD 0

#if defined(RASPI) // Raspberry Pi - must be defined globally via compiler setting

#   define OST_RASPI 1

#   if defined(GCC_DEBUG)
#       undef OST_DEBUG_BUILD
#       define OST_DEBUG_BUILD 1
#   endif

#   define OST_FUNCTION_SIGNATURE __PRETTY_FUNCTION__

namespace ostrich {
const int32_t g_Platform = g_PlatformRaspi;
const char *const g_PlatformString = u8"Raspberry Pi";
}

#elif defined(__linux__) // non-Raspberry Pi Linux flavors

#   define OST_LINUX 1

#   if defined(GCC_DEBUG)
#       undef OST_DEBUG_BUILD
#       define OST_DEBUG_BUILD 1
#   endif

#   define OST_FUNCTION_SIGNATURE __PRETTY_FUNCTION__

namespace ostrich {
const int32_t g_Platform = g_PlatformLinux;
const char *const g_PlatformString = u8"Linux";
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
#       undef OST_DEBUG_BUILD
#       define OST_DEBUG_BUILD 1
#   endif

#   define OST_FUNCTION_SIGNATURE __FUNCSIG__

#   define SIGHUP 1 // SIGHUP is POSIX, not standard C/C++

namespace ostrich {
const int32_t g_Platform = g_PlatformWindows;
const char *const g_PlatformString = u8"Windows";
}

#endif

#if ((OST_RASPI != 1) && (OST_WINDOWS != 1) && (OST_LINUX != 1))
#   error "Platform not specified or unsupported; check ost_common.h"
#endif

namespace ostrich {
constexpr bool g_DebugBuild = (OST_DEBUG_BUILD == 1) ? true : false;
}

/////////////////////////////////////////////////
// debug-specific defines can go here
#if (OST_DEBUG_BUILD == 1)
#endif

#define OST_USE_MEMORYTRACKER 0
//#include "sys_memory.h"

#endif /* OSTRICH_COMMON_H_ */
