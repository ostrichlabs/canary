/*
==========================================
Copyright (c) 2020 Ostrich Labs

DDS functions

Supports uncompressed DDS textures with no mipmaps.

Future support for DXTx/S3TC and mipmaps planned.

No plans to support cubemaps (yet)
==========================================
*/

#include "image.h"

#include "filesystem.h"

namespace {

/////////////////////////////////////////////////
// Exactly 128 bytes defined in 4-byte increments, so probably won't have padding
struct DDSHeader {

    uint32_t m_FileCode;            // 0x20534444, or "DDS "

    uint32_t m_Size;                // should be 124
    uint32_t m_Flags;
    uint32_t m_Height;              // largest mipmap
    uint32_t m_Width;               // largest mipmap
    uint32_t m_PitchorLinearSize;   // exact meaning depends on compression
    uint32_t m_VolumeDepth;
    uint32_t m_MipMapCount;
    uint32_t m_Reserved1[11];

    uint32_t m_PixelFormatSize;     // should be 32
    uint32_t m_PixelFormatFlags;    // Tells of presence of extra data in next 28 bytes
    uint32_t m_FourCC;              // DXT1, DXT3, DXT5
    uint32_t m_BitsPerPixel;
    uint32_t m_RedBitMask;
    uint32_t m_GreenBitMask;
    uint32_t m_BlueBitMask;
    uint32_t m_AlphaBitMask;
    uint32_t m_Caps;                // 0x1000 - no mipmaps, 0x401008 - w/ mipmaps, 0x1008 - cubemap
    uint32_t m_Caps2;               // mostly used for cubemaps
    uint32_t m_Caps3;               // unused
    uint32_t m_Caps4;               // unused
    uint32_t m_Reserved2;
};

/////////////////////////////////////////////////
// Determine Pixel Format
//
// in:
//      header - a fully-initialized DDS header
// returns:
//      PixelFormat - one of the valid formats, or FORMAT_NONE if it could not be determined 
/////////////////////////////////////////////////
ostrich::PixelFormat DeterminePixelFormat(const DDSHeader &header) {
    const uint32_t FIRSTMASK = 0xFF00'0000;
    const uint32_t DXT1 = 0x44585431; // "DXT1"
    const uint32_t DXT3 = 0x44585433; // "DXT3"
    const uint32_t DXT5 = 0x44585435; // "DXT5"

    // compressed
    if (header.m_PixelFormatFlags & 0x4) { // DDPF_FOURCC
        switch (header.m_FourCC) {
            case DXT1:
            {
                return ostrich::PixelFormat::FORMAT_DXT1;
            }
            case DXT3:
            {
                return ostrich::PixelFormat::FORMAT_DXT3;
            }
            case DXT5:
            {
                return ostrich::PixelFormat::FORMAT_DXT5;
            }
            default:
            {
                return ostrich::PixelFormat::FORMAT_NONE;
            }
        }
    }

    // uncompressed
    bool hasAlpha = (header.m_PixelFormatFlags & 0x1) ? true : false; // DDPF_ALPHAPIXELS
    if (header.m_BlueBitMask == FIRSTMASK) {
        if (hasAlpha) {
            return ostrich::PixelFormat::FORMAT_BGRA;
        }
        return ostrich::PixelFormat::FORMAT_BGR;
    }

    // this might not be necessary, but I'm doing it anyway
    if (header.m_RedBitMask == FIRSTMASK) {
        if (hasAlpha) {
            return ostrich::PixelFormat::FORMAT_RGBA;
        }
        return ostrich::PixelFormat::FORMAT_RGB;
    }

    return ostrich::PixelFormat::FORMAT_NONE;
}

} // anonymous namespace

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::Image ostrich::Image::LoadDDS(const char *filename) {
    ostrich::File file;
    if (!file.Open(filename, ostrich::FileMode::OPEN_READONLY)) {
        return ostrich::Image();
    }

    std::fstream &handle = file.getFStream();
    DDSHeader header = {};
    handle.read((char *)&header, sizeof(header));
    if (handle.bad()) {
        return ostrich::Image();
    }

    // verify file integrity
    if ((header.m_FileCode != 0x20534444) ||
        (header.m_Size != 124) ||
        (header.m_PixelFormatSize != 32)) {
        return ostrich::Image();
    }

    // verify supported features
    if ((header.m_VolumeDepth > 1) ||       // no cubemaps
        (header.m_MipMapCount > 1) ||       // no mipmaps
        (header.m_Caps != 0x0000'1000)) {   // also no mipmaps/cubemaps
        return ostrich::Image();
    }

    // determine pixel format
    ostrich::PixelFormat pixformat = ::DeterminePixelFormat(header);

    // determine data size
    auto &path = file.getPath();
    auto datasize = std::filesystem::file_size(path) - 128; // TODO: replace magic number with sizeof(header)

    // everything after the header is pixel data regardless of compression
    uint8_t *imgdata = new uint8_t[datasize];
    handle.read((char *)imgdata, datasize);
    if (handle.fail()) {
        delete[] imgdata;
        return ostrich::Image();
    }

    return ostrich::Image(filename, ostrich::ImageType::IMGTYPE_DDS, pixformat, header.m_Width,
        header.m_Height, header.m_BitsPerPixel, static_cast<int32_t>(datasize), imgdata);
}