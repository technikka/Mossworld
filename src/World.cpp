#include <algorithm>
#include <iomanip>  // for printing
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
using namespace std;

#include "Creature.h"
#include "Narration.h"
#include "Position.h"
#include "Stone.h"
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
    stones.reserve(config.stone.start_count);

    this->createTiles();
    this->InitializeStone();
    this->InitializeEnvironment();
    this->InitializeCreatures();
    this->InitializeNutrientClusters();
}

int World::GetWidth() { return width; }
int World::GetHeight() { return height; }

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
            NutrientCluster nutrients(tile, config);
            nutrient_clusters.push_back(
                (make_unique<NutrientCluster>(nutrients)));
            return nutrient_clusters.back().get();
        }
        case STONE: {
            Stone stone(tile);
            stones.push_back((make_unique<Stone>(stone)));
            return stones.back().get();
        }
    }
    return 0;
}

void World::RemoveEntity(Tile* tile) {
    if (tile->IsEmpty()) {
        return;
    }
    switch (tile->GetOccupant()->GetType()) {
        case CREATURE: {
            // place holder
            break;
        }
        case NUTRIENT_CLUSTER: {
            auto* nutrient_cluster =
                static_cast<NutrientCluster*>(tile->GetOccupant());
            erase_if(nutrient_clusters,
                     [nutrient_cluster](const auto& cluster) {
                         return cluster.get() == nutrient_cluster;
                     });
            break;
        }
        case STONE: {
            // place holder
            break;
        }
    }

    tile->RemoveOccupant();
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
    if (count == 0) {
        return;
    }
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
    if (count == 0) {
        return;
    }
    for (int i = 0; i < count; i++) {
        Tile* tile = SelectRandomNutrientGrowthTile();
        if (tile == nullptr) {
            return;
        }

        PlaceEntity(NUTRIENT_CLUSTER, *tile);
    }
}

void World::InitializeStone() {
    int stone_count = config.stone.start_count;
    if (stone_count == 0) {
        return;
    }
    // Distribute stones to encourage natural clustering while maintaining world coverage.
    int zone_count = static_cast<int>(round(sqrt(stone_count)));
    auto zones = GetLinearZones(zone_count);

    for (int zone_index = 0; zone_index < zones.size(); zone_index++) {
        const auto& zone = zones[zone_index];
        int stones_for_zone = stone_count / zone_count;
        if (zone_index < stone_count % zone_count) {
            stones_for_zone++;
        }
        for (int i = 0; i < stones_for_zone; i++) {
            Tile* tile = SelectRandomTile(zone.first, zone.second);
            if (tile == nullptr) {
                return;
            }
            PlaceEntity(STONE, *tile);
        }
    }
}

void World::PlaceNutrientCluster(Tile& tile) {
    PlaceEntity(NUTRIENT_CLUSTER, tile);
}

template <typename Callable>
void World::ForEachTileInRing(Tile* tile, int distance, Callable callable) {
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
            callable(tiles[new_y][new_x]);
        }
    }
}

void World::PlaceMoistureSpread(Tile* tile, int amount, int spread_distance) {
    double percent = config.moisture.dew_initial_retention;
    for (int i = 1; i <= spread_distance; i++) {
        amount *= percent;
        ForEachTileInRing(tile, i, [amount](Tile& ring_tile) {
            ring_tile.AdjustMoisture(amount);
        });
        percent *= config.moisture.dew_retention_decay;
    }
}

void World::PlaceSunlightSpread(Tile* tile, int amount, int spread_distance) {
    for (int i = 1; i <= spread_distance; i++) {
        ForEachTileInRing(tile, i, [amount](Tile& ring_tile) {
            if (amount > ring_tile.GetBaseSunlight()) {
                ring_tile.SetBaseSunlight(amount);
                ring_tile.SetEffectiveSunlight(amount);
            }
        });
        amount -= config.sunlight.spread_falloff;
    }
}

void World::PlaceShadeSpread(Tile* tile, int amount, int spread_distance) {
    for (int i = 1; i <= spread_distance; i++) {
        ForEachTileInRing(tile, i, [amount](Tile& ring_tile) {
            int shade = ring_tile.GetBaseSunlight() - amount;

            if (shade < ring_tile.GetEffectiveSunlight()) {
                ring_tile.SetEffectiveSunlight(shade);
            }
        });
        amount -= config.stone.shade_spread_falloff;
    }
}

void World::PlaceMoistureSource(int amount, int start_id, int end_id,
                                int spread_distance) {
    Tile* tile = SelectRandomTile(start_id, end_id);
    tile->AdjustMoisture(amount);
    PlaceMoistureSpread(tile, amount, spread_distance);
}

void World::PlaceMoistureSources(int initial_amount, int sources,
                                 int spread_distance) {
    auto zones = GetLinearZones(sources);

    for (const auto& zone : zones) {
        PlaceMoistureSource(initial_amount, zone.first, zone.second,
                            spread_distance);
    }
}

vector<Tile*> World::GetAdjacentTiles(const Tile& tile) {
    vector<Tile*> adjacent_tiles;
    vector<Position> possible_positions;
    Position position = tile.GetPosition();

    int x = position.x;
    int y = position.y;

    possible_positions.push_back({x - 1, y});  // left
    possible_positions.push_back({x + 1, y});  // right
    possible_positions.push_back({x, y - 1});  // up
    possible_positions.push_back({x, y + 1});  // down

    for (const Position& position : possible_positions) {
        if (position.x < 0 || position.x >= width) continue;
        if (position.y < 0 || position.y >= height) continue;

        Tile* tile = &tiles[position.y][position.x];
        adjacent_tiles.push_back(tile);
    }
    return adjacent_tiles;
}

// For Creature; NutrientCluster doesn't block openness.
vector<Tile*> World::GetAdjacentOpenTiles(Tile* current_tile) {
    vector<Tile*> adjacent_tiles = GetAdjacentTiles(*current_tile);
    vector<Tile*> valid_tiles;

    for (Tile* tile : adjacent_tiles) {
        if (tile->HasNutrientCluster() || tile->IsEmpty()) {
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

// Creature movement helpers.
int World::ScoreMoisture(Tile& tile, Creature& creature) {
    int moisture_difference =
        abs(tile.GetMoisture() - creature.GetIdealMoisture());

    return 10 - moisture_difference;
}

int World::ScoreSunlight(Tile& tile, Creature& creature) {
    int sunlight_difference =
        abs(tile.GetEffectiveSunlight() - creature.GetIdealSunlight());

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
    if (Narration::IsEnabled()) {
        Narration::AddEvent(Narration::NutrientFound(creature, day));
    }

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

int World::CalculateEffectiveSunlight(Tile& tile) {
    int sunlight = tile.GetBaseSunlight();
    sunlight -= config.stone.shade;
    return sunlight;
}

void World::UpdateSunlight() {
    ForEachTile([this](Tile& tile) {
        if (tile.HasStone()) {
            int sunlight = CalculateEffectiveSunlight(tile);
            tile.SetEffectiveSunlight(sunlight);
            PlaceShadeSpread(&tile,
                             sunlight - config.stone.shade_spread_falloff,
                             config.stone.shade_spread_distance);
        }
    });
}

int World::CalculateSunlightGrowthModifier(const Tile& tile) {
    int growth = 0;
    SunlightLevel sunlight_level = tile.GetEffectiveSunlightLevel();

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

int World::CalculateNutrientStress(Tile& tile) {
    int stress = 0;
    int sunlight_stress = config.nutrient_cluster.sunlight_stress_modifier;
    int moisture_stress = config.nutrient_cluster.moisture_stress_modifier;
    int fertility_stress = config.nutrient_cluster.fertility_stress_modifier;

    int sunlight_modifier = CalculateSunlightGrowthModifier(tile);
    int moisture_modifier = CalculateMoistureGrowthModifier(tile);

    if (sunlight_modifier < 0) {
        stress += sunlight_stress;
    }

    if (moisture_modifier < 0) {
        stress += moisture_stress;
    }

    if (tile.GetFertilityLevel() == FertilityLevel::None) {
        stress += fertility_stress;
    }

    return stress;
}

void World::UpdateNutrientClusterStress(NutrientCluster& nutrient_cluster) {
    Tile& tile = *nutrient_cluster.GetCurrentTile();
    int daily_stress = CalculateNutrientStress(tile);

    if (daily_stress > 0) {
        nutrient_cluster.AdjustStress(daily_stress);
    } else {
        nutrient_cluster.AdjustStress(config.nutrient_cluster.stress_recovery);
    }
}

// Handle growth and condition.
void World::ManageNutrientClusters() {
    ForEachTile([this](Tile& tile) {
        if (tile.HasNutrientCluster()) {
            // Safe after HasNutrientCluster(): GetOccupant() returns Entity*.
            auto* nutrient_cluster =
                static_cast<NutrientCluster*>(tile.GetOccupant());
            UpdateNutrientClusterStress(*nutrient_cluster);
            if (nutrient_cluster->GetStress() >=
                config.nutrient_cluster.stress_max) {
                RemoveEntity(&tile);
                return;
            }
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

        int sunlight = tile.GetEffectiveSunlight();
        if (sunlight >= config.sunlight.high_evaporation_threshold) {
            evaporation += config.sunlight.high_evaporation_modifier;
        } else if (sunlight >= config.sunlight.moderate_evaporation_threshold) {
            evaporation += config.sunlight.moderate_evaporation_modifier;
        } else if (sunlight >= config.sunlight.low_evaporation_threshold) {
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
        tile->SetBaseSunlight(amount);
        tile->SetEffectiveSunlight(amount);
        PlaceSunlightSpread(tile, amount - config.sunlight.spread_falloff,
                            config.sunlight.spread_distance);
    }
}

void World::InitializeEnvironment() {
    InitializeSunlight();
    UpdateSunlight();
    IntializeMoisture();
    InitializeTileFertility();
}

void World::UpdateEnvironment() {
    UpdateSunlight();
    ApplyMorningDew();
    ApplyEvaporation();
    UpdateFertility();
}

void World::BeginDay() {
    day++;
    Narration::current_narration_events.clear();
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

void World::UpdateStoneMemory() {
    for (const auto& stone : stones) {
        vector<Tile*> adjacent_tiles =
            GetAdjacentTiles(*stone->GetCurrentTile());
        for (Tile* tile : adjacent_tiles) {
            if (tile->HasCreature()) {
                auto* creature = static_cast<Creature*>(tile->GetOccupant());
                int creature_id = creature->GetId();
                stone->RecordVisit(creature_id, day);

                // Create FrequentVisitor Event
                int frequent_visitor_count = 5;
                if (stone->GetVisitCount(creature_id) ==
                    frequent_visitor_count) {
                    Narration::AddEvent(Narration::FrequentVisitor(
                        *creature, day, DescribeStone(*stone)));
                }
            }
        }
    }
}

void World::UpdateMemory() { UpdateStoneMemory(); }

void World::SetViewMode(ViewMode mode) { view_mode = mode; }

void World::PrintView() {
    switch (view_mode) {
        case ViewMode::Inspector:
            // * Change lambda here as needed for development:
            PrintTileView([](Tile& tile) {
                if (!tile.HasNutrientCluster()) {
                    return 0;
                }

                auto* nutrient_cluster =
                    static_cast<NutrientCluster*>(tile.GetOccupant());

                return nutrient_cluster->GetStress();
            });
            break;
        case ViewMode::World:
            PrintTileView([](Tile& tile) { return tile.GetSymbol(); });
            break;
        case ViewMode::Moisture:
            PrintTileView([](Tile& tile) { return tile.GetMoisture(); });
            break;
        case ViewMode::Sunlight:
            PrintTileView(
                [](Tile& tile) { return tile.GetEffectiveSunlight(); });
    }
}

void World::Observe() {
    if (day > 1) {
        PrintDivider();
    }
    PrintEventNotification();
    PrintView();
    PrintStatusBar();
    PrintCreatureBar();
    PrintObserverMenu();
    if (IsJournalOpen()) {
        PrintJournal();
    }
}

void World::PrintDivider() const {
    cout << "\n";
    cout << string(config.left_margin, ' ')
         << "──────────────────────────────────────────────────────────────────"
            "──"  // 68 chars
         << "\n\n";
}

void World::PrintLeftMargin() const { cout << string(config.left_margin, ' '); }

void World::PrintLine(const string& text) const {
    PrintLeftMargin();
    cout << text << '\n';
}

void World::PrintCentered(const string& text) const {
    PrintLeftMargin();
    cout << right << setw((68 + text.length()) / 2) << text << "\n\n";
}

void World::PrintEventNotification() const {
    int sig_events = 0;
    for (int i = 0; i < Narration::current_narration_events.size(); i++) {
        const auto& event = Narration::current_narration_events.at(i);

        if (event.priority == Narration::Priority::Critical ||
            event.priority == Narration::Priority::High) {
            sig_events++;
        }
    }
    if (sig_events > 0) {
        cout << "\n";
        if (sig_events == 1) {
            PrintLine(to_string(sig_events) +
                      " significant event occured today. Details in journal.");

        } else {
            PrintLine(to_string(sig_events) +
                      " significant events occured today. Details in journal.");
        }
    }
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
                ostringstream stream;
                stream << "[" << tile_value << "]";
                cout << setw(4) << stream.str();
                continue;
            }

            if (view_mode == ViewMode::World) {
                // setw() doesn't play with unicode symbols.
                cout << "   " << tile_value;
            } else {
                cout << setw(4) << tile_value;
            }
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
           to_string(tile.GetEffectiveSunlight()) + " / " +
           to_string(creature.GetIdealSunlight());
}

void World::PrintObserverMenu() const {
    cout << "\n\n";
    // cout << "A new day is unfolding.\n\n";
    PrintLine("Observe ➜ Enter   Leave ➜ 'exit'");
    PrintLine("Change View ➜ 1: ⌂ 2: ≈  3: ☀");
    PrintLine("Open/Close Journal ➜ 'J'");
    cout << "\n";
    PrintLeftMargin();
    cout << "❯ " << flush;
}

void World::PrintWrapped(const string& text, size_t width,
                         size_t indent) const {
    string indentation(indent, ' ');
    istringstream stream(text);
    string word;
    size_t line_length = 0;

    cout << indentation;

    while (stream >> word) {
        if (line_length == 0) {
            std::cout << word;
            line_length = word.length();
        } else if (line_length + 1 + word.length() <= width) {
            std::cout << ' ' << word;
            line_length += 1 + word.length();
        } else {
            std::cout << '\n' << indentation << word;
            line_length = word.length();
        }
    }

    std::cout << '\n';
}

void World::PrintJournal() const {
    cout << "\n";
    PrintDivider();
    PrintCentered("=== Journal ===");
    if (Narration::narration_history.empty()) {
        PrintCentered("No entries.");
    } else {
        int last_printed_day = -1;
        for (const Narration::Event& event : Narration::narration_history) {
            if (event.day != last_printed_day) {
                PrintCentered("⬢ Day " + to_string(event.day));
                last_printed_day = event.day;
            }
            PrintWrapped(event.text, 64, config.left_margin);
            cout << "\n";
        }
    }
    PrintDivider();
    PrintLeftMargin();
    cout << "❯ " << flush;
}

void World::PrintStatusBar() const {
    cout << "\n";
    PrintLeftMargin();
    cout << " Day: " << left << setw(5) << day << " Mosslings: " << setw(5)
         << creatures.size() << " Nutrient Clusters: " << setw(5)
         << nutrient_clusters.size() << "Viewing: " << ModeToString(view_mode)
         << "\n\n";
}

void World::PrintCreatureBar() {
    cout << "\n";
    PrintLine("   - Mosslings -          - Current / Ideal -   ");
    for (auto& creature : creatures) {
        PrintLeftMargin();
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

void World::ToggleJournal() { journal_open = !journal_open; }

bool World::IsJournalOpen() const { return journal_open; }

string World::DescribeStone(Stone& stone) {
    string text = "The ";
    Position position = stone.GetPosition();

    if (position.y < height / 2)
        text += "upper-";
    else
        text += "lower-";

    if (position.x < width / 2)
        text += "western ";
    else
        text += "eastern ";

    MoistureLevel moisture = stone.GetCurrentTile()->GetMoistureLevel();
    SunlightLevel sunlight =
        stone.GetCurrentTile()->GetEffectiveSunlightLevel();

    if (sunlight == SunlightLevel::Moderate) {
        text.append("sunlit");
    } else if (sunlight == SunlightLevel::Low) {
        text.append("shaded");
    } else if (!(moisture == MoistureLevel::Saturated) ||
               !(moisture == MoistureLevel::Ideal)) {
        text.append(ToString(moisture));
    }

    text.append(" stone");
    return text;
}