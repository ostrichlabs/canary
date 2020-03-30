/*
==========================================
Copyright (c) 2020 Ostrich Labs

Common interface for a renderer class
==========================================
*/

#ifndef I_RENDERER_H_
#define I_RENDERER_H_

#include "../common/console.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class IRenderer {
public:

    IRenderer() noexcept {}
    virtual ~IRenderer() {}

    virtual bool isActive() const = 0;

    virtual int Initialize(ConsolePrinter) = 0;
    virtual int Destroy() = 0;

    virtual void RenderScene() = 0;

private:
};

} // namespace ostrich

#endif /* I_RENDERER_H_ */
