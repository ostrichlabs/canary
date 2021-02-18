/*
==========================================
Copyright (c) 2020-2021 Ostrich Labs
==========================================
*/

#include "gl4_texture.h"

#include "../common/utility.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::GL4Texture::~GL4Texture() {
    if (m_Texture != 0) {
        ::glDeleteTextures(1, &m_Texture);
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::GL4Texture ostrich::GL4Texture::CreateTexture(const ostrich::Image &image, GL4Extensions &ext) {
    if (!image.isValid())
        return ostrich::GL4Texture();

    GLuint tex = 0;
    GLint internalformat = 0;
    GLenum pixelformat = 0;

    ::glGenTextures(1, &tex);
    ::glBindTexture(GL_TEXTURE_2D, tex);

    // these will be better/enhanced in future versions of canary
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    switch (image.getPixelFormat()) {
        case ostrich::PixelFormat::FORMAT_BGR:
        {
            internalformat = GL_RGB;
            pixelformat = GL_BGR;
            break;
        }
        case ostrich::PixelFormat::FORMAT_BGRA:
        {
            internalformat = GL_RGBA;
            pixelformat = GL_BGRA;
            break;
        }
        case ostrich::PixelFormat::FORMAT_RGB:
        {
            internalformat = GL_RGB;
            pixelformat = GL_RGB;
            break;
        }
        case ostrich::PixelFormat::FORMAT_RGBA:
        {
            internalformat = GL_RGBA;
            pixelformat = GL_RGBA;
            break;
        }
        case ostrich::PixelFormat::FORMAT_DXT1:
        {
            internalformat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            pixelformat = GL_RGBA;
            break;
        }
        case ostrich::PixelFormat::FORMAT_DXT3:
        {
            internalformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            pixelformat = GL_RGBA;
            break;
        }
        case ostrich::PixelFormat::FORMAT_DXT5:
        {
            internalformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            pixelformat = GL_RGBA;
            break;
        }
        default:
        {
            return ostrich::GL4Texture();
        }
    }

    auto imgdataptr = image.getData().lock();
    auto *imgdata = imgdataptr.get();

    if (image.isCompressed()) {
        ext.glCompressedTexImage2D(GL_TEXTURE_2D, 0, internalformat, image.getWidth(), image.getHeight(), 0, image.getDataSize(), imgdata);
    }
    else {
        ::glTexImage2D(GL_TEXTURE_2D, 0, internalformat, image.getWidth(), image.getHeight(), 0, pixelformat, GL_UNSIGNED_BYTE, imgdata);
    }

    return ostrich::GL4Texture(ostrich::utility::HashString(image.getFilename()), tex);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::GL4Texture::UnbindTexture() {

}