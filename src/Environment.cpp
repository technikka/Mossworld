#include <string>

#include "Environment.h"

using namespace std;

Environment::Environment(TileMap& tile_map, const WorldConfig& config,
                         const int& day)
    : tile_map(tile_map), config(config), day(day) {}

void Environment::Initialize() {
    InitializeSunlight();
    UpdateSunlight();
    IntializeMoisture();
    InitializeTileFertility();
}

void Environment::Update() {
    UpdateSunlight();
    ApplyMorningDew();
    ApplyEvaporation();
    UpdateFertility();
}

void Environment::PlaceMoistureSpread(Tile* tile, double amount,
                                      int spread_distance) {
    double percent = config.moisture.dew_initial_retention;
    for (int i = 1; i <= spread_distance; i++) {
        amount *= percent;
        tile_map.ForEachTileInRing(tile, i, [amount](Tile& ring_tile) {
            ring_tile.AdjustMoisture(amount);
        });
        percent *= config.moisture.dew_retention_decay;
    }
}

void Environment::PlaceShadeSpread(Tile* tile, double amount,
                                   int spread_distance) {
    for (int i = 1; i <= spread_distance; i++) {
        tile_map.ForEachTileInRing(tile, i, [amount](Tile& ring_tile) {
            double shade = ring_tile.GetBaseSunlight() - amount;

            if (shade < ring_tile.GetEffectiveSunlight()) {
                ring_tile.SetEffectiveSunlight(shade);
            }
        });
    }
}

void Environment::PlaceMoistureSource(double amount, int start_id, int end_id,
                                      int spread_distance) {
    Tile* tile = tile_map.SelectRandomTile(start_id, end_id);
    tile->AdjustMoisture(amount);
    PlaceMoistureSpread(tile, amount, spread_distance);
}

void Environment::PlaceMoistureSources(double initial_amount, int sources,
                                       int spread_distance) {
    auto zones = tile_map.GetLinearZones(sources);

    for (const auto& zone : zones) {
        PlaceMoistureSource(initial_amount, zone.first, zone.second,
                            spread_distance);
    }
}

void Environment::UpdateSunlight() {
    tile_map.ForEachTile([this](Tile& tile) {
        if (tile.HasStone()) {
            PlaceShadeSpread(&tile, config.stone.shade,
                             config.stone.shade_spread_distance);
        }
    });
}

void Environment::ApplyMorningDew() {
    // For now, apply morning dew to new random tiles.
    PlaceMoistureSources(config.moisture.morning_dew_amount,
                         config.moisture.source_count,
                         config.moisture.dew_spread_distance);
}

void Environment::ApplyEvaporation() {
    tile_map.ForEachTile([this](Tile& tile) {
        double evaporation = 0;

        double sunlight = tile.GetEffectiveSunlight();
        if (sunlight >= config.sunlight.high_evaporation_threshold) {
            evaporation += config.sunlight.high_evaporation_modifier;
        } else if (sunlight >= config.sunlight.moderate_evaporation_threshold) {
            evaporation += config.sunlight.moderate_evaporation_modifier;
        } else if (sunlight >= config.sunlight.low_evaporation_threshold) {
            if (day % static_cast<int>(
                          config.sunlight.low_evaporation_interval) ==
                0) {
                evaporation += config.sunlight.low_evaporation_modifier;
            };
        }
        tile.AdjustMoisture(-evaporation);
    });
}

void Environment::UpdateTileFertility(Tile& tile) {
    double fertility_change = 0;

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

void Environment::UpdateFertility() {
    tile_map.ForEachTile([this](Tile& tile) { UpdateTileFertility(tile); });
}

void Environment::IntializeMoisture() {
    PlaceMoistureSources(config.moisture.morning_dew_initial_amount,
                         config.moisture.source_count,
                         config.moisture.dew_spread_distance);
}

void Environment::InitializeTileFertility() {
    tile_map.ForEachTile([this](Tile& tile) {
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

void Environment::InitializeSunlight() {
    tile_map.ForEachTile([](Tile& tile) {
        double canopy_cover = tile.GetCanopyCover();
        double sunlight = (1.0 - canopy_cover) * 10;
        tile.SetBaseSunlight(sunlight);
        tile.SetEffectiveSunlight(sunlight);
    });
}