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

    cout << "\n";
    cout << "──────────────────────────────────────────────────────────────\n";
    cout << "Welcome to Mossworld.\n";
    cout << "Tiny creatures stir among the morning dew.\n\n";

    WorldConfig config;
    World world(config);  // instantiate world

    string input;

    while (true) {
        world.Observe();

        while (true) {
            getline(cin, input);

            if (input.empty()) {
                break;  // advance simulation
            }

            if (input == "exit") {
                return 0;
            }

            if (input == "1") {
                world.SetViewMode(ViewMode::World);
                world.Observe();
            }

            if (input == "2") {
                world.SetViewMode(ViewMode::Moisture);
                world.Observe();
            }
        }

        world.BeginDay();
        world.RunCreatures();
    }

    return 0;
}