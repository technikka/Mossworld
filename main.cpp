#include <iostream>

int main()
{
    char unoccupied_space = '.';
    char creature = 'K';
    char food = '*';

    int world_height = 6;
    int world_width = 9;

    // Get world size from user
    std::cout << "Select World Size";
    std::cout << std::endl;
    std::cout << "Height: (enter number 4 - 20)";
    std::cout << std::endl;
    std::cin >> world_height;

    std::cout << "Width: (enter number 4 - 20)";
    std::cout << std::endl;
    std::cin >> world_width;

    for (int row = 0; row < world_height; row++)
    {
        for (int column = 0; column < world_width; column++)
        {
            std::cout << unoccupied_space;
            std::cout << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}