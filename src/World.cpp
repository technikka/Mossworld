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

World::World(const WorldConfig& config)
    : config(config),
      view_mode(config.view_mode),
      width(config.width),
      height(config.height) {
    available_traits = mossling_traits;

    // prevent vector reallocation from invalidating tile occupant pointers
    creatures.reserve(config.creature.start_count);
    nutrient_clusters.reserve(config.nutrient_cluster.start_count);

    this->createTiles();
    this->InitializeEnvironment();
    this->InitializeCreatures();
    this->InitializeNutrientClusters();
}

int World::GetWidth() { return width; }
int World::GetHeight() { return height; }

int World::GetEntityCount(EntityType type) const {
    switch (type) {
        case CREATURE:
            return creatures.size();

        case NUTRIENT_CLUSTER:
            return nutrient_clusters.size();
    }
}

void World::createTiles() {
    tiles.clear();
    tiles.resize(height);

    for (int row = 0; row < height; row++) {
        tiles[row].reserve(width);

        for (int column = 0; column < width; column++) {
            int id = row * width + column;
            tiles[row].emplace_back(id, column, row, config);
        }
    }
}

Entity* World::CreateEntity(EntityType type, Tile* tile) {
    switch (type) {
        case CREATURE: {
            string trait = GetTrait();
            Creature mossling(MOSSLING, next_creature_id, tile, trait, config);
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

template <typename Callable>
void World::ForEachTile(Callable callable) {
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            callable(tiles[row][column]);
        }
    }
}

template <typename Callable>
void World::ForEachTileWithPosition(Callable callable) {
    for (int row = 0; row < height; ++row)
        for (int column = 0; column < width; ++column)
            callable(tiles[row][column], Position{row, column});
}

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

// Will return a tile regardless of occupant or other effects.
Tile* World::SelectRandomTile(int start_id, int end_id) {
    Tile* tile = nullptr;
    int rand_pos = rand() % (end_id - start_id);
    int tile_id = rand_pos + start_id;

    Position position = Tile::IdToCoordinates(tile_id, width);
    tile = &tiles[position.y][position.x];

    return tile;
}

Tile* World::SelectRandomEmptyTile(int start_id, int end_id) {
    Tile* tile = nullptr;
    do {
        int rand_pos = rand() % (end_id - start_id);
        int tile_id = rand_pos + start_id;

        Position position = Tile::IdToCoordinates(tile_id, width);
        tile = &tiles[position.y][position.x];

    } while (!tile->IsEmpty());  // * Potential infinite loop
    return tile;
}

Tile* World::SelectRandomNutrientGrowthTile() {
    vector<Tile*> growth_tiles;
    int highest_growth = numeric_limits<int>::min();

    ForEachTile([&](Tile& tile) {
        if (!tile.IsEmpty()) {
            return;
        }
        int growth = CalculateNutrientGrowth(tile);

        if (growth > highest_growth) {
            highest_growth = growth;
            growth_tiles.clear();
            growth_tiles.push_back(&tile);
        } else if (growth == highest_growth) {
            growth_tiles.push_back(&tile);
        }
    });

    if (growth_tiles.size() != 0) {
        int rand_index = rand() % growth_tiles.size();
        return growth_tiles.at(rand_index);
    }

    return nullptr;
}

void World::PlaceEntity(EntityType type, Tile& tile) {
    Entity* entity = CreateEntity(type, &tile);
    tile.SetOccupant(entity);
}

// Place by zone
void World::InitializeCreatures() {
    int count = config.creature.start_count;
    auto zones = GetLinearZones(count);

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
        Tile* tile = SelectRandomNutrientGrowthTile();
        if (tile == nullptr) {
            return;
        }

        PlaceEntity(NUTRIENT_CLUSTER, *tile);
    }
}

void World::PlaceNutrientCluster(Tile& tile) {
    PlaceEntity(NUTRIENT_CLUSTER, tile);
}

template <typename Callable>
void World::ForEachTileInRing(Tile* tile, int spread_amount, int distance,
                              Callable callable) {
    Position pos = tile->GetPosition();

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

            callable(tiles[new_y][new_x], spread_amount);
        }
    }
}

void World::PlaceMoistureSpread(Tile* tile, int amount, int spread_distance) {
    // First ring of tiles gets 70% moisture.
    double percent = 0.7;
    for (int i = 1; i <= spread_distance; i++) {
        amount *= percent;
        ForEachTileInRing(tile, amount, i,
                          [](Tile& ring_tile, int spread_amount) {
                              ring_tile.AdjustMoisture(spread_amount);
                          });
        percent *= 0.5;
    }
}

void World::PlaceSunlightSpread(Tile* tile, int amount, int spread_distance) {
    // Each ring gets 1 less intensity
    for (int i = 1; i <= spread_distance; i++) {
        ForEachTileInRing(tile, amount, i,
                          [](Tile& ring_tile, int spread_amount) {
                              ring_tile.SetSunlightIfGreater(spread_amount);
                          });
        amount -= 1;
    }
}

void World::PlaceMoistureSource(int amount, int start_id, int end_id,
                                int spread_distance) {
    Tile* tile = SelectRandomTile(start_id, end_id);
    tile->AdjustMoisture(amount);
    PlaceMoistureSpread(tile, amount, spread_distance);
}

// Place by zone and config.source_count.
void World::PlaceMoistureSources(int initial_amount, int sources,
                                 int spread_distance) {
    auto zones = GetLinearZones(sources);

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

int World::ScoreSunlight(Tile& tile, Creature& creature) {
    int sunlight_difference =
        abs(tile.GetSunlight() - creature.GetIdealSunlight());

    return 10 - sunlight_difference;
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
    score += ScoreSunlight(tile, creature);

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

int World::CalculateFertilityGrowthModifier(const Tile& tile) {
    int growth = 0;
    FertilityLevel fertility_level = tile.GetFertilityLevel();

    if (fertility_level == FertilityLevel::High) {
        growth += config.nutrient_growth.high_fertility_modifier;
    } else if (fertility_level == FertilityLevel::Moderate) {
        growth += config.nutrient_growth.moderate_fertility_modifier;
    } else if (fertility_level == FertilityLevel::Low) {
        growth += config.nutrient_growth.low_fertility_modifier;
    }
    return growth;
}

int World::CalculateMoistureGrowthModifier(const Tile& tile) {
    int growth = 0;
    MoistureLevel moisture_level = tile.GetMoistureLevel();

    if (moisture_level == MoistureLevel::Saturated) {
        growth += config.nutrient_growth.saturated_moisture_modifier;
    } else if (moisture_level == MoistureLevel::Wet) {
        growth += config.nutrient_growth.wet_moisture_modifier;
    } else if (moisture_level == MoistureLevel::Ideal) {
        growth += config.nutrient_growth.ideal_moisture_modifier;
    } else if (moisture_level == MoistureLevel::Damp) {
        growth += config.nutrient_growth.damp_moisture_modifier;
    } else if (moisture_level == MoistureLevel::Dry) {
        growth += config.nutrient_growth.dry_moisture_modifier;
    }
    return growth;
}

int World::CalculateSunlightGrowthModifier(const Tile& tile) {
    int growth = 0;
    SunlightLevel sunlight_level = tile.GetSunlightLevel();

    if (sunlight_level == SunlightLevel::Bright) {
        growth += config.nutrient_growth.bright_sunlight_modifier;
    } else if (sunlight_level == SunlightLevel::Moderate) {
        growth += config.nutrient_growth.moderate_sunlight_modifier;
    } else if (sunlight_level == SunlightLevel::Low) {
        growth += config.nutrient_growth.low_sunlight_modifier;
    } else if (sunlight_level == SunlightLevel::Dark) {
        growth += config.nutrient_growth.dark_sunlight_modifier;
    }
    return growth;
}

int World::CalculateNutrientGrowth(const Tile& tile) {
    int growth = 0;

    growth += CalculateFertilityGrowthModifier(tile);
    growth += CalculateMoistureGrowthModifier(tile);
    growth += CalculateSunlightGrowthModifier(tile);

    return growth;
}

// HandleNutrientGrowth
void World::ManageNutrientClusters() {
    ForEachTile([this](Tile& tile) {
        if (tile.HasNutrientCluster()) {
            return;
        }
        tile.AdjustNutrientGrowthProgress(CalculateNutrientGrowth(tile));

        if (tile.CanGrowNutrient() && tile.IsEmpty()) {
            PlaceNutrientCluster(tile);
            tile.ResetNutrientGrowthProgress();
        }
    });
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
    ForEachTile([this](Tile& tile) {
        int evaporation = 0;

        if (tile.GetSunlight() >= config.sunlight.high_evaporation_threshold) {
            evaporation += config.sunlight.high_evaporation_modifier;
        } else if (tile.GetSunlight() >=
                   config.sunlight.moderate_evaporation_threshold) {
            evaporation += config.sunlight.moderate_evaporation_modifier;
        } else if (tile.GetSunlight() >=
                   config.sunlight.low_evaporation_threshold) {
            if (day % config.sunlight.low_evaporation_interal == 0) {
                evaporation += config.sunlight.low_evaporation_modifier;
            };
        }
        tile.AdjustMoisture(-evaporation);
    });
}

void World::UpdateTileFertility(Tile& tile) {
    int fertility_change = 0;

    if (tile.HasNutrientCluster()) {
        fertility_change += config.fertility.nutrient_depletion_modifier;
    }

    MoistureLevel moisture_level = tile.GetMoistureLevel();

    if (moisture_level == MoistureLevel::Dry) {
        fertility_change += config.fertility.dry_moisture_modifier;
    } else if (moisture_level == MoistureLevel::Saturated) {
        fertility_change += config.fertility.saturated_moisture_modifier;
    } else if (moisture_level == MoistureLevel::Ideal) {
        fertility_change += config.fertility.ideal_moisture_modifier;
    }
    tile.AdjustFertility(fertility_change);
}

void World::UpdateFertility() {
    ForEachTile([this](Tile& tile) { UpdateTileFertility(tile); });
}

void World::IntializeMoisture() {
    PlaceMoistureSources(config.moisture.morning_dew_initial_amount,
                         config.moisture.source_count,
                         config.moisture.dew_spread_distance);
}

void World::InitializeTileFertility() {
    ForEachTile([this](Tile& tile) {
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
    });
}

void World::InitializeSunlight() {
    auto zones = GetLinearZones(config.sunlight.source_count);

    int amount = config.sunlight.initial_intensity;
    for (const auto& zone : zones) {
        Tile* tile = SelectRandomTile(zone.first, zone.second);
        tile->SetSunlight(amount);
        PlaceSunlightSpread(tile, amount - 1, config.sunlight.spread_distance);
    }
}

void World::InitializeEnvironment() {
    InitializeSunlight();
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
    UpdateEnvironment();
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

void World::PrintView() {
    switch (view_mode) {
        case ViewMode::Inspector:
            // * Change lambda here as needed for development:
            PrintTileView([](Tile& tile) { return tile.GetFertility(); });
            break;
        case ViewMode::World:
            PrintTileView([](Tile& tile) { return tile.GetSymbol(); });
            break;
        case ViewMode::Moisture:
            PrintTileView([](Tile& tile) { return tile.GetMoisture(); });
            break;
        case ViewMode::Sunlight:
            PrintTileView([](Tile& tile) { return tile.GetSunlight(); });
    }
}

void World::Observe() {
    PrintView();
    PrintStatusBar();
    PrintCreatureBar();
    PrintObserverMenu();
}

template <typename Callable>
void World::PrintTileView(Callable callable) {
    bool overlay_creatures = view_mode != ViewMode::World;

    cout << "\n" << right;
    for (int row = 0; row < height; row++) {
        cout << string(config.left_margin, ' ');
        for (int column = 0; column < width; column++) {
            Tile& tile = tiles[row][column];
            auto tile_value = callable(tile);

            if (overlay_creatures && tile.HasCreature()) {
                string cell = "[";
                cell += to_string(tile_value);
                cell += "]";
                cout << setw(4) << cell;
                continue;
            }
            cout << setw(4) << tile_value;
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

string World::PreferenceBar(Creature& creature) {
    Tile& tile = *creature.GetCurrentTile();

    return "   ≈ " + to_string(tile.GetMoisture()) + " / " +
           to_string(creature.GetIdealMoisture()) + "   ☀ " +
           to_string(tile.GetSunlight()) + " / " +
           to_string(creature.GetIdealSunlight());
}

void World::PrintObserverMenu() const {
    cout << "\n";
    cout << "A new day is unfolding.\n\n";
    cout << "Observe ➜ Enter   Leave ➜ 'exit'\n";
    cout << "Change View ➜ 1: ⌂ 2: ≈  3: ☀" << endl;
    cout << "❯ ";
}

void World::PrintStatusBar() const {
    cout << "\n";
    cout << " Day: " << left << setw(5) << day << " Mosslings: " << setw(5)
         << GetEntityCount(CREATURE) << " Nutrient Clusters: " << setw(5)
         << GetEntityCount(NUTRIENT_CLUSTER)
         << "Viewing: " << ModeToString(view_mode) << "\n";
    cout << "\n";
}

void World::PrintCreatureBar() {
    cout << "\n";
    cout << "   - Mosslings -          - Current / Ideal -   \n";
    for (auto& creature : creatures) {
        cout << left << setw(11) << creature->GetTrait()
             << EnergyBar(creature->GetEnergy(), creature->GetMaxEnergy())
             << PreferenceBar(*creature) << endl;
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