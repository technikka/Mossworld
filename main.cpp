#include <array>
#include <cstdlib>  // for rand()
#include <ctime>
#include <iostream>
#include <string>
using namespace std;

#include "Creature.h"
#include "World.h"

int main() {
    int day = 1;
    srand(time(0));  // seed rand

    cout << "\nWelcome to Mossworld.\n";
    cout << "Tiny creatures stir among the morning dew.\n\n";

    World world(3, 2);  // instantiate world

    bool sim_continue = false;
    do {
        world.print();
        world.printHUD(day);

        cout << "\nA new day is unfolding. Observe? (y/n)\n" << endl;

        char input;
        cin >> input;

        if (input == 'y') {
            sim_continue = true;
            day += 1;
            world.advanceDay();
        } else {
            sim_continue = false;
        }
    } while (sim_continue == true);

    return 0;
}