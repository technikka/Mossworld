#include <iostream>
using namespace std;

#include "Tile.h"

Tile::Tile(int id, int x, int y) {
    this->id = id;
    this->x = x;
    this->y = y;
};

Tile::Tile() {
    id = -1;
    x = -1;
    y = -1;
}

int Tile::GetId() { return id; }

Entity* Tile::GetOccupant() const { return occupant; }

void Tile::SetOccupant(Entity* occupant) { this->occupant = occupant; };

bool Tile::IsEmpty() const { return occupant == nullptr; };

bool Tile::HasType(EntityType type) const {
    return occupant != nullptr && occupant->GetType() == type;
}

bool Tile::HasCreature() const { return HasType(CREATURE); }

bool Tile::HasNutrientCluster() const { return HasType(NUTRIENT_CLUSTER); }

char Tile::GetSymbol() const {
    if (occupant == nullptr) {
        return '.';
    }

    return occupant->GetSymbol();
}

Position Tile::GetPosition() {
    Position position;
    position.x = x;
    position.y = y;
    return position;
}

Position Tile::IdToCoordinates(int tile_id, int world_width, int world_height) {
    int x = tile_id % world_width;
    int y = tile_id / world_width;
    Position position;
    position.x = x;
    position.y = y;
    return position;
}
