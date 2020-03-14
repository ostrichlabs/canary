/*
==========================================
renderer_egl.cpp

Copyright (c) 2019 Ostrich Labs

OpenGL ES 2.0 interface - primarily for the Raspberry Pi
==========================================
*/

#include "renderer_egl.h"
#include "../system/sys_error.h"

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
int ostrich::EGLRenderer::Destroy() {
    if (this->isActive()) {
    	m_isActive = false;
    }
    return 0;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
void ostrich::EGLRenderer::RenderScene() {
    if (!this->isActive())
        return;

    ::glViewport(0, 0, ostrich::g_ScreenWidth, ostrich::g_ScreenHeight);
    ::glClear(GL_COLOR_BUFFER_BIT);
}
