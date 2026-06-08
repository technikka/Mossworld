#include <array>
#include <cstdlib>  // for rand()
#include <ctime>
#include <iostream>
#include <string>
using namespace std;

#include "Creature.h"
#include "World.h"

int main() {
    int turn_number = 1;
    srand(time(0));  // seed rand

    cout << "\nWelcome to Mossworld.\n";
    cout << "Tiny creatures stir among the morning dew.\n\n";

    // World creates tiles and creatures
    World world(3, 2);
    world.print();
    world.printHUD(turn_number);

    return 0;
}