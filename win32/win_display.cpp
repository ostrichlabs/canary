/*
==========================================
win_display.cpp

Copyright (c) 2020 Ostrich Labs

IDisplay implementation for Windows
==========================================
*/

#include "win_display.h"

#include <string>
#include "../common/error.h"
#include "win_wndproc.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::WGLExtensions::Retrieve(HDC hdc) {
// WGL_ARB_extensions_string
    std::string extensionlist;
    wglGetExtensionsStringARB =
        PFNWGLGETEXTENSIONSSTRINGARBPROC(::wglGetProcAddress("wglGetExtensionsStringARB"));
    if (wglGetExtensionsStringARB == nullptr)
        return 1;
    m_WGL_ARB_extensions_string = true;
    extensionlist = this->wglGetExtensionsStringARB(hdc);

// WGL_ARB_create_context
    if (extensionlist.find("WGL_ARB_create_context") == std::string::npos)
        return 2;
    m_WGL_ARB_create_context = true;
    wglCreateContextAttribsARB =
        PFNWGLCREATECONTEXTATTRIBSARBPROC(::wglGetProcAddress("wglCreateContextAttribsARB"));

// WGL_ARB_create_context_profile
    if (extensionlist.find("WGL_ARB_create_context_profile") == std::string::npos)
        return 2;
    m_WGL_ARB_create_context_profile = true;

// WGL_ARB_pixel_format
    if (extensionlist.find("WGL_ARB_pixel_format") == std::string::npos)
        return 2;
    m_WGL_ARB_pixel_format = true;
    wglGetPixelFormatAttribivARB =
        PFNWGLGETPIXELFORMATATTRIBIVARBPROC(::wglGetProcAddress("wglGetPixelFormatAttribivARB"));
    wglChoosePixelFormatARB =
        PFNWGLCHOOSEPIXELFORMATARBPROC(::wglGetProcAddress("wglChoosePixelFormatARB"));

    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::DisplayWindows::DisplayWindows() noexcept :
m_isActive(false),
m_GLMajorVersion(0), m_GLMinorVersion(0),
m_WindowClassName(L"Canary"),
m_HInstance(nullptr), m_HWnd(nullptr), m_HDC(nullptr), m_HGLRC(nullptr) {

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::DisplayWindows::~DisplayWindows() {
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::DisplayWindows::Initialize(ostrich::ConsolePrinter conprinter) {
    if (this->isActive())
        return -1;

    m_ConPrinter = conprinter;
    if (!m_ConPrinter.isValid())
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    int result = this->InitWindow();
    if (result != 0)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, result);

    result = this->InitRenderer();
    if (result != 0)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, result);

    m_isActive = true;
    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::DisplayWindows::Destroy() {
    if (this->isActive()) {
        ::wglMakeCurrent(m_HDC, NULL);
        ::wglDeleteContext(m_HGLRC);
        ::ReleaseDC(m_HWnd, m_HDC);
        ::DestroyWindow(m_HWnd);
        ::UnregisterClassW(m_WindowClassName, m_HInstance);
        m_isActive = false;
    }
    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::DisplayWindows::SwapBuffers() {
    if (!this->isActive())
        return false;

    return ::SwapBuffers(m_HDC);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::DisplayWindows::InitWindow() {
    if (this->isActive())
        return -1;

    m_HInstance = ::GetModuleHandleW(NULL);
    WNDCLASSW wndclass = { };
    wndclass.lpfnWndProc = ostrich::WndProc;
    wndclass.hInstance = m_HInstance;
    wndclass.hbrBackground = HBRUSH(COLOR_BACKGROUND);
    wndclass.lpszClassName = m_WindowClassName;
    wndclass.style = CS_OWNDC;
    if (!::RegisterClassW(&wndclass)) {
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, ::GetLastError());
    }

// need to get WGL Extensions first because they use a dummy pixel format
// and you can only set a pixel format once for a given window
// GetWGLExtensions() will create a dummy window and destroy it
    m_WGLExt = this->GetWGLExtensions();

    m_HWnd = ::CreateWindowExW(0, m_WindowClassName,
        L"Canary Test", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        0, 0, ostrich::g_ScreenWidth, ostrich::g_ScreenHeight, 0, 0, m_HInstance, 0);

    if (m_HWnd == nullptr)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, ::GetLastError());

    m_HDC = ::GetDC(m_HWnd);
    if (m_HDC == nullptr)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, ::GetLastError());

    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::DisplayWindows::InitRenderer() {
    const int pixelformatattribs[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_DEPTH_BITS_ARB, 8,
        0
    };

    int pixelfmt = 0;
    UINT numformats = 0;
    if (m_WGLExt.wglChoosePixelFormatARB)
        m_WGLExt.wglChoosePixelFormatARB(m_HDC, pixelformatattribs, nullptr, 1, &pixelfmt, &numformats);
    if (numformats != 1)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, ::GetLastError());

// make sure the pixel format we got is hardware accelerated
    int pixelformatquery = WGL_ACCELERATION_ARB;
    int queryresult = WGL_NO_ACCELERATION_ARB;
    if (m_WGLExt.wglGetPixelFormatAttribivARB)
        m_WGLExt.wglGetPixelFormatAttribivARB(m_HDC, pixelfmt, 0, 1, &pixelformatquery, &queryresult);
    if (queryresult != WGL_FULL_ACCELERATION_ARB)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, ::GetLastError());

    PIXELFORMATDESCRIPTOR pfd = { };
    ::DescribePixelFormat(m_HDC, pixelfmt, sizeof(pfd), &pfd);
    if (::SetPixelFormat(m_HDC, pixelfmt, &pfd) == false)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, ::GetLastError());

    const int contextattribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 0,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    if (m_WGLExt.wglCreateContextAttribsARB)
        m_HGLRC = m_WGLExt.wglCreateContextAttribsARB(m_HDC, nullptr, contextattribs);
    if (m_HGLRC == nullptr)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, ::GetLastError());
    if (::wglMakeCurrent(m_HDC, m_HGLRC) == false)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, ::GetLastError());

    const char *versionstring = (const char *)::glGetString(GL_VERSION);
    if (versionstring == nullptr)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, ::GetLastError());
    if (::strlen(versionstring) >= 3) {
        m_GLMajorVersion = versionstring[0] - '0';
        m_GLMinorVersion = versionstring[2] - '0';
    }

    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::WGLExtensions ostrich::DisplayWindows::GetWGLExtensions() {
// generic pixel format descriptor just to get a hardware context
    PIXELFORMATDESCRIPTOR pfd = { };
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.cAuxBuffers = 0;
    
    HWND wglhwnd = ::CreateWindowExW(0, m_WindowClassName,
        L"Canary WGL Dummy Window", WS_OVERLAPPEDWINDOW,
        0, 0, ostrich::g_ScreenWidth, ostrich::g_ScreenHeight, 0, 0, m_HInstance, 0);

    HDC wglhdc = ::GetDC(wglhwnd);
    int pixelfmt = ::ChoosePixelFormat(wglhdc, &pfd);
    ::SetPixelFormat(wglhdc, pixelfmt, &pfd);

    HGLRC wglrc = ::wglCreateContext(wglhdc);
    ::wglMakeCurrent(wglhdc, wglrc);

    WGLExtensions wglext;
    if (wglext.Retrieve(wglhdc) != 0) {
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 0); // the location is indicative enough
    }

    ::wglMakeCurrent(wglhdc, NULL);
    ::wglDeleteContext(wglrc);
    ::ReleaseDC(wglhwnd, wglhdc);
    ::DestroyWindow(wglhwnd);
    return wglext;
}