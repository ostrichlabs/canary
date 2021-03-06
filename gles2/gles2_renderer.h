/*
==========================================
Copyright (c) 2020 Ostrich Labs

OpenGL ES 2.0 interface - primarily for the Raspberry Pi
==========================================
*/

#ifndef RENDERER_EGL_H_
#define RENDERER_EGL_H_

#include "../common/ost_common.h"

#if (OST_RASPI != 1)
#    error "This module should only be included in Raspberry Pi builds"
#endif

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "../game/i_renderer.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class EGLRenderer : public IRenderer {
public:

    EGLRenderer() noexcept;
    virtual ~EGLRenderer();
    EGLRenderer(EGLRenderer &&) = delete;
    EGLRenderer(const EGLRenderer &) = delete;
    EGLRenderer &operator=(EGLRenderer &&) = delete;
    EGLRenderer &operator=(const EGLRenderer &) = delete;

    bool isActive() const override { return m_isActive; }

    int Initialize(ConsolePrinter conprinter) override;
    int Destroy() override;

    void RenderScene(const SceneData *scenedata, int32_t extrapolation) override;

private:

    int CheckCaps();

    bool m_isActive;
    ConsolePrinter m_ConsolePrinter;
};

} // namespace ostrich

#endif /* RENDERER_EGL_H_ */
