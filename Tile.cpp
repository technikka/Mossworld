#include <iostream>
using namespace std;

#include "Tile.h"

Tile::Tile(int id, int x, int y, OccupantType occupant) {
    this->id = id;
    this->x = x;
    this->y = y;
    this->occupant = occupant;
};

Tile::Tile() {
    id = -1;
    x = -1;
    y = -1;
}

int Tile::GetId() { return id; }

OccupantType Tile::GetOccupant() { return occupant; }

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

int Tile::CoordinatesToId(Position coordinates, int world_width) {
    return coordinates.y * world_width + coordinates.x;
}

char OccupantTypeToChar(OccupantType type) {
    switch (type) {
        case EMPTY:
            return '.';

        case CREATURE:
            return 'k';

        case NUTRIENT:
            return '~';
    }
    return '?';  // should never be reached
}