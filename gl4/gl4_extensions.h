/*
==========================================
Copyright(c) 2020-2021 Ostrich Labs

Extension loader and keeper for OpenGL 4

This is more necessary for Windows, as the Microsoft GL driver only exposes up to 1.1.
Other extensions like KHR_debug are necessary here too though.

For consistency and simplicity, even non-Windows platforms should access functions in GL >1.1 using this object.

Actually retrieving function pointers is platform specific, through a common ostrich::glGetProcAddress() function.
==========================================
*/

#ifndef OSTRICH_GL4_EXTENSIONS_H_
#define OSTRICH_GL4_EXTENSIONS_H_

#include "gl/glcorearb.h"
#include "gl/glext.h"
#include "../common/console.h"

namespace ostrich {

/////////////////////////////////////////////////
// Common function for retrieving extension function pointers
// Note: win_display modules do not use this to retrieve WGL extensions, because they do not need to
void *glGetProcAddress(const char *name);

/////////////////////////////////////////////////
//
class GL4Extensions {
public:

    /////////////////////////////////////////////////
    // Constructor simply sets defaults.
    // Destructor does nothing; no memory is allocated.
    // Data is all either simple or copyable, so copy/move constructors/operators are default
    GL4Extensions() noexcept :
        m_glGetStringi(nullptr), m_glCompressedTexImage2D(nullptr), m_glDebugMessageControl(nullptr),
        m_glDebugMessageInsert(nullptr), m_glDebugMessageCallback(nullptr), m_glGetDebugMessageLog(nullptr), 
        m_glPushDebugGroup(nullptr), m_glPopDebugGroup(nullptr), m_glObjectLabel(nullptr),
        m_glGetObjectLabel(nullptr), m_glObjectPtrLabel(nullptr), m_glGetObjectPtrLabel(nullptr),
        m_DebugExtensionSupported(false), m_EXT_texture_compression_s3tc(false) {}
    virtual ~GL4Extensions() {}
    GL4Extensions(GL4Extensions &&) = default;
    GL4Extensions(const GL4Extensions &) = default;
    GL4Extensions &operator=(GL4Extensions &&) = default;
    GL4Extensions &operator=(const GL4Extensions &) = default;

    /////////////////////////////////////////////////
    // Load required OpenGL core/extension function pointers.
    //
    // in:
    //      consoleprinter - an initialized ConsolePrinter for logging
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int Load(ConsolePrinter consoleprinter);

    /////////////////////////////////////////////////
    // OpenGL 4 core functions
    // These should all exist, or something is seriously wrong
    // Refer to OpenGL documentation for a full explanation of each function
    /////////////////////////////////////////////////
    
    /////////////////////////////////////////////////
    // 1.3 - GL_ARB_texture_compression
    void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
    {
        if (this->m_glCompressedTexImage2D != nullptr) m_glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
    }

    /////////////////////////////////////////////////
    // 3.0
    const GLubyte *glGetStringi(GLenum name, GLuint index)
    {
        return (this->m_glGetStringi != nullptr) ? m_glGetStringi(name, index) : nullptr;
    }

    /////////////////////////////////////////////////
    // OpenGL extensions
    // For some, checking for their presence is enough
    // Refer to OpenGL documentation for a full explanation of each function
    /////////////////////////////////////////////////

    /////////////////////////////////////////////////
    // KHR_debug
    /////////////////////////////////////////////////
    bool debugSupported() const noexcept { return m_DebugExtensionSupported; }

    void glDebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled)
    { if (this->m_glDebugMessageControl != nullptr) m_glDebugMessageControl(source, type, severity, count, ids, enabled); }

    void glDebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf)
    { if (this->m_glDebugMessageInsert != nullptr) m_glDebugMessageInsert(source, type, id, severity, length, buf); }

    void glDebugMessageCallback(GLDEBUGPROC callback, const void *userParam)
    { if (this->m_glDebugMessageCallback != nullptr) m_glDebugMessageCallback(callback, userParam); }

    GLuint glGetDebugMessageLog(GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog)
    { return (this->m_glGetDebugMessageLog != nullptr) ? m_glGetDebugMessageLog(count, bufSize, sources, types, ids, severities, lengths, messageLog) : 0; }

    void glPushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar *message)
    { if (m_glPushDebugGroup != nullptr) m_glPushDebugGroup(source, id, length, message); }

    void glPopDebugGroup()
    { if (m_glPopDebugGroup != nullptr) m_glPopDebugGroup(); }

    void glObjectLabel(GLenum identifier, GLuint name, GLsizei length, const GLchar *label)
    { if (m_glObjectLabel != nullptr) m_glObjectLabel(identifier, name, length, label); }

    void glGetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label)
    { if (m_glGetObjectLabel != nullptr) m_glGetObjectLabel(identifier, name, bufSize, length, label); }

    void glObjectPtrLabel(void *ptr, GLsizei length, const GLchar *label)
    { if (m_glObjectPtrLabel != nullptr) m_glObjectPtrLabel(ptr, length, label); }
    
    void glGetObjectPtrLabel(void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label)
    { if (m_glGetObjectPtrLabel != nullptr) m_glGetObjectPtrLabel(ptr, bufSize, length, label); }
    
    /////////////////////////////////////////////////
    // Checks if S3TC texture compression is supported.
    // Texture compression itself is core functionality, but S3TC/DXTC formats are not.
    //
    // returns:
    //      true/false if GL_EXT_texture_compression_s3tc extension is supported.
    bool supportsS3TCCompression() const noexcept { return m_EXT_texture_compression_s3tc; }

private:

    /////////////////////////////////////////////////
    // Load required OpenGL core function pointers.
    // Assumes anything in OpenGL 4 will be here.
    // Something added in later versions will have to check for an extension
    //
    // in:
    //      consoleprinter - an initialized ConsolePrinter for logging
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int LoadCore(ConsolePrinter consoleprinter);

    /////////////////////////////////////////////////
    // Load required OpenGL extension function pointers.
    //
    // in:
    //      consoleprinter - an initialized ConsolePrinter for logging
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int LoadExtensions(ConsolePrinter consoleprinter);

    PFNGLGETSTRINGIPROC m_glGetStringi;
    PFNGLCOMPRESSEDTEXIMAGE2DPROC m_glCompressedTexImage2D;

    PFNGLDEBUGMESSAGECONTROLPROC m_glDebugMessageControl;
    PFNGLDEBUGMESSAGEINSERTPROC m_glDebugMessageInsert;
    PFNGLDEBUGMESSAGECALLBACKPROC m_glDebugMessageCallback;
    PFNGLGETDEBUGMESSAGELOGPROC m_glGetDebugMessageLog;
    PFNGLPUSHDEBUGGROUPPROC m_glPushDebugGroup;
    PFNGLPOPDEBUGGROUPPROC m_glPopDebugGroup;
    PFNGLOBJECTLABELPROC m_glObjectLabel;
    PFNGLGETOBJECTLABELPROC m_glGetObjectLabel;
    PFNGLOBJECTPTRLABELPROC m_glObjectPtrLabel;
    PFNGLGETOBJECTPTRLABELPROC m_glGetObjectPtrLabel;

    bool m_DebugExtensionSupported;
    bool m_EXT_texture_compression_s3tc;
};

} // namespace ostrich

#endif /* OSTRICH_GL4_EXTENSIONS_H_ */
