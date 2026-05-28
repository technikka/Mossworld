#include <iostream>
#include <string>
#include <cstdlib> // for rand()
using namespace std;

int main()
{
    char unoccupied_space = '.';
    char creature = 'K';
    char food = '*';

    int world_height = 6;
    int world_width = 9;

    int turn_number = 1;
    int creature_count = 3;
    int food_count = 2;

    // Get world size from user
    // COMMENTED OUT FOR TESTING
    // cout << "Select World Size\n";
    // cout << "Height: (enter number 4 - 20)\n";
    // cin >> world_height;
    // cout << "Width: (enter number 4 - 20)\n";
    // cin >> world_width;

    // Display world info
    cout
        << "\nWelcome to Mossworld.\n";
    cout << "Tiny creatures stir beneath the dawn mist.\n\n";

    // Display world status
    cout << "Turn: " << turn_number << "  |  " << "Creatures: " << creature_count << "  |  " << "Food: " << food_count << "\n\n";

    for (int row = 0; row < world_height; row++)
    {
        for (int column = 0; column < world_width; column++)
        {
            cout << unoccupied_space;
            cout << " ";
        }
        cout << endl;
    }
    cout << "\n\n";

    return 0;
}