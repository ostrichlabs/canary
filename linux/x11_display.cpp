/*
==========================================
Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "../common/ost_common.h"

#if (OST_LINUX != 1)
#   error "This module should only be included in Linux builds using X11"
#endif

#include "x11_display.h"
#include <GL/glxext.h>
#include <X11/Xutil.h>
#include "../common/error.h"
#include "../game/errorcodes.h"
#include "../gl4/gl4_extensions.h"

bool ostrich::DisplayX11::ms_XError = false;

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::DisplayX11::GLXErrorHandler(Display *display, XErrorEvent *event) {
    ms_XError = true;
    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::DisplayX11::DisplayX11() noexcept :
m_isActive(false), m_FrameBufferConfig(nullptr), m_Display(nullptr),
m_Colormap(0), m_GLWindow(0), m_GLContext(nullptr) {

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::DisplayX11::~DisplayX11() {
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::DisplayX11::Initialize(ostrich::ConsolePrinter consoleprinter) {
    if (this->isActive())
        return OST_ERROR_ISACTIVE;

    m_ConsolePrinter = consoleprinter;
    if (!m_ConsolePrinter.isValid())
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    int result = this->InitWindow();
    if (result != OST_ERROR_OK)
        return result;

    result = this->InitRenderer();
    if (result != OST_ERROR_OK)
        return result;

    m_isActive = true;
    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::DisplayX11::Destroy() {
    if (this->isActive()) {
        ::glXMakeCurrent(m_Display, 0, nullptr);
        ::glXDestroyContext(m_Display, m_GLContext);
        ::XDestroyWindow(m_Display, m_GLWindow);
        ::XFreeColormap(m_Display, m_Colormap);
        ::XCloseDisplay(m_Display);
        m_isActive = false;
    }
    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool ostrich::DisplayX11::SwapBuffers() {
    if (!this->isActive())
        return false;

    ::glXSwapBuffers(m_Display, m_GLWindow);
    return true;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::DisplayX11::InitWindow() {
    if (this->isActive()) {
        return OST_ERROR_ISACTIVE;
    }

    m_Display = ::XOpenDisplay(nullptr);
    if (m_Display == nullptr) {
        return OST_ERROR_X11OPENDISPLAY;
    }

    int glattribs[] = {
        GLX_X_RENDERABLE, True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_DOUBLEBUFFER, True,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_STENCIL_SIZE, 8,
        None
    };

    int glxmajor = 0;
    int glxminor = 0;
    if (!::glXQueryVersion(m_Display, &glxmajor, &glxminor)) {
        return OST_ERROR_GLXQUERYVERSION;
    }
    if (((glxmajor == 1) && (glxminor < 3)) || (glxmajor < 1)) {
        return OST_ERROR_GLXVERSION;
    }

    int fbcount = 0;
    GLXFBConfig *fbconfigs = ::glXChooseFBConfig(m_Display, DefaultScreen(m_Display),
        glattribs, &fbcount);
    if (fbconfigs == nullptr) {
        return OST_ERROR_GLXCHOOSEFBCONFIG;
    }

    // picking the best FB config
    int bestfbc = -1;
    int worstfbc = -1;
    int bestsample = -1;
    int worstsample = 99;
    for (int i = 0; i < fbcount; i++) {
        XVisualInfo *visualinfo = ::glXGetVisualFromFBConfig(m_Display, fbconfigs[i]);
        if (visualinfo != nullptr) {
            int samplebuffer = 0;
            int samples = 0;
            ::glXGetFBConfigAttrib(m_Display, fbconfigs[i], GLX_SAMPLE_BUFFERS, &samplebuffer);
            ::glXGetFBConfigAttrib(m_Display, fbconfigs[i], GLX_SAMPLES, &samples);

            if ((bestfbc < 0) || ((samplebuffer != 0) && (samples > bestsample))) {
                bestfbc = i;
                bestsample = samples;
            }
            if ((worstfbc < 0) || (samplebuffer == 0) || (samples < worstsample)) {
                worstfbc = i;
                worstsample = samples;
            }
        }
        ::XFree(visualinfo);
    }

    m_FrameBufferConfig = fbconfigs[bestfbc];
    ::XFree(fbconfigs);

    XVisualInfo *visual = ::glXGetVisualFromFBConfig(m_Display, m_FrameBufferConfig);
    if (visual == nullptr) {
        return OST_ERROR_GLXGETVISUALFROMFB;
    }

    Window rootwindow = RootWindow(m_Display, visual->screen);
    m_Colormap = ::XCreateColormap(m_Display, rootwindow, visual->visual, AllocNone);

    XSetWindowAttributes xwinattribs = {};
    xwinattribs.colormap = m_Colormap;
    xwinattribs.border_pixel = 0;
    xwinattribs.event_mask = VisibilityChangeMask |                 // Window masks
                             KeyPressMask | KeyReleaseMask |        // Keyboard masks
                             ButtonPressMask | ButtonReleaseMask |  // Mouse masks
                             ButtonMotionMask | PointerMotionMask;

    m_GLWindow = ::XCreateWindow(m_Display, rootwindow, 0, 0, 1920, 1080, 0, visual->depth,
        InputOutput, visual->visual, CWBorderPixel | CWColormap | CWEventMask, &xwinattribs);
    if (m_GLWindow == 0) {
        return OST_ERROR_XCREATEWINDOW;
    }

    ::XFree(visual);

    ::XStoreName(m_Display, m_GLWindow, "Canary Test"); // TODO: take from global constants
    ::XMapWindow(m_Display, m_GLWindow);

    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::DisplayX11::InitRenderer() {
    if (this->isActive()) {
        return OST_ERROR_ISACTIVE;
    }

    // this function isn't needed anywhere else, so it can stay local
    PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB =
        (PFNGLXCREATECONTEXTATTRIBSARBPROC)ostrich::glGetProcAddress("glXCreateContextAttribsARB");

    ms_XError = false;
    int (*oldhandler)(Display *, XErrorEvent *event) = ::XSetErrorHandler(&ostrich::DisplayX11::GLXErrorHandler);

    std::string glxextlist = ::glXQueryExtensionsString(m_Display, DefaultScreen(m_Display));
    bool supportcontext = (glxextlist.find("GLX_ARB_create_context") != std::string::npos);
    bool supportprofile = (glxextlist.find("GLX_ARB_create_context_profile") != std::string::npos);
    if ((supportcontext) && (supportprofile) && (glXCreateContextAttribsARB != nullptr)) {
        int contextattribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
            GLX_CONTEXT_MINOR_VERSION_ARB, 0,
            GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
            None
        };
        m_GLContext = glXCreateContextAttribsARB(m_Display, m_FrameBufferConfig, 0, True, contextattribs);
    }
    else {
        m_GLContext = ::glXCreateNewContext(m_Display, m_FrameBufferConfig, GLX_RGBA_TYPE, 0, True);
    }
    ::XSync(m_Display, false);
    if ((m_GLContext == nullptr) || (ms_XError == true)) {
        return OST_ERROR_GLXCREATECONTEXT;
    }

    ::XSetErrorHandler(oldhandler);

    if (!::glXIsDirect(m_Display, m_GLContext)) {
        return OST_ERROR_GLXINDIRECT;
    }

    if (::glXMakeCurrent(m_Display, m_GLWindow, m_GLContext) == False) {
        return OST_ERROR_GLXMAKECURRENT;
    }

    return OST_ERROR_OK;
}
