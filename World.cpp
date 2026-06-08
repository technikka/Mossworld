#include <iostream>
using namespace std;

#include "Creature.h"
#include "Tile.h"
#include "World.h"

World::World(int creature_count, int nutrient_count, int width, int height) {
    this->width = width;
    this->height = height;
    this->creature_count = creature_count;
    this->nutrient_count = nutrient_count;
    this->createTiles();
    this->placeCreatures();
}

int World::GetWidth() { return width; }
int World::GetHeight() { return height; }
int World::GetCreatureCount() { return creature_count; }
int World::GetNutrientCount() { return nutrient_count; }

void World::createTiles() {
    tiles.resize(height, vector<Tile>(width));
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            int id = row * width + column;
            OccupantType occupant = EMPTY;
            tiles[row][column] = Tile(id, column, row, occupant);
            // todo: don't delete this, understand what its doing first:
            // Position position = tiles[row][column].GetPosition();
            // cout << position.x << ", " << position.y << endl;
        }
    }
}

void World::placeCreatures() {
    int number_of_tiles = (height * width);
    unsigned int counter = 0;
    for (int i = 0; i < creature_count; i++) {
        int start_id = counter;
        // divide the tile space proportionally:
        int end_id = ((i + 1.0) / creature_count) * number_of_tiles;

        bool creature_placed = false;

        // * WARNING: this loop assumes at least one empty tile exists.
        // * Infinite-loop potential.
        while (!creature_placed) {
            int rand_pos = rand() % (end_id - start_id);

            // find the tile to update
            int tile_id = rand_pos + start_id;
            Position position = Tile::IdToCoordinates(tile_id, width, height);
            Tile& tile = tiles[position.y][position.x];

            // update tile
            if (tile.occupant == EMPTY) {
                // create create object
                Creature mossling(MOSSLING);
                tile.occupant = CREATURE;
                creature_placed = true;
            }
        }

        counter = end_id;
    }
}

void World::print() {
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            char symbol = OccupantTypeToChar(tiles[row][column].occupant);
            cout << ' ' << symbol << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

void World::printHUD(int turn_number) {
    cout << "\n\n";  // space beneath world

    cout << "Turn: " << turn_number << "  |  "
         << "Mosslings: " << creature_count << "  |  "
         << "Nutrient Clusters: " << nutrient_count << "\n\n";
}