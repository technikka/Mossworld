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

// is also creating creatures for now
void createTiles(int creature_count, int food_count) {
  int creatures_placed = 0;
  int food_placed = 0;

  for (int row = 0; row < WORLD_HEIGHT; row++) {
    for (int column = 0; column < WORLD_WIDTH; column++) {
      Tile tile;
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
      cout << ' ' << tile.occupant << ' ';
    }
    cout << endl;
  }
}

int main() {
  int turn_number = 1;
  int creature_count = 3;
  int food_count = 2;
  srand(time(0));  // seed rand

  cout << "\nWelcome to Mossworld.\n";
  cout << "Tiny creatures stir beneath the dawn mist.\n\n";

  createTiles(creature_count, food_count);

  cout << "\n\n";  // space beneath world

  cout << "Turn: " << turn_number << "  |  "
       << "Creatures: " << creature_count << "  |  "
       << "Food: " << food_count << "\n\n";

  return 0;
}