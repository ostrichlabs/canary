/*
==========================================
Copyright (c) 2020 Ostrich Labs

Common interface for a renderer class
==========================================
*/

#ifndef OSTRICH_I_RENDERER_H_
#define OSTRICH_I_RENDERER_H_

#include "scenedata.h"
#include "../common/console.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class IRenderer {
public:

    /////////////////////////////////////////////////
    // Empty constructor and destructor; interface has no data.
    // Copy/move constructors/operators are implicitly defaulted. ... that's probably fine for this yeah? (TODO: add them anyway)
    IRenderer() noexcept {}
    virtual ~IRenderer() {}

    /////////////////////////////////////////////////
    // Initialize the renderer.
    // Actual implementation depends on the renderer, but every renderer I know of requires some data from the window system.
    // If that information needs to be passed, it can be done here, but I'll cross that bridge when I get to it.
    //
    // in:
    //      consoleprinter - an initialized ConsolePrinter for logging
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    virtual int Initialize(ConsolePrinter consoleprinter) = 0;

    /////////////////////////////////////////////////
    // Cleans up any allocated data or created objects as required by the renderer.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    virtual int Destroy() = 0;

    /////////////////////////////////////////////////
    // Check if the object is valid (by checking the m_isActive flag).
    // I suppose I could have made the m_isActive flag a part of the interface, but I didn't (TODO: should I?)
    //
    // returns:
    //      m_isActive flag
    virtual bool isActive() const = 0;

    /////////////////////////////////////////////////
    // Given passed scene data, draw to the screen
    //
    // in:
    //      scenedata - a pointer to the scene data (think right now it's just screen clear color and an entity list)
    //      extrapolation - how far into the next frame we are, in milliseconds (lag/msperframe)
    // returns:
    //      void
    virtual void RenderScene(const SceneData *scenedata, int32_t extrapolation) = 0;

protected:

};

} // namespace ostrich

#endif /* OSTRICH_I_RENDERER_H_ */
