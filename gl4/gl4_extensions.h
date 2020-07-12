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

#include "../external/gl/glcorearb.h"
#include "../external/gl/glext.h"

namespace ostrich {

/////////////////////////////////////////////////
// Common function for retrieving extension function pointers
// Note: win_display modules do not use this, because they do not need to
void *glGetProcAddress(const char *name);

/////////////////////////////////////////////////
//
class GL4Extensions {
public:

    GL4Extensions() noexcept :
        m_glGetStringi(nullptr) {}
    virtual ~GL4Extensions() {}
    GL4Extensions(GL4Extensions &&) = default;
    GL4Extensions(const GL4Extensions &) = default;
    GL4Extensions &operator=(GL4Extensions &&) = default;
    GL4Extensions &operator=(const GL4Extensions &) = default;

    int Load();

    /////////////////////////////////////////////////
    // OpenGL 4 core functions
    // these should all exist, or something is seriously wrong
    /////////////////////////////////////////////////
    
    const GLubyte *glGetStringi(GLenum name, GLuint index) { return (this->m_glGetStringi != nullptr) ? m_glGetStringi(name, index) : nullptr; }

private:

    PFNGLGETSTRINGIPROC m_glGetStringi;
};

} // namespace ostrich

#endif /* GL4_EXTENSIONS_H_ */
