#ifndef WORLD_CONFIG_H
#define WORLD_CONFIG_H

#include "ViewMode.h"

struct WorldConfig {
    // * World
    ViewMode view_mode = ViewMode::World;
    int left_margin = 4;
    int width = 9;
    int height = 9;

    struct NutrientCluster {
        int start_count = 8;
        // every [interval] days there is a chance to grow.
        int high_fertility_growth_interval = 2;
        int moderate_fertility_growth_interval = 3;
        int low_fertility_growth_interval = 4;
    };

    struct Fertility {
        int initial_low = 3;
        int initial_high = 9;
    };

    struct Moisture {
        int source_count = 3;
        int morning_dew_initial_amount = 5;
        int morning_dew_amount = 3;
        int dew_spread_distance = 3;
    };

    struct Sunlight {
        int initial_intensity = 6;
        int source_count = 3;
        int spread_distance = 4;
        int high_evaporation_threshold = 8;
        int moderate_evaporation_threshold = 5;
        int low_evaporation_threshold = 3;
    };

    NutrientCluster nutrient_cluster;
    Moisture moisture;
    Fertility fertility;
    Sunlight sunlight;

    // * Creature
    int creature_start_count = 4;
};

#endif