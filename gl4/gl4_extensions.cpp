/*
==========================================
Copyright(c) 2020-2021 Ostrich Labs
==========================================
*/

#include "gl4_extensions.h"
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

    m_glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)ostrich::glGetProcAddress("glCompressedTexImage2D");
    if (m_glCompressedTexImage2D == nullptr) {
        return OST_ERROR_GL4COREGETPROCADDR;
    }

    m_glGetStringi = (PFNGLGETSTRINGIPROC)ostrich::glGetProcAddress("glGetStringi");
    if (m_glGetStringi == nullptr) {
        return OST_ERROR_GL4COREGETPROCADDR;
    }

    m_glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)ostrich::glGetProcAddress("glGenerateMipmap");
    if (m_glGenerateMipmap == nullptr) {
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
        extlist += ost_char::g_NewLine;
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

    if (extlist.find("GL_ARB_direct_state_access")) {
        m_glCreateTextures = (PFNGLCREATETEXTURESPROC)ostrich::glGetProcAddress("glCreateTextures");
        m_glTextureParameteri = (PFNGLTEXTUREPARAMETERIPROC)ostrich::glGetProcAddress("glTextureParameteri");
        m_glTextureStorage2D = (PFNGLTEXTURESTORAGE2DPROC)ostrich::glGetProcAddress("glTextureStorage2D");
        m_glTextureSubImage2D = (PFNGLTEXTURESUBIMAGE2DPROC)ostrich::glGetProcAddress("glTextureSubImage2D");
        m_glGenerateTextureMipmap = (PFNGLGENERATETEXTUREMIPMAPPROC)ostrich::glGetProcAddress("glGenerateTextureMipmap");
        if (m_glCreateTextures != nullptr &&
            m_glTextureParameteri != nullptr &&
            m_glTextureStorage2D != nullptr &&
            m_glTextureSubImage2D != nullptr &&
            m_glGenerateTextureMipmap != nullptr) {
            m_ARB_direct_state_access = true;
            consoleprinter.WriteMessage("OpenGL Extension Supported: GL_ARB_direct_state_access");
        }
    }

    return OST_ERROR_OK;
}
