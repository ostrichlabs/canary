/*
==========================================
Copyright (c) 2020 Ostrich Labs

Image loading functions.

Nothing here should be tied to a specific library. Look at x_image modules to see the implementations.
For now, output should be the same for all three: a single-dimensional array.
That might get tricksy with PNG since libpng uses a weird array-of-pointers setup.
==========================================
*/

#ifndef OSTRICH_IMAGE_H_
#define OSTRICH_IMAGE_H_

#include <cstddef>
#include <memory>
#include <string_view>

namespace ostrich {

/////////////////////////////////////////////////
// Supported image types
enum class ImageType : int32_t {
    IMGTYPE_NONE = 0,
    IMGTYPE_PNG,
    IMGTYPE_TGA,
    IMGTYPE_DDS,
    IMGTYPE_MAX
};

/////////////////////////////////////////////////
// Expected pixel formats
enum class PixelFormat : int32_t {
    FORMAT_NONE = 0,
    FORMAT_RGB,     // TGA
    FORMAT_RGBA,    // TGA
    FORMAT_BGR,     // DDS
    FORMAT_BGRA,    // DDS

    FORMAT_COMPRESSED_START = 100,
    FORMAT_DXT1,
    FORMAT_DXT3,
    FORMAT_DXT5,
    FORMAT_COMPRESSED_END = 199,

    FORMAT_MAX
};

/////////////////////////////////////////////////
// Raw image data, including statistics
// Data should be immutable once constructed
// 
// If an image load failed, the type is IMGTYPE_NONE
class Image {
public:

    /////////////////////////////////////////////////
    // Constructors are all private; use the static factory methods to load Images
    // Destructor can do nothing because all data is either simple or a smart pointer
    // Data is all either simple or copyable, so copy/move constructors/operators are default
    virtual ~Image() { }
    Image(Image &&) = default;
    Image(const Image &) = default;
    Image &operator=(Image &&) = default;
    Image &operator=(const Image &) = default;

    /////////////////////////////////////////////////
    // Load DDS file into memory
    // Currently single texture, will add texture compression and mipmaps in the future (if canary evolves that far)
    //
    // in:
    //      filename - A name or path+name to a DDS image file
    // returns:
    //      A constructed Image object
    static Image LoadDDS(const char *filename);

    /////////////////////////////////////////////////
    // Load TGA file into memory
    // Currently single text, uncompressed, will add RLE compression in the future (if I care enough)
    //
    // in:
    //      filename - A name or path+name to a TGA image file
    // returns:
    //      A constructed Image object
    static Image LoadTGA(const char *filename);

    /////////////////////////////////////////////////
    // Load PNG file into memory
    // Probably not going to be fully implemented anytime soon, because it's way more complicated than the above two
    //
    // Currently unimplemented
    //
    // in:
    //      filename - A name or path+name to a PNG image file
    // returns:
    //      A constructed Image object with ImgType IMGTYPE_NONE (because unimplemented)
    static Image LoadPNG(const char *filename);

    /////////////////////////////////////////////////
    // Check if the object is valid
    // Uses image type as shorthand for a valid image, assuming the image type is immutable and properly set in every factory method
    //
    // returns:
    //      true/false depending on ImageType
    bool isValid() const noexcept { return ((m_Type != ostrich::ImageType::IMGTYPE_NONE) ? true : false); }

    /////////////////////////////////////////////////
    // Check if the image data is a pre-compressed format
    //
    // returns:
    //      true/false depending on PixelFormat
    bool isCompressed() const noexcept
    { return ((m_Format > ostrich::PixelFormat::FORMAT_COMPRESSED_START) && (m_Format < ostrich::PixelFormat::FORMAT_COMPRESSED_END)); }

    /////////////////////////////////////////////////
    // accessor methods
    // data is returned as a weak_ptr so ownership remains with the Image object
    /////////////////////////////////////////////////

    std::string_view getFilename() const noexcept { return m_Filename; }
    ImageType getType() const noexcept { return m_Type; }
    PixelFormat getPixelFormat() const noexcept { return m_Format; }
    int32_t getWidth() const noexcept { return m_Width; }
    int32_t getHeight() const noexcept { return m_Height; }
    int32_t getBitsPerPixel() const noexcept { return m_Depth; }
    int32_t getDataSize() const noexcept { return m_DataSize; }
    std::weak_ptr<uint8_t[]> getData() const noexcept { return m_Data; }

private:
    
    /////////////////////////////////////////////////
    // Default constructor, when no valid image data is available
    Image() noexcept :
        m_Filename(""), m_Type(ostrich::ImageType::IMGTYPE_NONE), m_Format(ostrich::PixelFormat::FORMAT_NONE),
        m_Width(0), m_Height(0), m_Depth(0), m_DataSize(0), m_Data(nullptr) {}

    /////////////////////////////////////////////////
    // Regular constructor, when image data is available.
    // Should set all data fields.
    // All data is immutable once set here.
    Image(const char *filename, ImageType type, PixelFormat format, int32_t width, int32_t height, int32_t depth, int32_t datasize, uint8_t data[]) noexcept :
        m_Filename(filename), m_Type(type), m_Format(format), m_Width(width), m_Height(height), m_Depth(depth), m_DataSize(datasize), m_Data(data) {}

    const char *m_Filename;

    ImageType m_Type;
    PixelFormat m_Format;

    int32_t m_Width;
    int32_t m_Height;
    int32_t m_Depth;

    int32_t m_DataSize;
    std::shared_ptr<uint8_t[]> m_Data;
};

} // namespace ostrich

#endif /* OSTRICH_IMAGE_H_ */