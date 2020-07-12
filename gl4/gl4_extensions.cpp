/*
==========================================
Copyright(c) 2020 Ostrich Labs
==========================================
*/

#include "gl4_extensions.h"
#include "../game/errorcodes.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::GL4Extensions::Load() {
    m_glGetStringi = (PFNGLGETSTRINGIPROC)ostrich::glGetProcAddress("glGetStringi");
    if (m_glGetStringi == nullptr)
        return OST_ERROR_GL4COREGETPROCADDR;

    return OST_ERROR_OK;
}