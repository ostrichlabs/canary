/*
==========================================
Copyright (c) 2020 Ostrich Labs

IDisplay implementation for Linux, using X11
==========================================
*/

#ifndef OSTRICH_X11_DISPLAY_H_
#define OSTRICH_X11_DISPLAY_H_

#include <GL/glx.h>
#include "../game/i_display.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class DisplayX11 : public IDisplay {
public:

    /////////////////////////////////////////////////
    // Constructor is defined but simple.
    // Destructor is defined but does nothing so I don't know if I should bother.
    // Copy/move constructors/operators are deleted to prevent accidentally creating two windows
    // (if you want to make another, you can do it manually)
    DisplayX11() noexcept;
    virtual ~DisplayX11();
    DisplayX11(DisplayX11 &&) = delete;
    DisplayX11(const DisplayX11 &) = delete;
    DisplayX11 &operator=(DisplayX11 &&) = delete;
    DisplayX11 &operator=(const DisplayX11 &) = delete;

    /////////////////////////////////////////////////
    // Initialize the display.
    //
    // in:
    //      consoleprinter - an initialized ConsolePrinter for logging
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int Initialize(ConsolePrinter consoleprinter) override;

    /////////////////////////////////////////////////
    // Cleans up window contexts and handles.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int Destroy() override;

    /////////////////////////////////////////////////
    // Check if the object is valid (by checking the m_isActive flag).
    // I suppose I could have made the m_isActive flag a part of the interface, but I didn't (TODO: should I?)
    //
    // returns:
    //      m_isActive flag
    bool isActive() const override { return m_isActive; }

    /////////////////////////////////////////////////
    // Calls glXSwapBuffers()
    //
    // returns:
    //      true/false if operation was successful
    bool SwapBuffers() override;

private:

    /////////////////////////////////////////////////
    // Helper method to initialize an X11 display.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int InitWindow() override;

    /////////////////////////////////////////////////
    // Helper method to initialize the renderer.
    // Involves creating a dummy OpenGL context to retrieve WGL extensions and *then* create a real context.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int InitRenderer() override;

    // goofy "error handler" for X while creating a context
    static int GLXErrorHandler(Display *display, XErrorEvent *event);

    bool m_isActive;

    static bool ms_XError;

    ConsolePrinter  m_ConsolePrinter;

    GLXFBConfig m_FrameBufferConfig;
    Display *m_Display;
    Colormap m_Colormap;
    Window m_GLWindow;

    GLXContext m_GLContext;
};

} // namespace ostrich


#endif /* OSTRICH_X11_DISPLAY_H_ */
