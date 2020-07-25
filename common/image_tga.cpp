/*
==========================================
Copyright (c) 2020 Ostrich Labs

TGA functions

Supported image types:

2 - uncompressed truecolor (16, 24, or 32)
3 - uncompressed grayscale

Future supported planned but not implemented (and low priority):

10 - RLE truecolor (24 or 32)
11 - RLE grayscale
==========================================
*/

#include "image.h"
#include <cstring>
#include <fstream>
#include "filesystem.h"

namespace {

/////////////////////////////////////////////////
//
struct TGAHeader {

    // size on disk (without padding)
    static const int SIZE = 18;

    TGAHeader() = delete;
    ~TGAHeader() {}
    TGAHeader(TGAHeader &&) = default;
    TGAHeader(const TGAHeader &) = default;
    TGAHeader &operator=(TGAHeader &&) = default;
    TGAHeader &operator=(const TGAHeader &) = default;

    TGAHeader(uint8_t data[TGAHeader::SIZE]);

    uint8_t     m_IDLength;
    uint8_t     m_ColorMapType;
    uint8_t     m_ImageType;

    uint16_t    m_ColorMapIndex;
    uint16_t    m_ColorMapLength;
    uint8_t     m_ColorMapBitsPerPixel;

    uint16_t    m_XOrigin;
    uint16_t    m_YOrigin;
    uint16_t    m_Width;
    uint16_t    m_Height;
    uint8_t     m_BitsPerPixel;
    uint8_t     m_ImageDescriptor;
};

/////////////////////////////////////////////////
//
struct TGAFooter {

    // size on disk (without padding)
    static const int SIZE = 26;

    TGAFooter() = delete;
    ~TGAFooter() {}
    TGAFooter(TGAFooter &&) = default;
    TGAFooter(const TGAFooter &) = default;
    TGAFooter &operator=(TGAFooter &&) = default;
    TGAFooter &operator=(const TGAFooter &) = default;

    TGAFooter(uint8_t data[TGAFooter::SIZE]);

    uint32_t    m_ExtensionOffset;
    uint32_t    m_DeveloperAreaOffset;
    uint8_t     m_Signature[18]; // includes period and null
};

/////////////////////////////////////////////////
//
struct TGAPacket24 {
    uint8_t m_Type : 1;
    uint8_t m_Count : 7;
    uint8_t m_Pixel[3];
};

/////////////////////////////////////////////////
//
struct TGAPacket32 {
    uint8_t m_Type : 1;
    uint8_t m_Count : 7;
    uint8_t m_Pixel[4];
};

} // anonymous namespace

/////////////////////////////////////////////////
/////////////////////////////////////////////////
TGAHeader::TGAHeader(uint8_t data[TGAHeader::SIZE]) {
    m_IDLength = data[0];
    m_ColorMapType = data[1];
    m_ImageType = data[2];

    std::memcpy(&m_ColorMapIndex, &data[3], sizeof(m_ColorMapIndex));
    std::memcpy(&m_ColorMapLength, &data[5], sizeof(m_ColorMapLength));
    m_ColorMapBitsPerPixel = data[7];

    std::memcpy(&m_XOrigin, &data[8], sizeof(m_XOrigin));
    std::memcpy(&m_YOrigin, &data[10], sizeof(m_YOrigin));
    std::memcpy(&m_Width, &data[12], sizeof(m_Width));
    std::memcpy(&m_Height, &data[12], sizeof(m_Height));
    m_BitsPerPixel = data[16];
    m_ImageDescriptor = data[17];
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
TGAFooter::TGAFooter(uint8_t data[TGAFooter::SIZE]) {
    std::memcpy(&m_ExtensionOffset, &data[0], sizeof(m_ExtensionOffset));
    std::memcpy(&m_DeveloperAreaOffset, &data[4], sizeof(m_DeveloperAreaOffset));
    std::memcpy(&m_Signature, &data[8], sizeof(m_Signature));
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::Image ostrich::Image::LoadTGA(const char *filename) {
    ostrich::File file;
    if (!file.Open(filename, ostrich::FileMode::OPEN_READONLY)) {
        return ostrich::Image();
    }
    std::fstream &handle = file.getFStream();

    uint8_t headerdata[TGAHeader::SIZE] = { };
    handle.read((char *)&headerdata, TGAHeader::SIZE);
    if (handle.bad()) {
        return ostrich::Image();
    }
    TGAHeader header(headerdata);

    uint8_t footerdata[TGAFooter::SIZE] = { };
    handle.seekg((0 - TGAFooter::SIZE), std::ios_base::end);
    handle.read((char *)&footerdata, TGAFooter::SIZE);
    if (handle.bad()) {
        return ostrich::Image();
    }
    TGAFooter footer(footerdata);

    // color maps unsupported (for now)
    if (header.m_ColorMapType != 0) {
        return ostrich::Image();
    }

    // don't support RLE yet
    bool isRLE = ((header.m_ImageType & 0x0008) > 0) ? true : false;
    uint8_t imagetype = header.m_ImageType & 0x0007;
    if (((imagetype != 2) && (imagetype != 3)) || (isRLE)) {
        return ostrich::Image();
    }

    ostrich::PixelFormat pixformat = ostrich::PixelFormat::FORMAT_RGB;
    uint8_t alphabits = header.m_ImageDescriptor & 0x0F;
    if (alphabits > 0) {
        pixformat = ostrich::PixelFormat::FORMAT_RGBA;
    }

    // Detecting TGA version - this only means something once RLE is supported, maybe
    int TGAversion = 1;
    if (::memcmp(&footer.m_Signature, "TRUEVISION-XFILE.", sizeof(footer.m_Signature)) == 0) {
        TGAversion = 2;
    }

    handle.seekg(TGAHeader::SIZE, std::ios_base::beg);

    // this is the UNPACKED data size
    uint64_t datasize = static_cast<uint64_t>(header.m_Height) * header.m_Width * header.m_BitsPerPixel;
    uint8_t *imgdata = nullptr;

    if (!isRLE) {
        imgdata = new uint8_t[datasize];
        handle.read((char *)imgdata, datasize);
    }
    else {
        return ostrich::Image();
    }

    return ostrich::Image(filename, ostrich::ImageType::IMGTYPE_TGA, pixformat, header.m_Width,
        header.m_Height, header.m_BitsPerPixel, imgdata);
}