/*
==========================================
Copyright (c) 2020 Ostrich Labs

Scene data

A scene is whatever is on the screen: a clear color, a list of entities, and some kind of map
(whether a 2D tile map or a 3D environment - or an amalgamation of both)

For the crappier projects like Minesweeper, the environment will likely be just a 2D map.

Probably need some standard data structure for text as well.

The data should be in a Canary-standard format and translated by the renderer.
I am hoping any optimization can be done in the collection of scene data so there's less renderer-specific code.
==========================================
*/

#ifndef SCENEDATA_H_
#define SCENEDATA_H_

#include <list>
#include "i_entity.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class SceneData {
public:

    SceneData() noexcept :
        m_ClearColorRed(0.0f), m_ClearColorGreen(0.0f), m_ClearColorBlue(0.0f), m_ClearColorAlpha(1.0f) {}
    virtual ~SceneData() {}
    SceneData(SceneData &&) = delete;
    SceneData(const SceneData &) = delete;
    SceneData &operator=(SceneData &&) = delete;
    SceneData &operator=(const SceneData &) = delete;

    void setClearColor(float red, float green, float blue, float alpha) noexcept
    {
        m_ClearColorRed = red; m_ClearColorGreen = green; m_ClearColorBlue = blue; m_ClearColorAlpha = alpha;
    }

    float getClearColorRed() const noexcept { return m_ClearColorRed; }
    float getClearColorGreen() const noexcept { return m_ClearColorGreen; }
    float getClearColorBlue() const noexcept { return m_ClearColorBlue; }
    float getClearColorAlpha() const noexcept { return m_ClearColorAlpha; }
    const std::list<IEntity> *GetEntityList() { return &m_EntityList; }

private:

    float m_ClearColorRed;
    float m_ClearColorGreen;
    float m_ClearColorBlue;
    float m_ClearColorAlpha;

    std::list<IEntity> m_EntityList;
};

} // namespace ostrich

#endif /* SCENEDATA_H_ */