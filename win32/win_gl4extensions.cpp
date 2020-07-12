/*
==========================================
Copyright(c) 2020 Ostrich Labs

Windows implementation of ostrich::glGetProcAddress()
==========================================
*/

#include "../common/ost_common.h"

#if (OST_WINDOWS != 1)
#   error "This module should only be included in Windows builds"
#endif

#include <Windows.h>
#include "../gl4/gl4_extensions.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void *ostrich::glGetProcAddress(const char *name) {
    return (void *)::wglGetProcAddress(name);
}