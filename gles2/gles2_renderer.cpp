/*
==========================================
Copyright (c) 2020-2021 Ostrich Labs
==========================================
*/

#include "gles2_renderer.h"
#include <string_view>
#include "../common/error.h"
#include "../game/errorcodes.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::EGLRenderer::EGLRenderer() noexcept : m_isActive(false) {

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::EGLRenderer::~EGLRenderer() {
    this->Destroy();
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::EGLRenderer::Initialize(ostrich::ConsolePrinter conprinter) {
    if (this->isActive())
        return OST_ERROR_ISACTIVE;

    m_ConsolePrinter = conprinter;
    if (!m_ConsolePrinter.isValid())
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    int result = this->CheckCaps();
    if (result != OST_ERROR_OK)
        return result;

    ::glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    m_isActive = true;

    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::EGLRenderer::Destroy() {
    if (this->isActive()) {
    	m_isActive = false;
    }
    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::EGLRenderer::RenderScene(const SceneData *scenedata, int32_t extrapolation) {
    if (!this->isActive())
        return;

    if (scenedata == nullptr) {
        m_ConsolePrinter.DebugMessage(u8"Warning: SceneData pointer is null in OpenGL ES 2 renderer");
        throw ostrich::Exception(u8"SceneData pointer is null");
    }
    ::glClearColor(scenedata->getClearColorRed(), scenedata->getClearColorGreen(),
        scenedata->getClearColorBlue(), scenedata->getClearColorAlpha());

    ::glViewport(0, 0, ostrich::g_ScreenWidth, ostrich::g_ScreenHeight);
    ::glClear(GL_COLOR_BUFFER_BIT);
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::EGLRenderer::CheckCaps() {
    const char *glstring = nullptr;

    glstring = (const char *)::glGetString(GL_RENDERER);
    if (glstring == nullptr) {
        return OST_ERROR_ES2GETSTRING;
    }
    m_ConsolePrinter.WriteMessage(u8"OpenGL Renderer: %", { glstring });

    glstring = (const char *)::glGetString(GL_VENDOR);
    if (glstring == nullptr) {
        return OST_ERROR_ES2GETSTRING;
    }
    m_ConsolePrinter.WriteMessage(u8"OpenGL Vendor: %", { glstring });

    glstring = (const char *)::glGetString(GL_VERSION);
    if (glstring == nullptr) {
        return OST_ERROR_ES2GETSTRING;
    }
    m_ConsolePrinter.WriteMessage(u8"OpenGL Version: %", { glstring });
    std::string_view es2version = glstring;

    glstring = (const char *)::glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (glstring == nullptr) {
        return OST_ERROR_ES2GETSTRING;
    }
    m_ConsolePrinter.WriteMessage(u8"OpenGL Shading Language Version: %", { glstring });
    std::string_view shadingversion = glstring;

    glstring = (const char *)::glGetString(GL_EXTENSIONS);
    if (glstring == nullptr) {
        m_ConsolePrinter.WriteMessage(u8"No OpenGL extensions supported");
    }
    else {
        m_ConsolePrinter.WriteMessage(u8"Supported Extensions: %", { glstring });
    }

    // check GL versions: ES 2 and shading language 1
    // Note: this is also done by ::eglInitialize() from DisplayRaspi
    if (es2version.find("OpenGL ES 2") == std::string_view::npos) {
        return OST_ERROR_ES2VERSION;
    }
    if (shadingversion.find("OpenGL ES GLSL ES 1") == std::string_view::npos) {
        return OST_ERROR_ES2SHADERVERSION;
    }

    return OST_ERROR_OK;
}
