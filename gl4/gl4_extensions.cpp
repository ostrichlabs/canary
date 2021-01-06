/*
==========================================
Copyright(c) 2020 Ostrich Labs
==========================================
*/

#include "gl4_extensions.h"

#if (OST_WINDOWS == 1)
#   include <windows.h> // required for GL.h
#endif

#include <GL/gl.h>
#include "../common/ost_common.h"
#include "../game/errorcodes.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::GL4Extensions::Load(ostrich::ConsolePrinter consoleprinter) {
    int result = this->LoadCore(consoleprinter);
    if (result == OST_ERROR_OK) {
        result = this->LoadExtensions(consoleprinter);
    }

    return result;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::GL4Extensions::LoadCore(ostrich::ConsolePrinter consoleprinter) {
    m_glGetStringi = (PFNGLGETSTRINGIPROC)ostrich::glGetProcAddress("glGetStringi");
    if (m_glGetStringi == nullptr) {
        return OST_ERROR_GL4COREGETPROCADDR;
    }

    m_glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)ostrich::glGetProcAddress("glCompressedTexImage2D");
    if (m_glCompressedTexImage2D == nullptr) {
        return OST_ERROR_GL4COREGETPROCADDR;
    }

    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::GL4Extensions::LoadExtensions(ostrich::ConsolePrinter consoleprinter) {
    GLint extcount = 0;
    std::string_view ext;
    std::string extlist;
    ::glGetIntegerv(GL_NUM_EXTENSIONS, &extcount);
    for (GLint i = 0; i < extcount; i++) {
        ext = (const char *)(this->glGetStringi(GL_EXTENSIONS, i));
        extlist.append(ext);
        extlist += ost_char::g_Space;
    }
    consoleprinter.DebugMessage(u8"Supported extensions: %", { extlist });

    return OST_ERROR_OK;
}
