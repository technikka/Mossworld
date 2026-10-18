#pragma once

#include <json.hpp>
#include "WorldSnapshot.h"

using json = nlohmann::json;

// Position
inline void to_json(json& json_data, const Position& position) {
    json_data = {{"x", position.x}, {"y", position.y}};
}

// Tile
inline void to_json(json& json_data, const TileSnapshot& tile_snapshot) {
    json_data = {
        {"position", tile_snapshot.position},
        {"moisture", tile_snapshot.moisture},
        {"fertility", tile_snapshot.fertility},
        {"base_sunlight", tile_snapshot.base_sunlight},
        {"effective_sunlight", tile_snapshot.effective_sunlight},
        {"nutrient_growth_progress", tile_snapshot.nutrient_growth_progress},
        {"elevation", tile_snapshot.elevation},
        {"canopy_cover", tile_snapshot.canopy_cover},
        {"moss_cover", tile_snapshot.moss_cover}};
}

// Nutrient Cluster
inline void to_json(json& json_data,
                    const NutrientClusterSnapshot& nutrient_cluster_snapshot) {
    json_data = {{"position", nutrient_cluster_snapshot.position},
                 {"stress", nutrient_cluster_snapshot.stress}};
}

// Stone
inline void to_json(json& json_data, const StoneSnapshot& stone_snapshot) {
    json_data = {{"position", stone_snapshot.position}};
}

// * add creature types here
inline void to_json(json& json_data, const CreatureType& creature_type) {
    switch (creature_type) {
        case MOSSLING:
            json_data = "Mossling";
            break;

        default:
            json_data = "Unknown";
            break;
    }
}

// Creature
inline void to_json(json& json_data,
                    const CreatureSnapshot& creature_snapshot) {
    json_data = {{"id", creature_snapshot.id},
                 {"type", creature_snapshot.type},
                 {"trait", creature_snapshot.trait},
                 {"position", creature_snapshot.position},
                 {"energy", creature_snapshot.energy},
                 {"ideal_moisture", creature_snapshot.ideal_moisture},
                 {"ideal_sunlight", creature_snapshot.ideal_sunlight},
                 {"objective_position", creature_snapshot.objective_position}};
}

// World
inline void to_json(json& json_data, const WorldSnapshot& world_snapshot) {
    json_data = {{"day", world_snapshot.day},
                 {"creatures", world_snapshot.creatures},
                 {"nutrient_clusters", world_snapshot.nutrient_clusters},
                 {"stones", world_snapshot.stones},
                 {"tiles", world_snapshot.tiles}};
}
