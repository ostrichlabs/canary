/*
==========================================
Copyright(c) 2020 Ostrich Labs

Extension loader and keeper for OpenGL 4

This is more necessary for Windows, as the Microsoft GL driver only exposes up to 1.1.
Other extensions like ARB_debug_output are necessary here too though.

Actually retrieving function pointers is platform specific, through a common ostrich::glGetProcAddress() function.
==========================================
*/

#ifndef GL4_EXTENSIONS_H_
#define GL4_EXTENSIONS_H_

#include "gl/glcorearb.h"
#include "gl/glext.h"
#include "../common/console.h"

namespace ostrich {

/////////////////////////////////////////////////
// Common function for retrieving extension function pointers
// Note: win_display modules do not use this, because they do not need to
void *glGetProcAddress(const char *name);

/////////////////////////////////////////////////
//
class GL4Extensions {
public:

    enum class Supported : int32_t {
        ARB_POWER_OF_TWO = 0,
        EXTENSION_MAX
    };

    GL4Extensions() noexcept :
        m_glGetStringi(nullptr), m_glCompressedTexImage2D(nullptr), m_EXT_texture_compression_s3tc(false) {}
    virtual ~GL4Extensions() {}
    GL4Extensions(GL4Extensions &&) = default;
    GL4Extensions(const GL4Extensions &) = default;
    GL4Extensions &operator=(GL4Extensions &&) = default;
    GL4Extensions &operator=(const GL4Extensions &) = default;

    int Load(ConsolePrinter consoleprinter);

    /////////////////////////////////////////////////
    // OpenGL 4 core functions
    // these should all exist, or something is seriously wrong
    /////////////////////////////////////////////////
    
    // 1.3
    void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
    { if (this->m_glCompressedTexImage2D != nullptr) m_glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data); }

    // 3.0
    const GLubyte *glGetStringi(GLenum name, GLuint index) { return (this->m_glGetStringi != nullptr) ? m_glGetStringi(name, index) : nullptr; }

    /////////////////////////////////////////////////
    // OpenGL extensions
    // For some, checking for their presence is enough
    /////////////////////////////////////////////////
    
    // texture compression is core but DXTx formats are not
    bool supportsS3TCCompression() const noexcept { return m_EXT_texture_compression_s3tc; }

private:

    int LoadCore(ConsolePrinter consoleprinter);
    int LoadExtensions(ConsolePrinter consoleprinter);

    PFNGLGETSTRINGIPROC m_glGetStringi;
    PFNGLCOMPRESSEDTEXIMAGE2DPROC m_glCompressedTexImage2D;

    bool m_EXT_texture_compression_s3tc;
};

} // namespace ostrich

#endif /* GL4_EXTENSIONS_H_ */
