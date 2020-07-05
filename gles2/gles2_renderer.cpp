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

    m_ConPrinter = conprinter;
    if (!m_ConPrinter.isValid())
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
void ostrich::EGLRenderer::RenderScene() {
    if (!this->isActive())
        return;

    ::glViewport(0, 0, ostrich::g_ScreenWidth, ostrich::g_ScreenHeight);
    ::glClear(GL_COLOR_BUFFER_BIT);
}
