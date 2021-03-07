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
ostrich::GL4Texture ostrich::GL4Texture::CreateTexture(GL4Extensions &ext, const ostrich::Image &image) {
    if (!image.isValid())
        return ostrich::GL4Texture();

    GLint internalformat = 0;
    GLenum pixelformat = 0;
    if (!ostrich::GL4Texture::GetGLFormats(image.getPixelFormat(), internalformat, pixelformat)) {
        return ostrich::GL4Texture();
    }

    GLuint tex = 0;
    if (ext.DirectStateAccessSupported()) {
        tex = ostrich::GL4Texture::CreateTextureObject(ext, image, internalformat, pixelformat);
    }
    else {
        tex = ostrich::GL4Texture::CreateTextureCore(ext, image, internalformat, pixelformat);
    }

    return ostrich::GL4Texture(ostrich::utility::HashString(image.getFilename()), tex);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::GL4Texture::UnbindTexture() {

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
GLuint ostrich::GL4Texture::CreateTextureCore(GL4Extensions &ext, const ostrich::Image &image, GLint GLinternalformat, GLenum GLpixelformat) {
    GLuint tex = 0;

    ::glGenTextures(1, &tex);
    ::glBindTexture(GL_TEXTURE_2D, tex);

    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    auto imgdataptr = image.getData().lock();
    auto *imgdata = imgdataptr.get();

    ::glTexImage2D(GL_TEXTURE_2D, 0, GLinternalformat, image.getWidth(), image.getHeight(), 0, GLpixelformat, GL_UNSIGNED_BYTE, imgdata);
    ext.glGenerateMipmap(GL_TEXTURE_2D);
    
    ::glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
GLuint ostrich::GL4Texture::CreateTextureObject(GL4Extensions &ext, const ostrich::Image &image, GLint GLinternalformat, GLenum GLpixelformat) {
    GLuint tex = 0;

    ext.glCreateTextures(GL_TEXTURE_2D, 1, &tex);

    ext.glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ext.glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    ext.glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ext.glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    auto imgdataptr = image.getData().lock();
    auto *imgdata = imgdataptr.get();

    ext.glTextureStorage2D(tex, 1, GLinternalformat, image.getWidth(), image.getHeight());
    ext.glTextureSubImage2D(tex, 0, 0, 0, image.getWidth(), image.getHeight(), GLpixelformat, GL_UNSIGNED_BYTE, imgdata);
    ext.glGenerateTextureMipmap(tex);

    return tex;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::GL4Texture::GetGLFormats(ostrich::PixelFormat ostformat, GLint &GLinternalformat, GLenum &GLpixelformat) {
    switch (ostformat) {
        case ostrich::PixelFormat::FORMAT_BGR:
        {
            GLinternalformat = GL_RGB;
            GLpixelformat = GL_BGR;
            break;
        }
        case ostrich::PixelFormat::FORMAT_BGRA:
        {
            GLinternalformat = GL_RGBA;
            GLpixelformat = GL_BGRA;
            break;
        }
        case ostrich::PixelFormat::FORMAT_RGB:
        {
            GLinternalformat = GL_RGB;
            GLpixelformat = GL_RGB;
            break;
        }
        case ostrich::PixelFormat::FORMAT_RGBA:
        {
            GLinternalformat = GL_RGBA;
            GLpixelformat = GL_RGBA;
            break;
        }
        case ostrich::PixelFormat::FORMAT_DXT1:
        {
            GLinternalformat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            GLpixelformat = GL_RGBA;
            break;
        }
        case ostrich::PixelFormat::FORMAT_DXT3:
        {
            GLinternalformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            GLpixelformat = GL_RGBA;
            break;
        }
        case ostrich::PixelFormat::FORMAT_DXT5:
        {
            GLinternalformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            GLpixelformat = GL_RGBA;
            break;
        }
        default:
        {
            return false;
        }
    }
    
    return true;
}