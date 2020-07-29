/*
==========================================
Copyright(c) 2020 Ostrich Labs

Linux implementation of ostrich::glGetProcAddress()
==========================================
*/

#include "../common/ost_common.h"

#if (OST_LINUX != 1)
#   error "This module should only be included in Linux builds"
#endif

#include "../gl4/gl4_extensions.h"
#include <GL/glx.h>

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void *ostrich::glGetProcAddress(const char *name) {
    return (void *)::glXGetProcAddress((const unsigned char *)name);
}
