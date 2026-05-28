#include <cstdlib>  // for rand()
#include <iostream>
#include <string>
using namespace std;

int main() {
  char unoccupied_space = '.';
  char creature = 'k';
  char food = '~';

  int world_height = 6;
  int world_width = 9;

  int turn_number = 1;
  int creature_count = 3;
  int food_count = 2;

  struct Tile {
    int x;
    int y;
    char occupant;
  };

  // Create some randomness
  srand(time(0));
  int random_number;

  // Get world size from user
  // cout << "Select World Size\n";
  // cout << "Height: (enter number 4 - 20)\n";
  // cin >> world_height;
  // cout << "Width: (enter number 4 - 20)\n";
  // cin >> world_width;

  // Display world info
  cout << "\nWelcome to Mossworld.\n";
  cout << "Tiny creatures stir beneath the dawn mist.\n\n";

  // Create tiles & Print tiles
  int creatures_placed = 0;
  int food_placed = 0;

  for (int row = 0; row < world_height; row++) {
    for (int column = 0; column < world_width; column++) {
      random_number = rand() % 10;

      Tile tile;
      tile.x = column;
      tile.y = row;

      bool max_creatures = creatures_placed == creature_count;
      bool max_food = food_placed == food_count;

      if (random_number == 6 && !max_creatures) {
        tile.occupant = creature;
        creatures_placed += 1;
      } else if (random_number == 9 && !max_food) {
        tile.occupant = food;
        food_placed += 1;
      } else {
        tile.occupant = unoccupied_space;
      }
      cout << ' ' << tile.occupant << ' ';
    }
    cout << endl;
  }
  cout << "\n\n";

  // Display world status
  cout << "Turn: " << turn_number << "  |  "
       << "Creatures: " << creatures_placed << "  |  "
       << "Food: " << food_placed << "\n\n";

  return 0;
}