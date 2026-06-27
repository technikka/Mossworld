#include <algorithm>
#include <iostream>
#include <limits>
using namespace std;

#include "Creature.h"
#include "Narration.h"
#include "Position.h"
#include "Tile.h"
#include "World.h"

vector<string> mossling_traits = {
    "Bold",   "Diligent",  "Friendly", "Gentle",     "Humble",
    "Joyful", "Keen",      "Mindful",  "Nimble",     "Patient",
    "Quiet",  "Resilient", "Serene",   "Thoughtful", "Watchful"};

World::World(int width, int height) {
    this->width = width;
    this->height = height;
    this->creature_count = creature_start_count;
    this->nutrient_cluster_count = nutrient_cluster_start_count;
    available_traits = mossling_traits;

    // prevent vector reallocation from invalidating tile occupant pointers
    creatures.reserve(creature_start_count);
    nutrient_clusters.reserve(nutrient_cluster_start_count);

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
    }
}

void World::createTiles() {
    tiles.resize(height, vector<Tile>(width));
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            int id = row * width + column;
            tiles[row][column] = Tile(id, column, row);
        }
    }
}

Entity* World::CreateEntity(EntityType type, Tile* tile) {
    switch (type) {
        case CREATURE: {
            string trait = GetTrait();
            Creature mossling(MOSSLING, next_creature_id, tile, trait);
            creatures.push_back(make_unique<Creature>(mossling));
            next_creature_id++;
            // get() returns the raw pointer inside the unique_ptr
            return creatures.back().get();
        }
        case NUTRIENT_CLUSTER: {
            NutrientCluster nutrients(tile);
            nutrient_clusters.push_back(
                (make_unique<NutrientCluster>(nutrients)));
            return nutrient_clusters.back().get();
        }
    }
}

void World::RemoveEntity(EntityType type, Tile* tile) {
    switch (type) {
        case CREATURE: {
            // place holder
            break;
        }
        case NUTRIENT_CLUSTER: {
            // find the nutrient cluster by position
            // remove it from the nutrient cluster vector
            break;
        }
    }
};

// Places entity on random tile between start_id and end_id of Tile ID.
void World::PlaceEntity(EntityType type, int start_id, int end_id) {
    Tile* tile = nullptr;

    do {
        int rand_pos = rand() % (end_id - start_id);
        int tile_id = rand_pos + start_id;

        Position position = Tile::IdToCoordinates(tile_id, width, height);
        tile = &tiles[position.y][position.x];

    } while (!tile->IsEmpty());

    Entity* entity = CreateEntity(type, tile);
    tile->SetOccupant(entity);
}

void World::PlaceEntities(EntityType entity) {
    int number_of_tiles = (height * width);
    int entity_count = GetEntityCount(entity);
    unsigned int counter = 0;
    for (int i = 0; i < entity_count; i++) {
        int start_id = counter;
        // divide the tile space proportionally:
        int end_id = ((i + 1.0) / entity_count) * number_of_tiles;

        PlaceEntity(entity, start_id, end_id);
        counter = end_id;
    }
}

vector<Tile*> World::GetAdjacentOpenTiles(Tile* current_tile) {
    vector<Position> possible_positions;
    Position position = current_tile->GetPosition();
    int x = position.x;
    int y = position.y;

    possible_positions.push_back({x - 1, y});  // left
    possible_positions.push_back({x + 1, y});  // right
    possible_positions.push_back({x, y - 1});  // up
    possible_positions.push_back({x, y + 1});  // down

    vector<Tile*> valid_tiles;

    for (const Position& position : possible_positions) {
        if (position.x < 0 || position.x >= width) continue;
        if (position.y < 0 || position.y >= height) continue;

        Tile* tile = &tiles[position.y][position.x];

        // EntityType CREATURE is the only obstacle to occupying a new tile.
        if (!tile->HasCreature()) {
            valid_tiles.push_back(tile);
        }
    }

    return valid_tiles;
}

Tile* World::SelectTileTowardObjective(Creature& creature,
                                       const vector<Tile*>& valid_tiles) {
    Tile* objective_tile = creature.GetObjective();
    Tile* closest_tile = creature.GetCurrentTile();
    int closest_distance = creature.GetPosition().ManhattanDistanceTo(
        objective_tile->GetPosition());

    for (auto& tile : valid_tiles) {
        int new_distance = tile->GetPosition().ManhattanDistanceTo(
            objective_tile->GetPosition());

        // if distance is less than current distance to objective
        if (new_distance < closest_distance) {
            closest_distance = new_distance;
            closest_tile = tile;
        }
    }

    if (closest_tile != creature.GetCurrentTile()) {
        return closest_tile;
    }
    return nullptr;
}

Tile* World::SelectCreatureTile(Creature& creature) {
    vector<Tile*> valid_tiles = GetAdjacentOpenTiles(creature.GetCurrentTile());

    if (valid_tiles.empty()) {
        return creature.GetCurrentTile();
    }

    // select a valid tile that brings creature closer to objective
    if (creature.HasObjective()) {
        Tile* closer_tile = SelectTileTowardObjective(creature, valid_tiles);

        if (closer_tile != nullptr) {
            return closer_tile;
        }
    }

    // try to avoid back-tracking to previous tile
    if (creature.tile_history.size() > 1 && valid_tiles.size() > 1) {
        Tile* previous_tile =
            creature.tile_history.at(creature.tile_history.size() - 2);
        auto it = find(valid_tiles.begin(), valid_tiles.end(), previous_tile);
        if (it != valid_tiles.end()) {
            valid_tiles.erase(it);
        }
    }

    int rand_index = rand() % (valid_tiles.size());

    return valid_tiles.at(rand_index);
}

void World::HandleNutrientConsumption(Creature& creature, Tile* tile) {
    cout << "\n\n"
         << Narration::NutrientFound(creature.GetNutrientNeed(),
                                     creature.GetType(), creature.GetTrait());
    creature.RestoreEnergy();
    nutrient_cluster_count -= 1;

    // remove nutrient cluster from its container
    auto it = find_if(nutrient_clusters.begin(), nutrient_clusters.end(),
                      [tile](const auto& cluster) {
                          return cluster->GetCurrentTile() == tile;
                      });

    if (it != nutrient_clusters.end()) {
        nutrient_clusters.erase(it);
    }
}

void World::MoveCreature(Creature& creature) {
    // move creature to selected tile
    Tile* current_tile = creature.GetCurrentTile();
    Tile* new_tile = SelectCreatureTile(creature);

    if (new_tile->HasNutrientCluster()) {
        HandleNutrientConsumption(creature, new_tile);
    }

    creature.SetCurrentTile(new_tile);
    creature.tile_history.push_back(new_tile);

    // update tiles
    current_tile->SetOccupant(nullptr);
    new_tile->SetOccupant(&creature);
}

void World::ManageNutrientClusters() {
    // spawn a new cluster every fifth day
    bool multiple_of_five = (day % 5 == 0);
    if (multiple_of_five &&
        nutrient_cluster_count < nutrient_cluster_start_count) {
        // place on any random tile index
        PlaceEntity(NUTRIENT_CLUSTER, 0, ((height * width)));
        nutrient_cluster_count++;
    }
}

Tile* World::FindNearestNutrientCluster(Creature& creature) {
    Tile* nearest_tile = nullptr;
    int shortest_distance = numeric_limits<int>::max();

    Position creature_position = creature.GetPosition();

    for (auto& nutrient_cluster : nutrient_clusters) {
        Tile* nutrient_tile = nutrient_cluster->GetCurrentTile();

        int distance = creature_position.ManhattanDistanceTo(
            nutrient_cluster->GetPosition());

        if (distance < shortest_distance) {
            shortest_distance = distance;
            nearest_tile = nutrient_tile;
        }
    }
    return nearest_tile;
}

void World::SelectNutrientObjective(Creature& creature, int max_distance) {
    Tile* tile = FindNearestNutrientCluster(creature);

    if (tile == nullptr) {
        return;
    }

    int distance =
        creature.GetPosition().ManhattanDistanceTo(tile->GetPosition());

    if (distance > max_distance) {
        return;
    }

    creature.SetObjective(tile);
}

// currently only assessing NutrientNeed
void World::AssessNeeds(Creature& creature) {
    NutrientNeed nutrient_need = creature.GetNutrientNeed();

    switch (nutrient_need) {
        case NutrientNeed::High:
            // once death implemented: change to creature.GetEnergy()
            SelectNutrientObjective(creature, creature.GetMaxEnergy());
            break;
        case NutrientNeed::Medium:
            SelectNutrientObjective(creature, 2);
            break;
        case NutrientNeed::Low:
            break;
    }
}

void World::advanceDay() {
    day++;
    ManageNutrientClusters();

    for (auto& creature : creatures) {
        creature->LoseDailyEnergy();
        creature->ClearObjective();
        // * dereferences the unique_ptr to pass the Creature by reference.
        AssessNeeds(*creature);
        MoveCreature(*creature);
    }
}

void World::Print() {
    cout << "\n\n";  // space above world
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            cout << ' ' << tiles[row][column].GetSymbol() << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

void World::PrintHUD() {
    cout << "\n\n";  // space beneath world

    cout << "Day: " << day << "\n\n";
    cout << "------Energy------ \n";
    for (auto& creature : creatures) {
        cout << creature->GetTrait() << " Mossling: " << creature->GetEnergy()
             << '/' << creature->GetMaxEnergy() << endl;
    }
    cout << "\nNutrient Clusters: " << nutrient_cluster_count << "\n\n";
}

// selects random trait from available then updates available_traits
string World::GetTrait() {
    // refresh available_traits when used up
    string trait;
    if (available_traits.size() < 1) {
        available_traits = mossling_traits;
    }
    int rand_index = rand() % available_traits.size();
    trait = available_traits[rand_index];
    available_traits.erase(available_traits.begin() + rand_index);
    return trait;
}