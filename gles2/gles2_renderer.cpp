/*
==========================================
Copyright (c) 2020 Ostrich Labs
==========================================
*/

#include "gles2_renderer.h"
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

    /*
    TODO: Hardcode resolution - use constants
    */

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
        m_ConsolePrinter.DebugMessage(u8"Warning: SceneData pointer is null in OpenGL 4 renderer");
        throw ostrich::Exception(u8"SceneData pointer is null");
    }
    ::glClearColor(scenedata->getClearColorRed(), scenedata->getClearColorGreen(),
        scenedata->getClearColorBlue(), scenedata->getClearColorAlpha());

    ::glViewport(0, 0, ostrich::g_ScreenWidth, ostrich::g_ScreenHeight);
    ::glClear(GL_COLOR_BUFFER_BIT);
}
