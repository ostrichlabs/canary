/*
==========================================
Copyright(c) 2020-2021 Ostrich Labs

Extension loader and keeper for OpenGL 4

This is more necessary for Windows, as the Microsoft GL driver only exposes up to 1.1.

For consistency and simplicity, even non-Windows platforms should access functions in GL >1.1 using this object.
Pre-4.0 OpenGL functions will be loaded as Core, 4.0 and later will access via extensions

Actually retrieving function pointers is platform specific, through a common ostrich::glGetProcAddress() function.
==========================================
*/

#ifndef OSTRICH_GL4_EXTENSIONS_H_
#define OSTRICH_GL4_EXTENSIONS_H_

#include "../common/ost_common.h"

#if (OST_WINDOWS == 1)
#   include <windows.h> // required for GL headers
#endif

#include <gl/GL.h>
#include "gl/glext.h"       // taken from https://github.com/KhronosGroup/OpenGL-Registry
#include "../common/console.h"

namespace ostrich {

/////////////////////////////////////////////////
// Common function for retrieving extension function pointers
// Note: win_display modules do not use this to retrieve WGL extensions, because they do not need to
//
// in:
//      name - full name of OpenGL function to retrieve
// returns:
//      void pointer to the function - cast this to the function you need
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
        m_glCompressedTexImage2D(nullptr), m_glGetStringi(nullptr), m_glGenerateMipmap(nullptr),
        m_glDebugMessageControl(nullptr), m_glDebugMessageInsert(nullptr), m_glDebugMessageCallback(nullptr),
        m_glGetDebugMessageLog(nullptr), m_glPushDebugGroup(nullptr), m_glPopDebugGroup(nullptr),
        m_glObjectLabel(nullptr), m_glGetObjectLabel(nullptr), m_glObjectPtrLabel(nullptr), m_glGetObjectPtrLabel(nullptr),
        m_glCreateTextures(nullptr), m_glTextureParameteri(nullptr), m_glTextureStorage2D(nullptr),
        m_glTextureSubImage2D(nullptr), m_glGenerateTextureMipmap(nullptr),
        m_KHR_debug(false), m_EXT_texture_compression_s3tc(false), m_ARB_direct_state_access(false) {}
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
    // OpenGL core functions
    // These should all exist, or something is seriously wrong
    // Refer to OpenGL documentation for a full explanation of each function
    /////////////////////////////////////////////////
    
    /////////////////////////////////////////////////
    // 1.3 - GL_ARB_texture_compression
    void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
    { if (this->m_glCompressedTexImage2D != nullptr) { this->m_glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data); } }

    /////////////////////////////////////////////////
    // 3.0
    const GLubyte *glGetStringi(GLenum name, GLuint index)
    { return ((this->m_glGetStringi != nullptr) ? this->m_glGetStringi(name, index) : nullptr); }

    /////////////////////////////////////////////////
    // 3.0 - GL_ARB_framebuffer_object
    void glGenerateMipmap(GLenum target)
    { if (this->m_glGenerateMipmap != nullptr) { this->m_glGenerateMipmap(target); } }

    /////////////////////////////////////////////////
    // OpenGL extensions
    // For some, checking for their presence is enough
    // Refer to OpenGL documentation for a full explanation of each function
    /////////////////////////////////////////////////

    /////////////////////////////////////////////////
    // KHR_debug
    /////////////////////////////////////////////////
    bool debugSupported() const noexcept { return m_KHR_debug; }

    void glDebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled)
    { if (this->m_glDebugMessageControl != nullptr) { this->m_glDebugMessageControl(source, type, severity, count, ids, enabled); } }

    void glDebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf)
    { if (this->m_glDebugMessageInsert != nullptr) { this->m_glDebugMessageInsert(source, type, id, severity, length, buf); } }

    void glDebugMessageCallback(GLDEBUGPROC callback, const void *userParam)
    { if (this->m_glDebugMessageCallback != nullptr) { this->m_glDebugMessageCallback(callback, userParam); } }

    GLuint glGetDebugMessageLog(GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog)
    { return ((this->m_glGetDebugMessageLog != nullptr) ? this->m_glGetDebugMessageLog(count, bufSize, sources, types, ids, severities, lengths, messageLog) : 0); }

    void glPushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar *message)
    { if (this->m_glPushDebugGroup != nullptr) { this->m_glPushDebugGroup(source, id, length, message); } }

    void glPopDebugGroup()
    { if (this->m_glPopDebugGroup != nullptr) { this->m_glPopDebugGroup(); } }

    void glObjectLabel(GLenum identifier, GLuint name, GLsizei length, const GLchar *label)
    { if (this->m_glObjectLabel != nullptr) { this->m_glObjectLabel(identifier, name, length, label); } }

    void glGetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label)
    { if (this->m_glGetObjectLabel != nullptr) { this->m_glGetObjectLabel(identifier, name, bufSize, length, label); } }

    void glObjectPtrLabel(void *ptr, GLsizei length, const GLchar *label)
    { if (this->m_glObjectPtrLabel != nullptr) { this->m_glObjectPtrLabel(ptr, length, label); } }
    
    void glGetObjectPtrLabel(void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label)
    { if (this->m_glGetObjectPtrLabel != nullptr) { this->m_glGetObjectPtrLabel(ptr, bufSize, length, label); } }
    
    /////////////////////////////////////////////////
    // Checks if S3TC texture compression is supported.
    // Texture compression itself is core functionality, but S3TC/DXTC formats are not.
    //
    // returns:
    //      true/false if GL_EXT_texture_compression_s3tc extension is supported.
    bool supportsS3TCCompression() const noexcept { return m_EXT_texture_compression_s3tc; }

    /////////////////////////////////////////////////
    // ARB_direct_state_access
    // Adding functions as I need them, rather than all of them
    /////////////////////////////////////////////////

    bool DirectStateAccessSupported() const noexcept { return m_ARB_direct_state_access; }

    void glCreateTextures(GLenum target, GLsizei n, GLuint *textures)
    { if (this->m_glCreateTextures != nullptr) { this->m_glCreateTextures(target, n, textures); } }

    void glTextureParameteri(GLuint texture, GLenum pname, GLint param)
    { if (this->m_glTextureParameteri != nullptr) { this->m_glTextureParameteri(texture, pname, param); } }

    void glTextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
    { if (this->m_glTextureStorage2D != nullptr) { this->m_glTextureStorage2D(texture, levels, internalformat, width, height); } }

    void glTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
    { if (this->m_glTextureSubImage2D != nullptr) { this->m_glTextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, type, pixels); } }

    void glGenerateTextureMipmap(GLuint texture)
    { if (this->m_glGenerateTextureMipmap != nullptr) { this->m_glGenerateTextureMipmap(texture); } }

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

    PFNGLCOMPRESSEDTEXIMAGE2DPROC m_glCompressedTexImage2D;
    PFNGLGETSTRINGIPROC m_glGetStringi;
    PFNGLGENERATEMIPMAPPROC m_glGenerateMipmap;

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

    PFNGLCREATETEXTURESPROC m_glCreateTextures;
    PFNGLTEXTUREPARAMETERIPROC m_glTextureParameteri;
    PFNGLTEXTURESTORAGE2DPROC m_glTextureStorage2D;
    PFNGLTEXTURESUBIMAGE2DPROC m_glTextureSubImage2D;
    PFNGLGENERATETEXTUREMIPMAPPROC m_glGenerateTextureMipmap;

    bool m_KHR_debug;
    bool m_EXT_texture_compression_s3tc;
    bool m_ARB_direct_state_access;
};

} // namespace ostrich

#endif /* OSTRICH_GL4_EXTENSIONS_H_ */
