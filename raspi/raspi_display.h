/*
==========================================
Copyright (c) 2020 Ostrich Labs

IDisplay implementation for the Raspberry Pi
==========================================
*/

#ifndef DISPLAY_RASPI_H_
#define DISPLAY_RASPI_H_

#include "../common/ost_common.h"

#if (OST_RASPI != 1)
#    error "This module should only be included in Raspberry Pi builds"
#endif

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "bcm_host.h"
#include "../game/i_display.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class DisplayRaspi : public IDisplay {
public:

    DisplayRaspi();
    virtual ~DisplayRaspi();
    DisplayRaspi(DisplayRaspi &&) = delete;
    DisplayRaspi(const DisplayRaspi &) = delete;
    DisplayRaspi &operator=(DisplayRaspi &&) = delete;
    DisplayRaspi &operator=(const DisplayRaspi &) = delete;

    bool isActive() const override { return m_isActive; }

    int Initialize(ConsolePrinter conprinter) override;
    int Destroy() override;

    bool SwapBuffers() override;

private:

    int InitWindow() override;
    int InitRenderer() override;

    bool m_isActive;

    ConsolePrinter  m_ConsolePrinter;

// OpenGL ES 2.0 for Raspi, now and FOREVER
    EGLConfig   m_GLConfig;
    EGLContext  m_GLContext;
    EGLDisplay  m_GLDisplay;
    EGLSurface  m_GLSurface;

    EGL_DISPMANX_WINDOW_T       m_NativeWindow;
    DISPMANX_ELEMENT_HANDLE_T   m_DispmanElement;
    DISPMANX_DISPLAY_HANDLE_T   m_DispmanDisplay;
    DISPMANX_UPDATE_HANDLE_T    m_DispmanUpdate;
};

} // namespace ostrich

#endif /* DISPLAY_RASPI_H_ */
