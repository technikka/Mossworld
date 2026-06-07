#include <vector>
using namespace std;

#ifndef WORLD_H
#define WORLD_H

struct Position {
  int x;
  int y;
};

struct Tile {
  int id;
  int x;
  int y;
  //   OccupantType occupant;
};

class World {
 public:
  World(int creature_count, int nutrient_count, int width = 9, int height = 6);
  int GetWidth();
  int GetHeight();
  int GetCreatureCount();
  int GetNutrientCount();
  void createTiles();
  void print();

  vector<vector<Tile>> tiles;

 private:
  int height;
  int width;
  int creature_count;
  int nutrient_count;
};

#endif