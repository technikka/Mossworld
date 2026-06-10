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

int next_creature_id = 1;
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
        }
    }
}

void World::createCreature(Position position) {
    Creature mossling(MOSSLING, next_creature_id, position);
    creatures.push_back(mossling);
    next_creature_id++;
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
                createCreature(position);
                tile.occupant = CREATURE;  // setting enum
                creature_placed = true;
            }
        }
        counter = end_id;
    }
}

vector<Position> World::getAdjacentOpenPositions(Position position) {
    vector<Position> possible_positions;
    int x = position.x;
    int y = position.y;

    possible_positions.push_back({x - 1, y});  // left
    possible_positions.push_back({x + 1, y});  // right
    possible_positions.push_back({x, y - 1});  // up
    possible_positions.push_back({x, y + 1});  // down

    vector<Position> valid_positions;
    for (const Position& position : possible_positions) {
        if (position.x < 0 || position.x >= width) continue;
        if (position.y < 0 || position.y >= height) continue;

        Tile& tile = tiles[position.y][position.x];

        if (tile.occupant != CREATURE) {
            valid_positions.push_back(position);
        }
    }
    return valid_positions;
}

Position World::selectRandomPosition(Position current_position) {
    vector<Position> valid_positions =
        getAdjacentOpenPositions(current_position);
    int rand_index = rand() % (valid_positions.size());

    return valid_positions.at(rand_index);
}

void World::advanceDay() {
    // move creatures to a random adjacent tile
    for (Creature& creature : creatures) {
        Position current_position = creature.position;
        Position new_position = selectRandomPosition(current_position);
        creature.position = new_position;

        // update tiles
        Tile& current_tile = tiles[current_position.y][current_position.x];
        Tile& new_tile = tiles[new_position.y][new_position.x];
        current_tile.occupant = EMPTY;
        new_tile.occupant = CREATURE;
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

void World::printHUD(int day) {
    cout << "\n\n";  // space beneath world

    cout << "Day: " << day << "  |  "
         << "Mosslings: " << creature_count << "  |  "
         << "Nutrient Clusters: " << nutrient_count << "\n\n";
}