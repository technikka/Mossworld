#include <array>
#include <chrono>
#include <cstdlib>  // for rand()
#include <ctime>
#include <iostream>
#include <string>

using namespace std;  // needs to be above header files

#include "Creature.h"
#include "Narration.h"
#include "World.h"

void BenchmarkSimulation(World& world) {
    int sim_runs = 1000;

    Narration::SetEnabled(false);
    auto start = chrono::steady_clock::now();

    for (int i = 0; i < sim_runs; i++) {
        world.BeginDay();
        world.RunCreatures();
        world.UpdateMemory();
    }

    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "Simulation took " << duration.count() << " microseconds for "
         << sim_runs << " runs.";
}

int main(int argc, char* argv[]) {
    srand(time(0));  // seed rand

    const WorldConfig config;
    World world(config);  // instantiate world

    if (argc > 1 && string(argv[1]) == "--benchmark") {
        BenchmarkSimulation(world);
        return 0;
    }

    world.PrintDivider();
    world.PrintLine("Welcome to Mossworld.");
    world.PrintLine("Tiny creatures stir among the morning dew.");
    cout << "\n\n";

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

            if (input == "3") {
                world.SetViewMode(ViewMode::Sunlight);
                world.Observe();
            }

            if (input == "4") {
                world.SetViewMode(ViewMode::Elevation);
                world.Observe();
            }

            if (input == "j" || input == "J") {
                world.ToggleJournal();
                world.Observe();
            }

            if (input == "0") {
                world.SetViewMode(ViewMode::Inspector);
                world.Observe();
            }
        }

        world.BeginDay();
        world.RunCreatures();
        world.UpdateMemory();
    }

    return 0;
}