/*
==========================================
Copyright (c) 2020 Ostrich Labs

Standard file system functions
==========================================
*/

#ifndef OSTRICH_FILESYSTEM_H_
#define OSTRICH_FILESYSTEM_H_

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string_view>

namespace ostrich {

/////////////////////////////////////////////////
// Constants for file mode
// Use these instead of the more implementation-specific stuff below
enum class FileMode : int32_t {
    OPEN_READONLY = 0,
    OPEN_WRITEONLY,
    OPEN_WRITETRUNCATE,
    OPEN_WRITEAPPEND,
    OPEN_MAXMODE
};

/////////////////////////////////////////////////
// Wrapper function for opening a file using C-style FILE handles
// Largely untested for now, because it's only meant for use with the PNG loader
//
// in:
//      filename - a string_view with the file/path+file name. May be UTF-8 or UTF-16 encoded.
//      mode - a constant specifying the file mode to open with (see definition of FileMode)
//      filehandle - a C-style FILE handle
// returns:
//      true/false whether or not the open was successful
bool OpenFile(const std::string_view filename, FileMode mode, FILE **filehandle);

/////////////////////////////////////////////////
// Collection of file objects
// Isn't really intended to be a wrapper, just a convenient pass-around
// If you want to do fileops, get the fstream reference.
// If you want file information, get the path reference.
// If you want neither, why are you using this object?
class File {
public:

    /////////////////////////////////////////////////
    // Constructor is default; all data is default constructible. Use Open() to "construct"
    // Destructor can do nothing because all data has their own destructors
    // fstream cannot be copied, so copy constructor/operator is deleted
    File() noexcept { }
    virtual ~File() { }
    File(File &&) = default;
    File(const File &) = delete;
    File &operator=(File &&) = default;
    File &operator=(const File &) = delete;

    /////////////////////////////////////////////////
    // Open a file and get path information
    //
    // in:
    //      filename - a string_view with the file/path+file name. May be UTF-8 or UTF-16 encoded. 
    //      mode - a constant specifying the file mode to open with (see definition of FileMode)
    // returns:
    //      true/false whether or not the open was successful
    bool Open(const std::string_view filename, ostrich::FileMode mode);

    /////////////////////////////////////////////////
    // Manually close the file
    //
    // returns:
    //      void
    void Close();

    /////////////////////////////////////////////////
    // Check whether or not the file is open
    //
    // returns:
    //      true/false depending on if file handle is open
    bool isOpen() const noexcept { return m_Handle.is_open(); }

    /////////////////////////////////////////////////
    // accessor methods
    // return references; you can get creative if you don't want to use reference objects (ex. file.getFStream().read())
    /////////////////////////////////////////////////

    std::fstream &getFStream() noexcept { return m_Handle; }
    const std::filesystem::path &getPath() const noexcept { return m_Path; }

private:

    std::fstream m_Handle;
    std::filesystem::path m_Path;
};

} // namespace ostrich

#endif /* OSTRICH_FILESYSTEM_H_ */
