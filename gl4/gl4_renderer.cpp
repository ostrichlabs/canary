/*
==========================================
Copyright (c) 2020 Ostrich Labs

OpenGL 4.0 interface
==========================================
*/

#include "gl4_renderer.h"
#include "../common/error.h"
#include "../game/errorcodes.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::GL4Renderer::GL4Renderer() noexcept : m_isActive(false) {

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

    /*
    TODO: Hardcode resolution - use constants
    */

    ::glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    m_isActive = true;

    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::GL4Renderer::Destroy() {
    if (this->isActive()) {
        m_isActive = false;
    }
    return OST_ERROR_OK;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::GL4Renderer::RenderScene(const SceneData *scenedata, int32_t extrapolation) {
    UNUSED_PARAMETER(extrapolation);
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