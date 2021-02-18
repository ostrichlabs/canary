/*
==========================================
Copyright(c) 2020-2021 Ostrich Labs
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
// NOTE FOR TOMORROW:
// I need to check for specific extensions here too, starting with KHR_debug
/////////////////////////////////////////////////
int ostrich::GL4Extensions::LoadExtensions(ostrich::ConsolePrinter consoleprinter) {
    GLint extcount = 0;
    bool supported = false;
    std::string_view ext;
    std::string extlist;
    ::glGetIntegerv(GL_NUM_EXTENSIONS, &extcount);
    for (GLint i = 0; i < extcount; i++) {
        supported = true;
        ext = (const char *)(this->glGetStringi(GL_EXTENSIONS, i));
        extlist.append(ext);
        extlist += ost_char::g_Space;

        if (ext.find("GL_KHR_debug")) {
            m_glDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC)ostrich::glGetProcAddress("glDebugMessageControl");
            m_glDebugMessageInsert = (PFNGLDEBUGMESSAGEINSERTPROC)ostrich::glGetProcAddress("glDebugMessageInsert");
            m_glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)ostrich::glGetProcAddress("glDebugMessageCallback");
            m_glGetDebugMessageLog = (PFNGLGETDEBUGMESSAGELOGPROC)ostrich::glGetProcAddress("glGetDebugMessageLog");
            m_glPushDebugGroup = (PFNGLPUSHDEBUGGROUPPROC)ostrich::glGetProcAddress("glPushDebugGroup");
            m_glPopDebugGroup = (PFNGLPOPDEBUGGROUPPROC)ostrich::glGetProcAddress("glPopDebugGroup");
            m_glObjectLabel = (PFNGLOBJECTLABELPROC)ostrich::glGetProcAddress("glObjectLabel");
            m_glGetObjectLabel = (PFNGLGETOBJECTLABELPROC)ostrich::glGetProcAddress("glGetObjectLabel");
            m_glObjectPtrLabel = (PFNGLOBJECTPTRLABELPROC)ostrich::glGetProcAddress("glObjectPtrLabel");
            m_glGetObjectPtrLabel = (PFNGLGETOBJECTPTRLABELPROC)ostrich::glGetProcAddress("glGetObjectPtrLabel");
            if (m_glDebugMessageControl != nullptr &&
                m_glDebugMessageInsert != nullptr &&
                m_glDebugMessageCallback != nullptr &&
                m_glGetDebugMessageLog != nullptr &&
                m_glPushDebugGroup != nullptr &&
                m_glPopDebugGroup != nullptr &&
                m_glObjectLabel != nullptr &&
                m_glGetObjectLabel != nullptr &&
                m_glObjectPtrLabel != nullptr &&
                m_glGetObjectPtrLabel != nullptr) {
                consoleprinter.DebugMessage("GL_KHR_debug supported");
            }
        }
    }
    consoleprinter.DebugMessage(u8"Supported extensions: %", { extlist });

    return OST_ERROR_OK;
}
