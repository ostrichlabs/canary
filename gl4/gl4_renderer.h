/*
==========================================
Copyright (c) 2020-2021 Ostrich Labs

OpenGL 4.0 interface
==========================================
*/

#ifndef OSTRICH_GL4_RENDERER_H_
#define OSTRICH_GL4_RENDERER_H_

#include "../common/ost_common.h"

#if (OST_WINDOWS == 1)
#   include <windows.h> // required for GL.h
#endif

#include <GL/gl.h>
#include "GL/glcorearb.h"   // taken from https://github.com/KhronosGroup/OpenGL-Registry
#include "GL/glext.h"       // taken from https://github.com/KhronosGroup/OpenGL-Registry
#include "gl4_extensions.h"
#include "gl4_texture.h"
#include "../game/i_renderer.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class GL4Renderer : public IRenderer {
public:

    /////////////////////////////////////////////////
    // Constructor is defined in cpp but I'm not sure if it needs to be.
    // Destructor is defined, but no memory is allocated so it may not be necessary.
    // Copy/move constructors/operators are deleted to prevent accidentally creating two GL renderers
    // (if you want to make another, you can do it manually)
    GL4Renderer() noexcept;
    virtual ~GL4Renderer();
    GL4Renderer(GL4Renderer &&) = delete;
    GL4Renderer(const GL4Renderer &) = delete;
    GL4Renderer &operator=(GL4Renderer &&) = delete;
    GL4Renderer &operator=(const GL4Renderer &) = delete;

    /////////////////////////////////////////////////
    // Initialize the renderer.
    // Sets the m_isActive flag to true when done.
    //
    // in:
    //      consoleprinter - an initialized ConsolePrinter for logging
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int Initialize(ConsolePrinter conprinter) override;

    /////////////////////////////////////////////////
    // Cleans up any allocated data or created objects as required by the renderer.
    // Flips the m_isActive flag to false when done.
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int Destroy() override;

    /////////////////////////////////////////////////
    // Check if the object is valid (by checking the m_isActive flag).
    // The flag should be false by default & after Destroy() and true after Initialize()
    //
    // returns:
    //      m_isActive flag
    bool isActive() const noexcept override { return m_isActive; }

    /////////////////////////////////////////////////
    // Given passed scene data, draw to the screen
    //
    // in:
    //      scenedata - a pointer to the scene data (think right now it's just screen clear color and an entity list)
    //      extrapolation - how far into the next frame we are, in milliseconds (lag/msperframe)
    // returns:
    //      void
    void RenderScene(const SceneData *scenedata, int32_t extrapolation) override;

private:

    /////////////////////////////////////////////////
    // Check the OpenGL and GLSL versions
    //
    // returns:
    //      An error code (OST_ERROR_OK (0) is the only successful code)
    int CheckCaps();

    const GLint MAJOR_VERSION_MINIMUM = 4;
    const char GL_SHADING_LANGUAGE_VERSION_MINIMUM = '4';

    bool m_isActive;
    bool m_DebugContext;
    ConsolePrinter m_ConsolePrinter;

    GL4Extensions m_Ext;

    /////////////////////////////////////////////////
    // for use with debug extensions
    // static methods/variables are necessary to interface with the extension
    // This kind of wrecks the ability to have two renderer objects but I don't think that's a big concern right now
    /////////////////////////////////////////////////

    /////////////////////////////////////////////////
    // Install the callback function and a console printer
    //
    // in:
    //      ext - Loaded OpenGL extensions
    //      consoleprinter - A copy of a ConsolePrinter object
    // returns:
    //      void
    static void InitDebugExtension(GL4Extensions &ext, ConsolePrinter consoleprinter);

    /////////////////////////////////////////////////
    // Logs the debug message to the console but does nothing else for now
    // Future revisions might send messages to an EventQueue but not yet
    //
    // in:
    //      see KHR_debug documentation
    // returns:
    //      void
    static void DebugMessageCallback(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

    static ConsolePrinter ms_DebugPrinter;
};

} // namespace ostrich

#endif /* OSTRICH_GL4_RENDERER_H_ */
