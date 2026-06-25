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

    string input;

    while (true) {
        world.Print();
        world.PrintHUD();

        cout << "\nA new day is unfolding. Observe? (Press Enter or 'exit' to "
                "quit)";

        getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        world.advanceDay();
    }

    return 0;
}