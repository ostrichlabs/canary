/*
==========================================
Copyright (c) 2020 Ostrich Labs

PNG functions
==========================================
*/

#include "image.h"
#include "ost_common.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::Image ostrich::Image::LoadPNG(const char *filename) {
    OST_UNUSED_PARAMETER(filename);
    return ostrich::Image();
}

/*
#include "image.h"
#include "filesystem.h"
#if (OST_DEBUG_BUILD == 1)
#   define PNG_DEBUG 3
#endif
#include <png.h>
#include <zlib.h>

#if (OST_WINDOWS == 1)
#   pragma warning(disable : 4611) // libpng does some wacky stuff with C-style jumps and they're dumb
#endif


/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::Image::Load(const char *filename) {
    m_Filename = filename;

    FILE *handle = nullptr;
    if (!ostrich::OpenFile(filename, ostrich::FileMode::OPEN_READONLY, &handle))
        return false;

    m_Type = this->DetectFileType(handle);

    bool result = false;
    switch (m_Type) {
        case ostrich::ImageType::IMGTYPE_PNG:
        {
            result = this->LoadPNG(handle);
            break;
        }
        default:
        {
            result = false;
        }
    }

    ::fclose(handle);
    return result;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::Image::Destroy() {
    m_Filename = nullptr;
    m_Type = ostrich::ImageType::IMGTYPE_NONE;
    m_Width = 0;
    m_Height = 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::ImageType ostrich::Image::DetectFileType(FILE *handle) {
    if (this->isPNG(handle))
        return ostrich::ImageType::IMGTYPE_PNG;

    return ostrich::ImageType::IMGTYPE_NONE;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::Image::isPNG(FILE *handle) {
    uint8_t header[PNG_SIG_BYTES] = {};
    if (::fread(header, sizeof(header[0]), PNG_SIG_BYTES, handle) != PNG_SIG_BYTES)
        return false;

    return (::png_sig_cmp(header, 0, PNG_SIG_BYTES) == 0);
}

/////////////////////////////////////////////////
// A lot of this code comes from following libpng manual
/////////////////////////////////////////////////
bool ostrich::Image::LoadPNG(FILE *handle) {
    png_structp png_ptr = ::png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr)
        return false;

    png_infop info_ptr = ::png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        ::png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return false;
    }

    png_infop end_ptr = ::png_create_info_struct(png_ptr);
    if (end_ptr == nullptr) {
        ::png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return false;
    }

    if (::setjmp(png_jmpbuf(png_ptr))) {
        ::png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
        return false;
    }

    ::png_init_io(png_ptr, handle);
    ::png_set_sig_bytes(png_ptr, PNG_SIG_BYTES);

    ::png_read_info(png_ptr, info_ptr);

    m_Width = ::png_get_image_width(png_ptr, info_ptr);
    m_Height = ::png_get_image_height(png_ptr, info_ptr);

    if (::setjmp(png_jmpbuf(png_ptr))) {
        ::png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
        return false;
    }

    m_DataSize = int64_t(m_Width) * m_Height * sizeof(png_byte);

    png_bytep *row_pointers = new png_bytep * (sizeof(png_bytep) * m_Height);

    return false;
}


namespace {

constexpr int32_t l_PNGHeaderSize = 8;

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::ImageLoadResult LoadPNGWat(FILE *handle, uint8_t **imgdata) {
    png_structp png_ptr = ::png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr)
        return ostrich::ImageLoadResult::IMG_LIBPNG_MEMORY;

    png_infop pnginfo = ::png_create_info_struct(png_ptr);
    if (pnginfo == nullptr) {
        ::png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return ostrich::ImageLoadResult::IMG_LIBPNG_MEMORY;
    }

    if (::setjmp(png_jmpbuf(png_ptr))) {
        ::png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return ostrich::ImageLoadResult::IMG_LIBPNG_JMP;
    }

    ::png_init_io(png_ptr, handle);
    ::png_set_sig_bytes(png_ptr, l_PNGHeaderSize);

    png_infop pnginfo_ptr = nullptr;
    ::png_read_info(png_ptr, pnginfo_ptr);

    auto width = ::png_get_image_width(png_ptr, pnginfo_ptr);
    auto height = ::png_get_image_height(png_ptr, pnginfo_ptr);
    auto colortype = ::png_get_color_type(png_ptr, pnginfo_ptr);
    auto bitdepth = ::png_get_bit_depth(png_ptr, pnginfo_ptr);

    uint64_t allocbytes = 0;

    if (::setjmp(png_jmpbuf(png_ptr))) {
        ::png_destroy_info_struct(png_ptr, &pnginfo_ptr);
        ::png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return ostrich::ImageLoadResult::IMG_LIBPNG_JMP;
    }

    *imgdata = new uint8_t(sizeof(png_bytep) * height * width);

    return ostrich::ImageLoadResult::IMG_OK;
}

} // anonymous namespace

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::ImageLoadResult ostrich::LoadImage(const char *filename, uint8_t **imgdata) {
    FILE *handle = nullptr;
    if (!ostrich::OpenFile(filename, ostrich::FileMode::OPEN_READONLY, &handle))
        return ostrich::ImageLoadResult::IMG_FILE_NOT_FOUND;

    uint8_t header[l_PNGHeaderSize] = { };
    if (::fread(header, sizeof(header[0]), l_PNGHeaderSize, handle) != l_PNGHeaderSize)
        return ostrich::ImageLoadResult::IMG_READERROR;

    auto isPNG = !::png_sig_cmp(header, 0, 8);
    if (isPNG) {
        return ::LoadPNGWat(handle, imgdata);
    }

    return ostrich::ImageLoadResult::IMG_OK;
}
*/