#include <algorithm>
#include <iostream>
using namespace std;

#include "Creature.h"
#include "Tile.h"
#include "World.h"

World::World(int creature_count, int nutrient_cluster_count, int width,
             int height) {
    this->width = width;
    this->height = height;
    this->creature_count = creature_count;
    this->nutrient_cluster_count = nutrient_cluster_count;
    this->createTiles();
    this->PlaceEntities(CREATURE);
    this->PlaceEntities(NUTRIENT_CLUSTER);
}

int World::GetWidth() { return width; }
int World::GetHeight() { return height; }

int World::GetEntityCount(EntityType type) {
    switch (type) {
        case CREATURE:
            return creature_count;

        case NUTRIENT_CLUSTER:
            return nutrient_cluster_count;
        case EMPTY:
            return 0;
    }
}

void World::createTiles() {
    tiles.resize(height, vector<Tile>(width));
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            int id = row * width + column;
            EntityType occupant = EMPTY;
            tiles[row][column] = Tile(id, column, row, occupant);
        }
    }
}

void World::CreateEntity(EntityType type, Position position) {
    switch (type) {
        case CREATURE: {
            Creature mossling(MOSSLING, next_creature_id, position);
            creatures.push_back(mossling);
            next_creature_id++;
            break;
        }
        case NUTRIENT_CLUSTER: {
            // todo create nutrient cluster
            NutrientCluster nutrients(NUTRIENT_CLUSTER, position);
            nutrient_clusters.push_back(nutrients);
            break;
        }
        case EMPTY: {
            break;
        }
    }
}

void World::PlaceEntities(EntityType entity) {
    int number_of_tiles = (height * width);
    int entity_count = GetEntityCount(entity);
    unsigned int counter = 0;
    for (int i = 0; i < entity_count; i++) {
        int start_id = counter;
        // divide the tile space proportionally:
        int end_id = ((i + 1.0) / entity_count) * number_of_tiles;

        bool entity_placed = false;

        // * WARNING: this loop assumes at least one empty tile exists.
        // * Infinite-loop potential.
        while (!entity_placed) {
            int rand_pos = rand() % (end_id - start_id);

            // find the tile to update
            int tile_id = rand_pos + start_id;
            Position position = Tile::IdToCoordinates(tile_id, width, height);
            Tile& tile = tiles[position.y][position.x];

            // update tile
            if (tile.occupant == EMPTY) {
                CreateEntity(entity, position);
                tile.occupant = entity;  // setting enum
                entity_placed = true;
            }
        }
        counter = end_id;
    }
}

void World::PlaceNutrientClusters() {}

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

Position World::selectPosition(Creature& creature) {
    vector<Position> valid_positions =
        getAdjacentOpenPositions(creature.position);

    if (valid_positions.empty()) {
        return creature.position;
    }

    // try to avoid back-tracking to previous position
    if (creature.position_history.size() > 1 && valid_positions.size() > 1) {
        Position previous_position =
            creature.position_history.at(creature.position_history.size() - 2);
        auto it = find(valid_positions.begin(), valid_positions.end(),
                       previous_position);
        if (it != valid_positions.end()) {
            valid_positions.erase(it);
        }
    }

    int rand_index = rand() % (valid_positions.size());

    return valid_positions.at(rand_index);
}

void World::MoveCreature(Creature& creature) {
    Position current_position = creature.position;
    Position new_position = selectPosition(creature);
    creature.position = new_position;
    creature.position_history.push_back(new_position);

    // update tiles
    Tile& current_tile = tiles[current_position.y][current_position.x];
    Tile& new_tile = tiles[new_position.y][new_position.x];
    current_tile.occupant = EMPTY;
    new_tile.occupant = CREATURE;

    // cout << "Creature " << creature.GetId() << " moved from ("
    //      << current_position.x << ", " << current_position.y << ") to ("
    //      << new_position.x << ", " << new_position.y << ")\n";
}

void World::advanceDay() {
    for (Creature& creature : creatures) {
        MoveCreature(creature);
    }
}

void World::print() {
    cout << "\n\n";  // space above world
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            char symbol = EntityTypeToChar(tiles[row][column].occupant);
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
         << "Nutrient Clusters: " << nutrient_cluster_count << "\n\n";
}