/*
==========================================
Copyright(c) 2020 Ostrich Labs

Extension loader and keeper for OpenGL 4

This is more necessary for Windows, as the Microsoft GL driver only exposes up to 1.1.
Other extensions like ARB_debug_output are necessary here too though.

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
    // Currently unused
    enum class Supported : int32_t {
        ARB_POWER_OF_TWO = 0,
        EXTENSION_MAX
    };

    /////////////////////////////////////////////////
    // Constructor simply sets defaults.
    // Destructor does nothing; no memory is allocated.
    // Data is all either simple or copyable, so copy/move constructors/operators are default
    GL4Extensions() noexcept :
        m_glGetStringi(nullptr), m_glCompressedTexImage2D(nullptr), m_EXT_texture_compression_s3tc(false) {}
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
    // these should all exist, or something is seriously wrong
    /////////////////////////////////////////////////
    
    // 1.3 - GL_ARB_texture_compression
    void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
    { if (this->m_glCompressedTexImage2D != nullptr) m_glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data); }

    // 3.0
    const GLubyte *glGetStringi(GLenum name, GLuint index) { return (this->m_glGetStringi != nullptr) ? m_glGetStringi(name, index) : nullptr; }

    /////////////////////////////////////////////////
    // OpenGL extensions
    // For some, checking for their presence is enough
    /////////////////////////////////////////////////
    
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

    bool m_EXT_texture_compression_s3tc;
};

} // namespace ostrich

#endif /* OSTRICH_GL4_EXTENSIONS_H_ */
