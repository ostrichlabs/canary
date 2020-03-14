/*
==========================================
gl4_renderer.h

Copyright (c) 2020 Ostrich Labs

OpenGL 4.0 interface
==========================================
*/

#ifndef GL4_RENDERER_H_
#define GL4_RENDERER_H_

#include "../common/ost_common.h"

#if (OST_WINDOWS != 1)
#    error "This module should only be included in Windows builds"
#endif

#include <windows.h>
#include <gl/GL.h>
#include "../game/i_renderer.h"
#include "../external/gl/glcorearb.h"
#include "../external/gl/glext.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class GL4Renderer : public IRenderer {
public:

    GL4Renderer() noexcept;
    virtual ~GL4Renderer();
    GL4Renderer(GL4Renderer &&) = delete;
    GL4Renderer(const GL4Renderer &) = delete;
    GL4Renderer &operator=(GL4Renderer &&) = delete;
    GL4Renderer &operator=(const GL4Renderer &) = delete;

    bool isActive() const override { return m_isActive; }

    int Initialize(ConsolePrinter conprinter) override;
    int Destroy() override;

    void RenderScene() override;

private:

    bool m_isActive;
    ConsolePrinter m_ConPrinter;
};

} // namespace ostrich

#endif /* GL4_RENDERER_H_ */
