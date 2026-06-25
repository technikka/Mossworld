#ifndef ENTITY_H
#define ENTITY_H

#include <string>

#include "EntityType.h"

// Forward declaration.
class Tile;

class Entity {
   public:
    Entity(EntityType type, Tile* current_tile);

    Tile* GetCurrentTile() const;
    void SetCurrentTile(Tile* tile);
    EntityType GetType() const;

   protected:
    Tile* current_tile = nullptr;
    EntityType type;
};

#endif