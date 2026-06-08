using namespace std;
#ifndef TILE_H
#define TILE_H

struct Position {
    int x;
    int y;
};

enum OccupantType { EMPTY, CREATURE, NUTRIENT };
char OccupantTypeToChar(OccupantType type);

class Tile {
   public:
    Tile();
    Tile(int id, int x, int y, OccupantType occupant);
    int GetId();
    OccupantType occupant;
    OccupantType GetOccupant();
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