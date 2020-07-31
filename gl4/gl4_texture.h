/*
==========================================
Copyright (c) 2020 Ostrich Labs

Helper structure to map a bound GL texture to a file name.

The file name is used as the common ID because it's there.
==========================================
*/

#ifndef GL4_TEXTURE_H
#define GL4_TEXTURE_H

#include "../common/ost_common.h"

#if (OST_WINDOWS == 1)
#   include <Windows.h> // required for gl.h
#endif

#include <GL/gl.h>
#include <string>
#include "gl4_extensions.h"
#include "../common/image.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class GL4Texture {
public:

    virtual ~GL4Texture();
    GL4Texture(GL4Texture &&) = delete;
    GL4Texture(const GL4Texture &) = delete;
    GL4Texture &operator=(GL4Texture &&) = delete;
    GL4Texture &operator=(const GL4Texture &) = delete;

    static GL4Texture CreateTexture(const ostrich::Image &image, GL4Extensions &ext);

    void UnbindTexture();

    std::string_view getFilename() const noexcept { return m_Name; }
    GLuint getTexObject() const noexcept { return m_Texture; }

private:

    GL4Texture() noexcept : m_Name(""), m_Texture(0) {}

    GL4Texture(std::string_view name, GLuint tex) noexcept : m_Name(std::string(name)), m_Texture(tex) {}

    std::string m_Name;
    GLuint m_Texture;
};

} // namespace ostrich

#endif /* GL4_TEXTURE_H */
