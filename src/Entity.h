#ifndef ENTITY_H
#define ENTITY_H

#include <string>

#include "EntityType.h"
#include "Position.h"

// Forward declaration.
class Tile;

class Entity {
   public:
    Entity(EntityType type, Tile* current_tile);
    Position GetPosition() const;

    Tile* GetCurrentTile() const;
    void SetCurrentTile(Tile* tile);
    EntityType GetType() const;

    char GetSymbol() const;

   protected:
    Tile* current_tile = nullptr;
    EntityType type;
    char symbol;
};

#endif