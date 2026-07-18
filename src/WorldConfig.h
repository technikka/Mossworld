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
    };

    struct NutrientGrowth {
        int high_fertility_modifier = 3;
        int moderate_fertility_modifier = 2;
        int low_fertility_modifier = 1;

        int saturated_moisture_modifier = -2;
        int wet_moisture_modifier = 0;
        int ideal_moisture_modifier = 1;
        int damp_moisture_modifier = 1;
        int dry_moisture_modifier = -2;

        int bright_sunlight_modifier = -1;
        int moderate_sunlight_modifier = 1;
        int low_sunlight_modifier = 1;
        int dark_sunlight_modifier = -2;
    };

    struct Fertility {
        int initial_low = 3;
        int initial_high = 9;

        int dry_moisture_modifier = -1;
        int saturated_moisture_modifier = -1;
        int ideal_moisture_modifier = 1;

        int nutrient_depletion_modifier = -1;
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

        int high_evaporation_modifier = 2;
        int moderate_evaporation_modifier = 1;
        int low_evaporation_modifier = 1;
        int low_evaporation_interal = 2;  // every n days apply modifer
    };

    NutrientCluster nutrient_cluster;
    Moisture moisture;
    Fertility fertility;
    Sunlight sunlight;
    NutrientGrowth nutrient_growth;

    int creature_start_count = 4;
};

#endif