/*
==========================================
Copyright (c) 2020 Ostrich Labs

Image loading functions.

Nothing here should be tied to a specific library. Look at x_image modules to see the implementations.
For now, output should be the same for all three: a single-dimensional array.
That might get tricksy with PNG since libpng uses a weird array-of-pointers setup.
==========================================
*/

#ifndef IMAGE_H_
#define IMAGE_H_

#include <cstddef>
#include <memory>
#include <string_view>

namespace ostrich {

/////////////////////////////////////////////////
//
enum class ImageType : int32_t {
    IMGTYPE_NONE = 0,
    IMGTYPE_PNG,
    IMGTYPE_TGA,
    IMGTYPE_DDS,
    IMGTYPE_MAX
};

/////////////////////////////////////////////////
//
enum class PixelFormat : int32_t {
    FORMAT_NONE = 0,
    FORMAT_RGB,
    FORMAT_RGBA,
    FORMAT_MAX
};

/////////////////////////////////////////////////
//
class Image {
public:

    virtual ~Image() { }
    Image(Image &&) = default;
    Image(const Image &) = default;
    Image &operator=(Image &&) = default;
    Image &operator=(const Image &) = default;

    // Currently single texture, will add texture compression in the future (and mipmaps when canary goes ~*3D*~)
    static Image LoadDDS(const char *filename);

    // Single texture, uncompressed (RLE maybe later)
    static Image LoadTGA(const char *filename);

    // Probably not going to be fully implemented anytime soon
    static Image LoadPNG(const char *filename);

    bool isValid() const noexcept { return ((m_Type != ostrich::ImageType::IMGTYPE_NONE) ? true : false); }

    std::string_view getFilename() const noexcept { return m_Filename; }
    ImageType getType() const noexcept { return m_Type; }
    PixelFormat getPixelFormat() const noexcept { return m_Format; }
    int32_t getWidth() const noexcept { return m_Width; }
    int32_t getHeight() const noexcept { return m_Height; }
    int32_t getBitsPerPixel() const noexcept { return m_Depth; }
    std::weak_ptr<uint8_t[]> getData() const noexcept { return m_Data; }

private:
    
    Image() noexcept :
        m_Filename(""), m_Type(ostrich::ImageType::IMGTYPE_NONE), m_Format(ostrich::PixelFormat::FORMAT_NONE),
        m_Width(0), m_Height(0), m_Depth(0), m_Data(nullptr) {}

    Image(const char *filename, ImageType type, PixelFormat format, int32_t width, int32_t height, int32_t depth, uint8_t data[]) noexcept :
        m_Filename(filename), m_Type(type), m_Format(format), m_Width(width), m_Height(height), m_Depth(depth), m_Data(data) {}

    const char *m_Filename;

    ImageType m_Type;
    PixelFormat m_Format;

    int32_t m_Width;
    int32_t m_Height;
    int32_t m_Depth;

    std::shared_ptr<uint8_t[]> m_Data;
};

} // namespace ostrich

#endif /* IMAGE_H_ */