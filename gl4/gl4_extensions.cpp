/*
==========================================
Copyright(c) 2020-2021 Ostrich Labs
==========================================
*/

#include "gl4_extensions.h"
#include "../common/ost_common.h"

#if (OST_WINDOWS == 1)
#   include <windows.h> // required for GL.h
#endif

#include <GL/gl.h>
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
    OST_UNUSED_PARAMETER(consoleprinter);

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

    if (extlist.find("GL_KHR_debug")) {
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
            m_KHR_debug = true;
            consoleprinter.WriteMessage(u8"OpenGL Extension Supported: GL_KHR_debug");
        }
    }

    if (extlist.find("GL_EXT_texture_compression_s3tc")) {
        m_EXT_texture_compression_s3tc = true;
        consoleprinter.WriteMessage("OpenGL Extension Supported: GL_EXT_texture_compression_s3tc");
    }

    return OST_ERROR_OK;
}
