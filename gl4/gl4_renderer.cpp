/*
==========================================
Copyright (c) 2020-2021 Ostrich Labs

OpenGL 4.0 interface
==========================================
*/

#include "gl4_renderer.h"
#include "../common/error.h"
#include "../game/errorcodes.h"

ostrich::ConsolePrinter ostrich::GL4Renderer::ms_DebugPrinter;

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::GL4Renderer::GL4Renderer() noexcept : m_isActive(false), m_DebugContext(false) {

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::GL4Renderer::~GL4Renderer() {
    // not allocating memory yet, no need to implement destructor
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::GL4Renderer::Initialize(ostrich::ConsolePrinter conprinter) {
    if (this->isActive())
        return OST_ERROR_ISACTIVE;

    m_ConsolePrinter = conprinter;
    if (!m_ConsolePrinter.isValid())
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    int result = this->CheckCaps();
    if (result != OST_ERROR_OK) {
        m_ConsolePrinter.WriteMessage(u8"Last GL Error: %", { std::to_string(::glGetError()) });
        return result;
    }

    // loading extensions
    result = m_Ext.Load(m_ConsolePrinter);
    if (result != OST_ERROR_OK) {
        return result;
    }

    // initializing debug extension
    // only bother if we have a debug context
    GLint flag = 0;
    ::glGetIntegerv(GL_CONTEXT_FLAGS, &flag);
    if (flag & GL_CONTEXT_FLAG_DEBUG_BIT) {
        m_DebugContext = true;
        this->InitDebugExtension(m_Ext, m_ConsolePrinter);
    }

    ::glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    m_isActive = true;

    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::GL4Renderer::Destroy() {
    if (this->isActive()) {
        m_isActive = false;
        m_DebugContext = false;
    }
    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::GL4Renderer::RenderScene(const SceneData *scenedata, int32_t extrapolation) {
    OST_UNUSED_PARAMETER(extrapolation);
    if (this->isActive()) {
        if (scenedata == nullptr) {
            m_ConsolePrinter.DebugMessage(u8"Warning: SceneData pointer is null in OpenGL 4 renderer");
            throw ostrich::Exception(u8"SceneData pointer is null");
        }
        ::glClearColor(scenedata->getClearColorRed(), scenedata->getClearColorGreen(),
            scenedata->getClearColorBlue(), scenedata->getClearColorAlpha());

        ::glViewport(0, 0, ostrich::g_ScreenWidth, ostrich::g_ScreenHeight);
        ::glClear(GL_COLOR_BUFFER_BIT);
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::GL4Renderer::CheckCaps() {
    const char *glstring = nullptr;

    glstring = (const char *)::glGetString(GL_RENDERER);
    if (glstring == nullptr) {
        return OST_ERROR_GL4GETSTRING;
    }
    m_ConsolePrinter.WriteMessage(u8"OpenGL Renderer: %", { glstring });

    glstring = (const char *)::glGetString(GL_VENDOR);
    if (glstring == nullptr) {
        return OST_ERROR_GL4GETSTRING;
    }
    m_ConsolePrinter.WriteMessage(u8"OpenGL Vendor: %", { glstring });

    glstring = (const char *)::glGetString(GL_VERSION);
    if (glstring == nullptr) {
        return OST_ERROR_GL4GETSTRING;
    }
    m_ConsolePrinter.WriteMessage(u8"OpenGL Version: %", { glstring });

    glstring = (const char *)::glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (glstring == nullptr) {
        return OST_ERROR_GL4GETSTRING;
    }
    char glshadermajorversion = glstring[0];
    m_ConsolePrinter.WriteMessage(u8"OpenGL Shading Language Version: %", { glstring });

    // check GL versions
    GLint major = 0;
    ::glGetIntegerv(GL_MAJOR_VERSION, &major);
    if (major < this->MAJOR_VERSION_MINIMUM) {
        return OST_ERROR_GL4VERSION;
    }
    if (glshadermajorversion < GL_SHADING_LANGUAGE_VERSION_MINIMUM) {
        return OST_ERROR_GLSHADERVERSION;
    }

    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::GL4Renderer::InitDebugExtension(ostrich::GL4Extensions &ext, ostrich::ConsolePrinter consoleprinter) {
    ext.glDebugMessageCallback(&ostrich::GL4Renderer::DebugMessageCallback, nullptr);
    if (consoleprinter.isValid()) {
        ms_DebugPrinter = consoleprinter;
        ms_DebugPrinter.WriteMessage(u8"OpenGL: Installed debug callback");
    }
    else {
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);
    }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::GL4Renderer::DebugMessageCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    OST_UNUSED_PARAMETER(source);
    OST_UNUSED_PARAMETER(type);
    OST_UNUSED_PARAMETER(id);
    OST_UNUSED_PARAMETER(severity);
    OST_UNUSED_PARAMETER(length);
    OST_UNUSED_PARAMETER(message);
    OST_UNUSED_PARAMETER(userParam);

    std::string msg;
}