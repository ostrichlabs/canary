/*
==========================================
Copyright (c) 2020 Ostrich Labs

IDisplay implementation for the Raspberry Pi
==========================================
*/

#include "raspi_display.h"
#include "../common/error.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::DisplayRaspi::DisplayRaspi() :
m_isActive(false),
m_GLConfig(nullptr), m_GLContext(nullptr), m_GLDisplay(nullptr), m_GLSurface(nullptr),
m_GLMajorVersion(0), m_GLMinorVersion(0),
m_NativeWindow({ 0 }), m_DispmanElement(0), m_DispmanDisplay(0), m_DispmanUpdate(0) {

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::DisplayRaspi::~DisplayRaspi() {
    this->Destroy();
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::DisplayRaspi::Initialize(ostrich::ConsolePrinter conprinter) {
    if (this->isActive())
        return -1;

    m_ConPrinter = conprinter;
    if (!m_ConPrinter.isValid())
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    ::bcm_host_init();

    int result = this->InitWindow();
    if (result != 0) {
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, result);
    }

    result = this->InitRenderer();
    if (result != 0) {
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, result);
    }

    m_isActive = true;
    return result;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::DisplayRaspi::Destroy() {
    if (this->isActive()) {
        ::eglSwapBuffers(m_GLDisplay, m_GLSurface);
        ::eglMakeCurrent(m_GLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        ::eglDestroySurface(m_GLDisplay, m_GLSurface);
        ::eglDestroyContext(m_GLDisplay, m_GLContext);
        ::eglTerminate(m_GLDisplay);
        ::bcm_host_deinit();
        m_isActive = false;
    }
    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::DisplayRaspi::SwapBuffers() {
    if (!this->isActive())
        return false;

    return bool(::eglSwapBuffers(m_GLDisplay, m_GLSurface));
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::DisplayRaspi::InitWindow() {
    uint32_t displayheight = 0;
    uint32_t displaywidth = 0;

    int32_t result = ::graphics_get_display_size(0, &displaywidth, &displayheight);
    if (result < 0)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 1);

    VC_RECT_T destrect;
    destrect.x = 0;
    destrect.y = 0;
    destrect.width = ostrich::g_ScreenWidth;
    destrect.height = ostrich::g_ScreenHeight;
    VC_RECT_T sourcerect;
    sourcerect.x = 0;
    sourcerect.y = 0;
    sourcerect.width = ostrich::g_ScreenWidth << 16;
    sourcerect.height = ostrich::g_ScreenHeight << 16;

    m_DispmanDisplay = ::vc_dispmanx_display_open(0);
    if (m_DispmanDisplay == DISPMANX_NO_HANDLE)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 2);
    m_DispmanUpdate = ::vc_dispmanx_update_start(0);
    if (m_DispmanUpdate == DISPMANX_NO_HANDLE)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 3);
    m_DispmanElement = ::vc_dispmanx_element_add(m_DispmanUpdate, m_DispmanDisplay,
            0, &destrect, 0, &sourcerect, DISPMANX_PROTECTION_NONE,
            0, 0, DISPMANX_NO_ROTATE);
    if (m_DispmanElement == DISPMANX_NO_HANDLE)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 4);

    m_NativeWindow.element = m_DispmanElement;
    m_NativeWindow.height = ostrich::g_ScreenHeight;
    m_NativeWindow.width = ostrich::g_ScreenWidth;
    ::vc_dispmanx_update_submit_sync(m_DispmanUpdate);

    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::DisplayRaspi::InitRenderer() {
    const EGLint configattribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_RED_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_STENCIL_SIZE, 8,
            EGL_DEPTH_SIZE, 8,
            EGL_NONE
    };

    const EGLint contextattribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };

    m_GLDisplay = ::eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (m_GLDisplay == EGL_NO_DISPLAY)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 1);

    EGLBoolean initresult = ::eglInitialize(m_GLDisplay, &m_GLMajorVersion,
            &m_GLMinorVersion);
    if (initresult == EGL_FALSE)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 2);

    EGLint numconfigs = 0;
    if (!::eglChooseConfig(m_GLDisplay, configattribs, &m_GLConfig, 1, &numconfigs))
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 3);

    m_GLSurface = ::eglCreateWindowSurface(m_GLDisplay, m_GLConfig, &m_NativeWindow, NULL);
    if (m_GLSurface == EGL_NO_SURFACE)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 4);

    m_GLContext = ::eglCreateContext(m_GLDisplay, m_GLConfig, EGL_NO_CONTEXT, contextattribs);
    if (m_GLContext == EGL_NO_CONTEXT)
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 5);

    if (!::eglMakeCurrent(m_GLDisplay, m_GLSurface, m_GLSurface, m_GLContext))
        throw ostrich::InitException(OST_FUNCTION_SIGNATURE, 6);

    return 0;
}
