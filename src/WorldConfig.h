#ifndef WORLD_CONFIG_H
#define WORLD_CONFIG_H

#include "ViewMode.h"

struct WorldConfig {
    // * World
    ViewMode view_mode = ViewMode::World;
    int left_margin = 4;
    int width = 15;
    int height = 15;

    struct Creature {
        int max_energy = 10;
        int start_count = 6;
        int initial_energy = 10;
    };

    struct NutrientCluster {
        int start_count = 12;

        int stress_min = 0;
        int stress_max = 10;
        int initial_stress = 0;
        int stress_recovery = -2;
        int sunlight_stress_modifier = 1;
        int moisture_stress_modifier = 1;
        int fertility_stress_modifier = 1;
    };

    struct Stone {
        int start_count = 12;
        int shade = 1;
        int shade_spread_distance = 1;
    };

    struct NutrientGrowth {
        int progress_floor = 0;
        int progress_ceiling = 40;

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
        double min = 0;
        double max = 10;
        double initial_low = 3;
        double initial_high = 9;
        double dry_moisture_modifier = -1;
        double saturated_moisture_modifier = -1;
        double ideal_moisture_modifier = 1;
        double nutrient_depletion_modifier = -1;
    };

    struct Moisture {
        double min = 0;
        double max = 10;
        int source_count = 6;
        double morning_dew_initial_amount = 3;
        double morning_dew_amount = 3;
        int dew_spread_distance = 3;
        double dew_initial_retention = 0.7;  // First ring at 70%
        double dew_retention_decay = 0.5;
    };

    struct Sunlight {
        double min = 0.0;
        double max = 10.0;

        double high_evaporation_threshold = 8;
        double moderate_evaporation_threshold = 5;
        double low_evaporation_threshold = 3;

        double high_evaporation_modifier = 2;
        double moderate_evaporation_modifier = 1;
        double low_evaporation_modifier = 1;
        int low_evaporation_interval = 2;  // every n days apply modifer
    };

    Creature creature;
    NutrientCluster nutrient_cluster;
    Stone stone;
    Moisture moisture;
    Fertility fertility;
    Sunlight sunlight;
    NutrientGrowth nutrient_growth;
};

#endif