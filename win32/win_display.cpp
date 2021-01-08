/*
==========================================
Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "../common/ost_common.h"

#if (OST_WINDOWS != 1)
#   error "This module should only be included in Windows builds"
#endif

#include "win_display.h"
#include <string>
#include "win_wndproc.h"
#include "../common/error.h"
#include "../game/errorcodes.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::WGLExtensions::Retrieve(HDC hdc) {
// WGL_ARB_extensions_string
    std::string extensionlist;
    wglGetExtensionsStringARB =
        PFNWGLGETEXTENSIONSSTRINGARBPROC(::wglGetProcAddress("wglGetExtensionsStringARB"));
    if (wglGetExtensionsStringARB == nullptr)
        return OST_ERROR_WGLGETPROCADDR;
    extensionlist = this->wglGetExtensionsStringARB(hdc);

// WGL_ARB_create_context
    if (extensionlist.find("WGL_ARB_create_context") == std::string::npos)
        return OST_ERROR_WGLGETPROCADDR;
    wglCreateContextAttribsARB =
        PFNWGLCREATECONTEXTATTRIBSARBPROC(::wglGetProcAddress("wglCreateContextAttribsARB"));

// WGL_ARB_create_context_profile
    if (extensionlist.find("WGL_ARB_create_context_profile") == std::string::npos)
        return OST_ERROR_WGLGETPROCADDR;
    m_WGL_ARB_create_context_profile = true;

// WGL_ARB_pixel_format
    if (extensionlist.find("WGL_ARB_pixel_format") == std::string::npos)
        return OST_ERROR_WGLGETPROCADDR;
    wglGetPixelFormatAttribivARB =
        PFNWGLGETPIXELFORMATATTRIBIVARBPROC(::wglGetProcAddress("wglGetPixelFormatAttribivARB"));
    wglChoosePixelFormatARB =
        PFNWGLCHOOSEPIXELFORMATARBPROC(::wglGetProcAddress("wglChoosePixelFormatARB"));

    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::DisplayWindows::DisplayWindows() noexcept :
m_isActive(false),
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
        return OST_ERROR_ISACTIVE;

    m_ConsolePrinter = conprinter;
    if (!m_ConsolePrinter.isValid())
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    int result = this->InitWindow();
    if (result != OST_ERROR_OK)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, result, std::make_pair(true, ::GetLastError()));

    result = this->InitRenderer();
    if (result != OST_ERROR_OK)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, result, std::make_pair(true, ::GetLastError()));

    m_isActive = true;
    return OST_ERROR_OK;
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
    return OST_ERROR_OK;
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
        return OST_ERROR_ISACTIVE;

    m_HInstance = ::GetModuleHandleW(NULL);
    WNDCLASSW wndclass = { };
    wndclass.style = 0;
    wndclass.lpfnWndProc = WNDPROC(ostrich::WndProc);
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = m_HInstance;
    wndclass.hIcon = 0;
    wndclass.hCursor = 0;
    wndclass.hbrBackground = (HBRUSH)COLOR_GRAYTEXT;
    wndclass.lpszMenuName = 0;
    wndclass.lpszClassName = m_WindowClassName;
    if (!::RegisterClassW(&wndclass)) {
        return OST_ERROR_WINREGISTERCLASS;
    }

// need to get WGL Extensions first because they use a dummy pixel format
// and you can only set a pixel format once for a given window
// GetWGLExtensions() will create a dummy window and destroy it
    m_WGLExt = this->GetWGLExtensions();

    m_HWnd = ::CreateWindowExW(0, m_WindowClassName,
        L"Canary Test", WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,         // WS_SYSMENU is for windowed
        0, 0, ostrich::g_ScreenWidth, ostrich::g_ScreenHeight, 0, 0, m_HInstance, 0); // WS_POPUP is for fullscreen

    if (m_HWnd == nullptr)
        return OST_ERROR_WINCREATEWINDOW;

    m_HDC = ::GetDC(m_HWnd);
    if (m_HDC == nullptr)
        return OST_ERROR_WINGETDC;

    return OST_ERROR_OK;
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
        return OST_ERROR_WINGLCHOOSEFORMAT;

// make sure the pixel format we got is hardware accelerated
    int pixelformatquery = WGL_ACCELERATION_ARB;
    int queryresult = WGL_NO_ACCELERATION_ARB;
    if (m_WGLExt.wglGetPixelFormatAttribivARB)
        m_WGLExt.wglGetPixelFormatAttribivARB(m_HDC, pixelfmt, 0, 1, &pixelformatquery, &queryresult);
    if (queryresult != WGL_FULL_ACCELERATION_ARB)
        return OST_ERROR_WINGLGETFORMAT;

    PIXELFORMATDESCRIPTOR pfd = { };
    ::DescribePixelFormat(m_HDC, pixelfmt, sizeof(pfd), &pfd);
    if (::SetPixelFormat(m_HDC, pixelfmt, &pfd) == false)
        return OST_ERROR_WINSETFORMAT;

    const int contextattribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 0,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    if (m_WGLExt.wglCreateContextAttribsARB)
        m_HGLRC = m_WGLExt.wglCreateContextAttribsARB(m_HDC, nullptr, contextattribs);
    if (m_HGLRC == nullptr)
        return OST_ERROR_WINGLCREATECONTEXT;
    if (::wglMakeCurrent(m_HDC, m_HGLRC) == false)
        return OST_ERROR_WINGLMAKECURRENT;

    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::WGLExtensions ostrich::DisplayWindows::GetWGLExtensions() {
// generic descriptions just to get a dummy window
    LPCWSTR tmpclassname = L"WGLDummyClassName";
    WNDCLASSW wndclass = { };
    wndclass.lpfnWndProc = ostrich::WndProc;
    wndclass.hInstance = m_HInstance;
    wndclass.lpszClassName = tmpclassname;
    wndclass.style = CS_OWNDC;
    ::RegisterClassW(&wndclass);

    PIXELFORMATDESCRIPTOR pfd = { };
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.cAuxBuffers = 0;
    
    HWND wglhwnd = ::CreateWindowExW(0, tmpclassname,
        L"Canary WGL Dummy Window", WS_OVERLAPPEDWINDOW,
        0, 0, 1, 1, NULL, NULL, m_HInstance, NULL);

    HDC wglhdc = ::GetDC(wglhwnd);
    int pixelfmt = ::ChoosePixelFormat(wglhdc, &pfd);
    ::SetPixelFormat(wglhdc, pixelfmt, &pfd);

    HGLRC wglrc = ::wglCreateContext(wglhdc);
    ::wglMakeCurrent(wglhdc, wglrc);

    WGLExtensions wglext;
    int result = wglext.Retrieve(wglhdc);
    if (result != OST_ERROR_OK) {
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, result); // have to throw from here
    }

    ::wglMakeCurrent(wglhdc, NULL);
    ::wglDeleteContext(wglrc);
    ::ReleaseDC(wglhwnd, wglhdc);
    ::DestroyWindow(wglhwnd);
    return wglext;
}