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
      creature_count(config.creature_start_count) {
    available_traits = mossling_traits;

    // prevent vector reallocation from invalidating tile occupant pointers
    creatures.reserve(config.creature_start_count);
    nutrient_clusters.reserve(config.nutrient_cluster.start_count);

    this->createTiles();
    this->InitializeEnvironment();
    this->InitializeCreatures();
    this->InitializeNutrientClusters();
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

// Will only return a tile that does not have an occupant.
Tile* World::SelectRandomFertileTile(FertilityLevel fertility_level) {
    Tile* tile = nullptr;
    vector<Tile*> fertile_tiles;

    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            Tile& tile = tiles[row][column];

            if (tile.GetFertilityLevel() == fertility_level &&
                tile.GetOccupant() == nullptr) {
                fertile_tiles.push_back(&tile);
            }
        }
    }

    if (fertile_tiles.size() != 0) {
        int rand_index = rand() % fertile_tiles.size();
        return fertile_tiles.at(rand_index);
    }

    return nullptr;
}

void World::PlaceEntity(EntityType type, Tile& tile) {
    Entity* entity = CreateEntity(type, &tile);
    tile.SetOccupant(entity);
}

// Place by zone
void World::InitializeCreatures() {
    auto zones = GetLinearZones(GetEntityCount(CREATURE));

    for (const auto& zone : zones) {
        Tile* tile = SelectRandomEmptyTile(zone.first, zone.second);
        if (tile == nullptr) {
            return;
        }
        PlaceEntity(CREATURE, *tile);
    }
}

void World::InitializeNutrientClusters() {
    int count = config.nutrient_cluster.start_count;
    for (int i = 0; i < count; i++) {
        Tile* tile = SelectRandomFertileTile(FertilityLevel::High);
        if (tile == nullptr) {
            tile = SelectRandomFertileTile(FertilityLevel::Moderate);
        }
        if (tile == nullptr) {
            tile = SelectRandomFertileTile(FertilityLevel::Low);
        }
        if (tile == nullptr) {
            return;
        }
        PlaceEntity(NUTRIENT_CLUSTER, *tile);
    }
}

void World::PlaceNutrientCluster(FertilityLevel fertility_level) {
    Tile* tile = SelectRandomFertileTile(fertility_level);
    if (tile != nullptr) {
        PlaceEntity(NUTRIENT_CLUSTER, *tile);
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

            tiles[new_y][new_x].AdjustMoisture(spread_amount);
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
    tile->AdjustMoisture(amount);
    PlaceMoistureSpread(tile, amount, spread_distance);
}

// Place by zone and config.source_count.
void World::PlaceMoistureSources(int initial_amount, int sources,
                                 int spread_distance) {
    auto zones = GetLinearZones(config.moisture.source_count);

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

int World::ScoreObjective(Tile& tile, Creature& creature) {
    Tile* objective_tile = creature.GetObjective();
    Tile* creature_tile = creature.GetCurrentTile();
    int current_distance = objective_tile->GetPosition().ManhattanDistanceTo(
        creature_tile->GetPosition());
    int new_distance =
        objective_tile->GetPosition().ManhattanDistanceTo(tile.GetPosition());

    if (new_distance < current_distance) {
        return 10;
    } else if (new_distance == current_distance) {
        return 5;
    }

    return 0;
}

int World::ScoreMoisture(Tile& tile, Creature& creature) {
    int moisture_difference =
        abs(tile.GetMoisture() - creature.GetIdealMoisture());

    return 10 - moisture_difference;
}

int World::ScoreBacktracking(Tile& tile, Creature& creature) {
    Tile* previous_tile =
        creature.tile_history.at(creature.tile_history.size() - 2);

    if (previous_tile == &tile) {
        return -5;
    }

    return 0;
}

int World::ScoreTile(Tile& tile, Creature& creature) {
    int score = 0;

    if (creature.HasObjective()) {
        score += ScoreObjective(tile, creature);
    }

    score += ScoreMoisture(tile, creature);

    if (creature.tile_history.size() > 1) {
        score += ScoreBacktracking(tile, creature);
    }

    return score;
}

Tile* World::SelectCreatureTile(Creature& creature) {
    vector<Tile*> valid_tiles = GetAdjacentOpenTiles(creature.GetCurrentTile());

    if (valid_tiles.empty()) {
        return creature.GetCurrentTile();
    }

    struct TileScore {
        Tile* tile;
        int score;
    };

    vector<TileScore> tile_scores;
    Tile* highest_scoring_tile = valid_tiles.front();
    // Double scores first tile for now.
    int highest_score = ScoreTile(*highest_scoring_tile, creature);

    // Score each valid tile based on creature needs & preferences.
    for (auto& tile : valid_tiles) {
        TileScore tile_score{tile, ScoreTile(*tile, creature)};
        tile_scores.push_back(tile_score);
        if (tile_score.score > highest_score) {
            highest_scoring_tile = tile;
            highest_score = tile_score.score;
        }
    }
    return highest_scoring_tile;
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
    int chance = rand() % 9;

    bool is_high_fertility_growth_day =
        (day % config.nutrient_cluster.high_fertility_growth_interval == 0);
    if (is_high_fertility_growth_day) {
        PlaceNutrientCluster(FertilityLevel::High);
        return;
    }

    bool is_moderate_fertility_growth_day =
        (day % config.nutrient_cluster.moderate_fertility_growth_interval == 0);
    if (is_moderate_fertility_growth_day) {
        PlaceNutrientCluster(FertilityLevel::Moderate);
        return;
    }

    bool is_low_fertility_growth_day =
        (day % config.nutrient_cluster.low_fertility_growth_interval == 0);
    if (is_low_fertility_growth_day) {
        PlaceNutrientCluster(FertilityLevel::Low);
        return;
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

void World::ApplyMorningDew() {
    // For now, apply morning dew to new random tiles.
    PlaceMoistureSources(config.moisture.morning_dew_amount,
                         config.moisture.source_count,
                         config.moisture.dew_spread_distance);
}

void World::ApplyEvaporation() {
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            Tile& tile = tiles[row][column];

            if (tile.GetMoisture() > 0) {
                tile.AdjustMoisture(-1);
            }
        }
    }
}

void World::UpdateTileFertility(Tile& tile) {
    int fertility_change = 0;
    if (tile.HasNutrientCluster()) {
        fertility_change -= 1;
    }
    MoistureLevel moisture_level = tile.GetMoistureLevel();
    if (moisture_level == MoistureLevel::Dry ||
        moisture_level == MoistureLevel::Saturated) {
        fertility_change -= 1;
    } else if (moisture_level == MoistureLevel::Ideal) {
        fertility_change += 1;
    }
    tile.AdjustFertility(fertility_change);
}

void World::UpdateFertility() {
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            UpdateTileFertility(tiles[row][column]);
        }
    }
}

void World::IntializeMoisture() {
    PlaceMoistureSources(config.moisture.morning_dew_initial_amount,
                         config.moisture.source_count,
                         config.moisture.dew_spread_distance);
}

void World::InitializeTileFertility() {
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            Tile& tile = tiles[row][column];
            MoistureLevel moisture = tile.GetMoistureLevel();

            if (moisture == MoistureLevel::Dry ||
                moisture == MoistureLevel::Saturated) {
                tile.SetFertility(0);
            } else if (moisture == MoistureLevel::Damp ||
                       moisture == MoistureLevel::Wet) {
                tile.SetFertility(config.fertility.initial_low);
            } else if (moisture == MoistureLevel::Ideal) {
                tile.SetFertility(config.fertility.initial_high);
            }
        }
    }
}

void World::InitializeEnvironment() {
    IntializeMoisture();
    InitializeTileFertility();
}

void World::UpdateEnvironment() {
    ApplyMorningDew();
    ApplyEvaporation();
    UpdateFertility();
}

void World::BeginDay() {
    day++;
    ManageNutrientClusters();
    if (day != 1) {
        UpdateEnvironment();
    }
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
    PrintCreatureBar();
    PrintObserverMenu();
    // PrintFertilityView();
}

void World::PrintMoistureView() {
    cout << "\n" << right;  // space above world
    for (int row = 0; row < height; row++) {
        cout << string(config.left_margin, ' ');
        for (int column = 0; column < width; column++) {
            Tile& tile = tiles[row][column];
            if (tile.HasCreature()) {
                string cell = "[";
                cell += tile.GetSymbol();
                cell += "]";

                cout << setw(4) << cell;
                continue;
            }
            cout << setw(4) << tile.GetMoisture();
        }
        cout << endl;
    }
    cout << endl;
}

//! temporary
void World::PrintFertilityView() {
    cout << "\n" << right;  // space above world
    for (int row = 0; row < height; row++) {
        cout << string(config.left_margin, ' ');
        for (int column = 0; column < width; column++) {
            Tile& tile = tiles[row][column];
            if (tile.HasCreature()) {
                string cell = "[";
                cell += tile.GetSymbol();
                cell += "]";

                cout << setw(4) << cell;
                continue;
            }
            cout << setw(4) << tile.GetFertility();
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

string World::MoistureBar(int current, int ideal) {
    return " Current " + to_string(current) + " |  Ideal " + to_string(ideal);
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

void World::PrintCreatureBar() {
    cout << "\n";
    cout << "── Mossling Vitality ──     ── Moisture ──\n";
    for (auto& creature : creatures) {
        cout << left << setw(11) << creature->GetTrait()
             << EnergyBar(creature->GetEnergy(), creature->GetMaxEnergy())
             << MoistureBar(creature->GetCurrentTile()->GetMoisture(),
                            creature->GetIdealMoisture())
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