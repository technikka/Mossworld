#include <array>
#include <cstdlib>  // for rand()
#include <iostream>
#include <string>
using namespace std;

const char UNOCCUPIED_SPACE = '.';
const char CREATURE = 'k';
const char FOOD = '~';

const int WORLD_HEIGHT = 6;
const int WORLD_WIDTH = 9;

struct Tile {
  int x;
  int y;
  char occupant;
};

using World = array<array<Tile, WORLD_WIDTH>, WORLD_HEIGHT>;

void printWorld(World& world) {
  for (int row = 0; row < world.size(); row++) {
    for (int column = 0; column < world[0].size(); column++) {
      cout << ' ' << world[row][column].occupant << ' ';
    }
    cout << endl;
  }
  cout << endl;
}

World createWorld(int creature_count, int food_count) {
  int creatures_placed = 0;
  int food_placed = 0;
  World world;  // instantiating world array

  for (int row = 0; row < WORLD_HEIGHT; row++) {
    for (int column = 0; column < WORLD_WIDTH; column++) {
      Tile& tile = world[row][column];
      tile.x = column;
      tile.y = row;

      bool max_creatures = creatures_placed == creature_count;
      bool max_food = food_placed == food_count;

      int random_number = rand() % 10;

      if (random_number == 6 && !max_creatures) {
        tile.occupant = CREATURE;
        creatures_placed += 1;
      } else if (random_number == 9 && !max_food) {
        tile.occupant = FOOD;
        food_placed += 1;
      } else {
        tile.occupant = UNOCCUPIED_SPACE;
      }
    }
  }
  return world;
}

int main() {
  int turn_number = 1;
  int creature_count = 3;
  int food_count = 2;
  srand(time(0));  // seed rand

  cout << "\nWelcome to Mossworld.\n";
  cout << "Tiny creatures stir beneath the dawn mist.\n\n";

  World world = createWorld(creature_count, food_count);

  printWorld(world);

  cout << "\n\n";  // space beneath world

  cout << "Turn: " << turn_number << "  |  "
       << "Creatures: " << creature_count << "  |  "
       << "Food: " << food_count << "\n\n";

  return 0;
}