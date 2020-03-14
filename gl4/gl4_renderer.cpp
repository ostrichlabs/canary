/*
==========================================
gl4_renderer.h

Copyright (c) 2020 Ostrich Labs

OpenGL 4.0 interface
==========================================
*/

#include "gl4_renderer.h"
#include "../common/error.h"

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::GL4Renderer::GL4Renderer() noexcept : m_isActive(false) {

}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
ostrich::GL4Renderer::~GL4Renderer() {
    this->Destroy();
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::GL4Renderer::Initialize(ostrich::ConsolePrinter conprinter) {
    if (this->isActive())
        return -1;

    m_ConPrinter = conprinter;
    if (!m_ConPrinter.isValid())
        throw ostrich::ProxyException(OST_FUNCTION_SIGNATURE);

    /*
    TODO: Hardcode resolution - use constants
    */

    ::glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    m_isActive = true;

    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
int ostrich::GL4Renderer::Destroy() {
    if (this->isActive()) {
        m_isActive = false;
    }
    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::GL4Renderer::RenderScene() {
    if (this->isActive()) {
        ::glViewport(0, 0, ostrich::g_ScreenWidth, ostrich::g_ScreenHeight);
        ::glClear(GL_COLOR_BUFFER_BIT);
    }
}