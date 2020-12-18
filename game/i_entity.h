/*
==========================================
Copyright (c) 2020 Ostrich Labs

Interface for entities

An "entity" is a game object with state: model, texture(s), position, movement, and maybe in the future triggers.
The common interface should tell other subsystems the information they need to know, without necessarily
revealing the entity's precise state.

So for a 2D object (a tile, or a button, or whatever) in a 2D game, you should know its current coordinates and texture.

Early versions of canary will be nothing but 2D objects, so that will suffice for now.
==========================================
*/

#ifndef OSTRICH_I_ENTITY_H_
#define OSTRICH_I_ENTITY_H_

#include "../common/ost_common.h"

namespace ostrich {

/////////////////////////////////////////////////
//
class IEntity {
public:

    IEntity() noexcept = default;
    virtual ~IEntity() {}
    IEntity(IEntity &&) = delete;
    IEntity(const IEntity &) = delete;
    IEntity &operator=(IEntity &&) = delete;
    IEntity &operator=(const IEntity &) = delete;

    std::string_view getTextureName();

    int32_t getXPos();
    int32_t getYPos();

    int32_t getXLength();
    int32_t getYLength();

private:

};

} // namespace ostrich

#endif /* OSTRICH_I_ENTITY_H_ */