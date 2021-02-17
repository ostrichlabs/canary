/*
==========================================
Copyright (c) 2020 Ostrich Labs

Helper structure to map a bound GL texture to a file name.

Going to use std::hash of the filename as the ID until there's problems. Then I'll figure something else out.
==========================================
*/

#ifndef OSTRICH_GL4_TEXTURE_H
#define OSTRICH_GL4_TEXTURE_H

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
// Manage a texture loaded into GL
// Currently just handles 2D textures with no mipmaps
class GL4Texture {
public:

    /////////////////////////////////////////////////
    // Constructors are all private; use the static factory methods to create GL4Textures
    // Destructor is explicitly defined
    // Copy/move constructors/operators are deleted to prevent deleting the same texture twice
    virtual ~GL4Texture();
    GL4Texture(GL4Texture &&) = delete;
    GL4Texture(const GL4Texture &) = delete;
    GL4Texture &operator=(GL4Texture &&) = delete;
    GL4Texture &operator=(const GL4Texture &) = delete;

    /////////////////////////////////////////////////
    // Load image data into OpenGL
    //
    // in:
    //      image - constructed Image object with valid data
    //      ext - GL extension object with pre-loaded functions
    // returns:
    //      on success, a GL4Texture with a valid GL texture ID and a unique ID generated from a hash function
    //      on failure, a default-constructed object (reference glGetError())
    static GL4Texture CreateTexture(const ostrich::Image &image, GL4Extensions &ext);

    /////////////////////////////////////////////////
    // Force GL to unbind the texture
    // This is a permanent operation; there is no mechanism for re-loading a texture after this is complete
    // Maybe this is a bad idea, but we'll cross that bridge when we get to it
    //
    // returns:
    //      void
    void UnbindTexture();

    /////////////////////////////////////////////////
    // accessor methods
    /////////////////////////////////////////////////
    std::size_t getUniqueID() const noexcept { return m_UniqueID; }
    GLuint getTexObject() const noexcept { return m_Texture; }

private:

    /////////////////////////////////////////////////
    // Default constructor, when no data is available
    GL4Texture() noexcept : m_UniqueID(0), m_Texture(0) {}

    /////////////////////////////////////////////////
    // Creates an object with provided data
    GL4Texture(std::size_t uid, GLuint tex) noexcept : m_UniqueID(uid), m_Texture(tex) {}

    std::size_t m_UniqueID;
    GLuint m_Texture;
};

} // namespace ostrich

#endif /* OSTRICH_GL4_TEXTURE_H */
