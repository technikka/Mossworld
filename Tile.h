using namespace std;
#include "EntityType.h"
#include "Position.h"
#ifndef TILE_H
#define TILE_H

class Tile {
   public:
    Tile();
    Tile(int id, int x, int y, EntityType occupant);
    int GetId();
    EntityType occupant;
    Position GetPosition();
    static Position IdToCoordinates(int tile_id, int world_width,
                                    int world_height);
    int CoordinatesToId(Position coordinates, int world_width);

   private:
    int id;
    int x;
    int y;
};

#endif