/*
==========================================
Copyright (c) 2020 Ostrich Labs

IDisplay implementation for Windows
==========================================
*/

#ifndef WIN_DISPLAY_H_
#define WIN_DISPLAY_H_

#include "../common/ost_common.h"

#if (OST_WINDOWS != 1)
#    error "This module should only be included in Windows builds"
#endif

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
    WGLExtensions() noexcept :
        m_WGL_ARB_extensions_string(false),
        m_WGL_ARB_create_context(false),
        m_WGL_ARB_create_context_profile(false),
        m_WGL_ARB_pixel_format(false),
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

    int Retrieve(HDC hdc);

    bool m_WGL_ARB_extensions_string;
    bool m_WGL_ARB_create_context;
    bool m_WGL_ARB_create_context_profile;
    bool m_WGL_ARB_pixel_format;
    PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
    PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
};

/////////////////////////////////////////////////
// TODO: These probably belong in the renderer class
//       Better yet, loading methods should be platform specific but still expose the same functions
struct GLExtensions {
    GLExtensions() noexcept {}
    ~GLExtensions() {}
    GLExtensions(GLExtensions &&) = default;
    GLExtensions(const GLExtensions &) = default;
    GLExtensions &operator=(GLExtensions &&) = default;
    GLExtensions &operator=(const GLExtensions &) = default;


};

/////////////////////////////////////////////////
//
class DisplayWindows : public IDisplay {
public:

    DisplayWindows() noexcept;
    virtual ~DisplayWindows();
    DisplayWindows(DisplayWindows &&) = delete;
    DisplayWindows(const DisplayWindows &) = delete;
    DisplayWindows &operator=(DisplayWindows &&) = delete;
    DisplayWindows &operator=(const DisplayWindows &) = delete;

    bool isActive() const override { return m_isActive; }

    int Initialize(ConsolePrinter conprinter) override;
    int Destroy() override;

    bool SwapBuffers() override;

private:

    int InitWindow() override;
    int InitRenderer() override;

// retrieving WGL extensions for window creation
    WGLExtensions GetWGLExtensions();

    bool m_isActive;

    ConsolePrinter  m_ConPrinter;

    GLint   m_GLMajorVersion;
    GLint   m_GLMinorVersion;

    WGLExtensions m_WGLExt;

    LPCWSTR     m_WindowClassName;
    HINSTANCE   m_HInstance;
    HWND        m_HWnd;
    HDC         m_HDC;
    HGLRC       m_HGLRC;
};

} // namespace ostrich

#endif /* WIN_DISPLAY_H */