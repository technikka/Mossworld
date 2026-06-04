#include <array>
#include <cstdlib>  // for rand()
#include <ctime>
#include <iostream>
#include <string>
using namespace std;

#include "creatures.h"

const int WORLD_HEIGHT = 6;
const int WORLD_WIDTH = 9;

enum OccupantType { EMPTY, CREATURE, NUTRIENT };

struct Tile {
  int id;
  int x;
  int y;
  OccupantType occupant;
};

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

struct Position {
  int x;
  int y;
};

using World = array<array<Tile, WORLD_WIDTH>, WORLD_HEIGHT>;

void printWorld(World& world) {
  for (int row = 0; row < WORLD_HEIGHT; row++) {
    for (int column = 0; column < WORLD_WIDTH; column++) {
      char symbol = OccupantTypeToChar(world[row][column].occupant);
      cout << ' ' << symbol << ' ';
    }
    cout << endl;
  }
  cout << endl;
}

Position idToCoordinates(int tile_id) {
  int x = tile_id % WORLD_WIDTH;
  int y = tile_id / WORLD_WIDTH;
  Position position;
  position.x = x;
  position.y = y;
  return position;
}

int coordinatesToId(Position coordinates) {
  return coordinates.y * WORLD_WIDTH + coordinates.x;
}

World createWorld(int creature_count, int nutrient_count) {
  // * world[0] is the first row, world[0][1] is first row, second column.
  World world;  // instantiating world array

  // create tiles
  for (int row = 0; row < WORLD_HEIGHT; row++) {
    for (int column = 0; column < WORLD_WIDTH; column++) {
      Tile& tile = world[row][column];
      tile.id = row * WORLD_WIDTH + column;
      tile.x = column;
      tile.y = row;
      tile.occupant = EMPTY;
    }
  }

  // create spawn zones and randomly place a creature within each zone.
  int number_of_tiles = (WORLD_HEIGHT * WORLD_WIDTH);

  unsigned int counter = 0;
  for (int i = 0; i < creature_count; i++) {
    int start_id = counter;
    // divide the tile space proportionally:
    int end_id = ((i + 1.0) / creature_count) * number_of_tiles;

    bool creature_placed = false;

    // * WARNING: this loop assumes at least one empty tile exists in the zone.
    // * Infinite-loop potential.
    while (!creature_placed) {
      int rand_pos = rand() % (end_id - start_id);

      // find the tile to update
      int tile_id = rand_pos + start_id;
      Position pos = idToCoordinates(tile_id);  // p
      Tile& tile = world[pos.y][pos.x];

      // update tile
      if (tile.occupant == EMPTY) {
        tile.occupant = CREATURE;
        creature_placed = true;
      }
    }

    counter = end_id;
  }

  return world;
}

int main() {
  int turn_number = 1;
  int creature_count = 3;
  int nutrient_count = 2;
  srand(time(0));  // seed rand

  cout << "\nWelcome to Mossworld.\n";
  cout << "Tiny creatures stir beneath the dawn mist.\n\n";

  World world = createWorld(creature_count, nutrient_count);

  printWorld(world);

  cout << "\n\n";  // space beneath world

  cout << "Turn: " << turn_number << "  |  "
       << "Mosslings: " << creature_count << "  |  "
       << "Nutrient Clusters: " << nutrient_count << "\n\n";

  return 0;
}