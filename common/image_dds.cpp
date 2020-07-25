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
    uint32_t m_FileFormat;          // DXT1, DXT3, DXT5
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
        (header.m_FileFormat != 0) ||       // no compression
        (header.m_Caps != 0x0000'1000)) {   // also no mipmaps/cubemaps
        return ostrich::Image();
    }

    // determine file size
    //handle.seekg(std::ios_base::end);
    //uint64_t filesize = (uint64_t(handle.tellg()) - sizeof(DDSHeader));

    return ostrich::Image();
}