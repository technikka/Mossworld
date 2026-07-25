#include "Entity.h"

#include <string>

#include "Tile.h"

using namespace std;

Entity::Entity(EntityType type, Tile* current_tile) {
    this->type = type;
    this->current_tile = current_tile;
}

Tile* Entity::GetCurrentTile() const { return current_tile; }

void Entity::SetCurrentTile(Tile* tile) { current_tile = tile; }

EntityType Entity::GetType() const { return type; }

Position Entity::GetPosition() const { return GetCurrentTile()->GetPosition(); }

string Entity::GetSymbol() const { return symbol; }