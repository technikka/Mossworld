#include <algorithm>
#include <iomanip>  // for printing
#include <iostream>
#include <limits>
using namespace std;

#include "Creature.h"
#include "Narration.h"
#include "Position.h"
#include "Tile.h"
#include "ViewMode.h"
#include "World.h"

vector<string> mossling_traits = {
    "Bold",   "Diligent",  "Friendly", "Gentle",     "Humble",
    "Joyful", "Keen",      "Mindful",  "Nimble",     "Patient",
    "Quiet",  "Resilient", "Serene",   "Thoughtful", "Watchful"};

World::World(WorldConfig& config)
    : config(config),
      view_mode(config.view_mode),
      width(config.width),
      height(config.height),
      creature_count(config.creature_start_count),
      nutrient_cluster_count(config.nutrient_cluster_start_count)

{
    available_traits = mossling_traits;

    // prevent vector reallocation from invalidating tile occupant pointers
    creatures.reserve(config.creature_start_count);
    nutrient_clusters.reserve(config.nutrient_cluster_start_count);

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

vector<pair<int, int>> World::GetLinearZones(int zone_count) const {
    vector<pair<int, int>> zones;

    int number_of_tiles = width * height;

    for (int i = 0; i < zone_count; i++) {
        int start_id = (i * number_of_tiles) / zone_count;
        int end_id = ((i + 1) * number_of_tiles) / zone_count;

        zones.push_back({start_id, end_id});
    }

    return zones;
}

Tile* World::SelectRandomEmptyTile(int start_id, int end_id) {
    Tile* tile = nullptr;
    do {
        int rand_pos = rand() % (end_id - start_id);
        int tile_id = rand_pos + start_id;

        Position position = Tile::IdToCoordinates(tile_id, width, height);
        tile = &tiles[position.y][position.x];

    } while (!tile->IsEmpty());
    return tile;
}

// Places entity on random tile between start_id and end_id of Tile ID.
void World::PlaceEntity(EntityType type, int start_id, int end_id) {
    Tile* tile = SelectRandomEmptyTile(start_id, end_id);
    Entity* entity = CreateEntity(type, tile);
    tile->SetOccupant(entity);
}

void World::PlaceEntities(EntityType entity) {
    auto zones = GetLinearZones(GetEntityCount(entity));

    for (const auto& zone : zones) {
        PlaceEntity(entity, zone.first, zone.second);
    }
}

void World::ApplyMoistureRing(Tile* tile, int amount, int distance,
                              double percent) {
    Position pos = tile->GetPosition();
    int spread_amount = amount * percent;

    for (int y_offset = -distance; y_offset <= distance; y_offset++) {
        for (int x_offset = -distance; x_offset <= distance; x_offset++) {
            if (x_offset == 0 && y_offset == 0) {
                continue;
            }

            if (abs(x_offset) != distance && abs(y_offset) != distance) {
                continue;
            }

            int new_x = pos.x + x_offset;
            int new_y = pos.y + y_offset;

            if (new_y < 0 || new_y >= height || new_x < 0 || new_x >= width) {
                continue;
            }

            tiles[new_y][new_x].AddMoisture(spread_amount, config.max_moisture);
        }
    }
}

void World::PlaceMoistureSpread(Tile* tile, int amount, int spread_distance) {
    // First ring of tiles gets 70% moisture.
    double percent = 0.7;
    for (int i = 1; i <= spread_distance; i++) {
        ApplyMoistureRing(tile, amount, i, percent);
        percent *= 0.5;
    }
}

void World::PlaceMoistureSource(int amount, int start_id, int end_id,
                                int spread_distance) {
    Tile* tile = SelectRandomEmptyTile(start_id, end_id);
    tile->AddMoisture(amount, config.max_moisture);
    PlaceMoistureSpread(tile, amount, spread_distance);
}

void World::PlaceMoistureSources(int initial_amount, int sources,
                                 int spread_distance) {
    auto zones = GetLinearZones(config.moisture_sources);

    for (const auto& zone : zones) {
        PlaceMoistureSource(initial_amount, zone.first, zone.second,
                            spread_distance);
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
    // spawn a new cluster every [nutrient_spawn_interval] days
    bool multiple_of_interval = (day % config.nutrient_spawn_interval == 0);
    if (multiple_of_interval &&
        nutrient_cluster_count < config.nutrient_cluster_start_count) {
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

void World::ClearMoisture() {
    for (auto& row : tiles) {
        for (auto& tile : row) {
            tile.SetMoisture(0);
        }
    }
}

void World::AddMorningDew() {
    PlaceMoistureSources(config.morning_dew_moisture, config.moisture_sources,
                         config.dew_spread_distance);
}

void World::ApplyEnvironmentalConditions() {
    ClearMoisture();  // reset then regenerate for now
    AddMorningDew();
};

void World::BeginDay() {
    day++;
    ManageNutrientClusters();
    ApplyEnvironmentalConditions();
}

void World::RunCreatures() {
    for (auto& creature : creatures) {
        creature->LoseDailyEnergy();
        creature->ClearObjective();
        // * dereferences the unique_ptr to pass the Creature by reference.
        AssessNeeds(*creature);
        MoveCreature(*creature);
    }
}

void World::SetViewMode(ViewMode mode) { view_mode = mode; }

ViewMode World::GetViewMode(ViewMode mode) { return mode; }

void World::PrintView() {
    switch (view_mode) {
        case ViewMode::World:
            PrintWorldView();
            break;
        case ViewMode::Moisture:
            PrintMoistureView();
            break;
    }
}

void World::Observe() {
    PrintView();
    PrintStatusBar();
    PrintEnergyBar();
    PrintObserverMenu();
}

void World::PrintMoistureView() {
    cout << "\n" << right;  // space above world
    for (int row = 0; row < height; row++) {
        cout << string(config.left_margin, ' ');
        for (int column = 0; column < width; column++) {
            cout << setw(4) << tiles[row][column].GetMoisture();
        }
        cout << endl;
    }
    cout << endl;
}

void World::PrintWorldView() {
    cout << "\n" << right;  // space above world
    for (int row = 0; row < height; row++) {
        cout << string(config.left_margin, ' ');
        for (int column = 0; column < width; column++) {
            cout << setw(4) << tiles[row][column].GetSymbol();
        }
        cout << endl;
    }
    cout << endl;
}

string World::EnergyBar(int energy, int max_energy) {
    string bar = "[";

    for (int i = 0; i < max_energy; i++) {
        if (i < energy) {
            bar += "■";
        } else {
            bar += "□";
        }
    }

    bar += "]";

    return bar;
}

void World::PrintObserverMenu() const {
    cout << "\n";
    cout << "A new day is unfolding.\n\n";
    cout << " Observe ➜ Enter |  Switch View ➜ 1: World  2: Moisture  |  Leave "
            "➜ "
            "'exit'\n";
    cout << "❯ ";
}

void World::PrintStatusBar() const {
    cout << "\n";
    cout << " Day: " << left << setw(5) << day << " Mosslings: " << setw(5)
         << creature_count << " Nutrient Clusters: " << setw(5)
         << nutrient_cluster_count << "Viewing: " << ModeToString(view_mode)
         << "\n";
    cout << "\n";
}

void World::PrintEnergyBar() {
    cout << "\n";
    cout << "──── Mossling Vitality ─────\n";
    for (auto& creature : creatures) {
        cout << creature->GetTrait() << " Mossling: "
             << EnergyBar(creature->GetEnergy(), creature->GetMaxEnergy())
             << endl;
    }
}

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