/*
==========================================
Copyright (c) 2020 Ostrich Labs

IDisplay implementation for Windows
==========================================
*/

#ifndef OSTRICH_WIN_DISPLAY_H_
#define OSTRICH_WIN_DISPLAY_H_

#include <windows.h>
#include <gl/GL.h>
#include "../external/gl/glcorearb.h"
#include "../external/gl/glext.h"
#include "../external/gl/wgl.h"
#include "../external/gl/wglext.h"
#include "../game/i_display.h"

namespace ostrich {

/////////////////////////////////////////////////
//
struct WGLExtensions {

    /////////////////////////////////////////////////
    // Constructor is defined but simple.
    // Destructor is defined but does nothing.
    // Copy/move constructors/operators are default - data is simple
    WGLExtensions() noexcept :
        m_WGL_ARB_create_context_profile(false),
        wglGetExtensionsStringARB(nullptr),
        wglCreateContextAttribsARB(nullptr),
        wglChoosePixelFormatARB(nullptr),
        wglGetPixelFormatAttribivARB(nullptr)
    { }
    ~WGLExtensions() {}
    WGLExtensions(WGLExtensions &&) = default;
    WGLExtensions(const WGLExtensions &) = default;
    WGLExtensions &operator=(WGLExtensions &&) = default;
    WGLExtensions &operator=(const WGLExtensions &) = default;

    /////////////////////////////////////////////////
    // Get function pointers for required WGL extensions
    //
    // in:
    //      hdc - temporary GL context
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int Retrieve(HDC hdc);

    bool m_WGL_ARB_create_context_profile;
    PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
    PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
};

/////////////////////////////////////////////////
//
class DisplayWindows : public IDisplay {
public:

    /////////////////////////////////////////////////
    // Constructor is defined but simple.
    // Destructor is defined but does nothing so I don't know if I should bother.
    // Copy/move constructors/operators are deleted to prevent accidentally creating two windows
    // (if you want to make another, you can do it manually)
    DisplayWindows() noexcept;
    virtual ~DisplayWindows();
    DisplayWindows(DisplayWindows &&) = delete;
    DisplayWindows(const DisplayWindows &) = delete;
    DisplayWindows &operator=(DisplayWindows &&) = delete;
    DisplayWindows &operator=(const DisplayWindows &) = delete;

    /////////////////////////////////////////////////
    // Initialize the display.
    //
    // in:
    //      consoleprinter - an initialized ConsolePrinter for logging
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int Initialize(ConsolePrinter conprinter) override;

    /////////////////////////////////////////////////
    // Cleans up window contexts and handles.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int Destroy() override;

    /////////////////////////////////////////////////
    // Check if the object is valid (by checking the m_isActive flag).
    // I suppose I could have made the m_isActive flag a part of the interface, but I didn't (TODO: should I?)
    //
    // returns:
    //      m_isActive flag
    bool isActive() const override { return m_isActive; }

    /////////////////////////////////////////////////
    // Calls SwapBuffers()
    //
    // returns:
    //      true/false if operation was successful
    bool SwapBuffers() override;

private:

    /////////////////////////////////////////////////
    // Helper method to initialize a Windows display.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int InitWindow() override;

    /////////////////////////////////////////////////
    // Helper method to initialize the renderer.
    // Involves creating a dummy OpenGL context to retrieve WGL extensions and *then* create a real context.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int InitRenderer() override;

    /////////////////////////////////////////////////
    // Retrieves WGL extensions for window creation.
    // This is so important that a failure to create the dummy context should actually throw.
    //
    // returns:
    //      a constructed WGLExtensions object
    WGLExtensions GetWGLExtensions();

    bool m_isActive;

    ConsolePrinter  m_ConsolePrinter;

    WGLExtensions m_WGLExt;

    LPCWSTR     m_WindowClassName;
    HINSTANCE   m_HInstance;
    HWND        m_HWnd;
    HDC         m_HDC;
    HGLRC       m_HGLRC;
};

} // namespace ostrich

#endif /* OSTRICH_WIN_DISPLAY_H */