#include <array>
#include <cstdlib>  // for rand()
#include <ctime>
#include <iostream>
#include <string>
using namespace std;

#include "Creature.h"
#include "World.h"

int main() {
    srand(time(0));  // seed rand

    cout << "\nWelcome to Mossworld.\n";
    cout << "Tiny creatures stir among the morning dew.\n\n";

    World world;  // instantiate world

    bool sim_continue = false;
    do {
        world.Print();
        world.PrintHUD();

        cout << "\nA new day is unfolding. Observe? (y/n)" << endl;

        char input;
        cin >> input;

        if (input == 'y') {
            sim_continue = true;
            world.advanceDay();
        } else {
            sim_continue = false;
        }
    } while (sim_continue == true);

    return 0;
}