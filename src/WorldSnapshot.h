#pragma once

#include <optional>
#include <vector>
#include "Creature.h"

struct CreatureSnapshot {
    int id;
    CreatureType type;
    std::string trait;
    Position position;
    int energy;
    double ideal_moisture;
    double ideal_sunlight;
    std::optional<Position> objective_position;
};

struct NutrientClusterSnapshot {
    Position position;
    int stress;
};

struct StoneSnapshot {
    Position position;
};

struct TileSnapshot {
    Position position;
    double moisture;
    double fertility;
    double base_sunlight;
    double effective_sunlight;
    int nutrient_growth_progress;
    double elevation;
    double canopy_cover;
    double moss_cover;
};

struct WorldSnapshot {
    std::vector<CreatureSnapshot> creatures;
    std::vector<NutrientClusterSnapshot> nutrient_clusters;
    std::vector<StoneSnapshot> stones;
    std::vector<TileSnapshot> tiles;
};
