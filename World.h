#include <vector>

#include "Creature.h"
#include "Tile.h"
using namespace std;

#ifndef WORLD_H
#define WORLD_H

class World {
   public:
    World(int creature_count, int nutrient_count, int width = 9,
          int height = 6);
    int GetWidth();
    int GetHeight();
    int GetCreatureCount();
    int GetNutrientCount();
    void print();
    void printHUD(int day);
    void advanceDay();

    // * Grid indexing:
    // * tiles[row][column]
    // * tiles[0] = first row
    // * tiles[0][1] = first row, second column
    vector<vector<Tile>> tiles;

    vector<Creature> creatures;

   private:
    int height;
    int width;
    int creature_count;
    int nutrient_count;
    int next_creature_id = 1;

    void createTiles();
    void placeCreatures();
    void createCreature(Position position);
    vector<Position> getAdjacentOpenPositions(Position position);
    Position selectPosition(Creature& creature);
    void MoveCreature(Creature& creature);
};

#endif