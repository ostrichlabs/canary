/*
==========================================
Copyright (c) 2020 Ostrich Labs

IDisplay implementation for Linux, using X11
==========================================
*/

#ifndef X11_DISPLAY_H_
#define X11_DISPLAY_H_

#include "../game/i_display.h"
#include <GL/glx.h>

namespace ostrich {

/////////////////////////////////////////////////
//
class DisplayX11 : public IDisplay {
public:

    DisplayX11() noexcept;
    virtual ~DisplayX11();
    DisplayX11(DisplayX11 &&) = delete;
    DisplayX11(const DisplayX11 &) = delete;
    DisplayX11 &operator=(DisplayX11 &&) = delete;
    DisplayX11 &operator=(const DisplayX11 &) = delete;

    bool isActive() const override { return m_isActive; }

    int Initialize(ConsolePrinter consoleprinter) override;
    int Destroy() override;

    bool SwapBuffers() override;

private:

    int InitWindow() override;
    int InitRenderer() override;

    bool m_isActive;

    // goofy "error handler" for X while creating a context
    static bool ms_XError;
    static int GLXErrorHandler(Display *display, XErrorEvent *event);

    ConsolePrinter  m_ConsolePrinter;

    GLXFBConfig m_FrameBufferConfig;
    Display *m_Display;
    Colormap m_Colormap;
    Window m_GLWindow;

    GLXContext m_GLContext;
};

} // namespace ostrich


#endif /* X11_DISPLAY_H_ */
