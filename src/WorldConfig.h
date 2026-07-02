#ifndef WORLD_CONFIG_H
#define WORLD_CONFIG_H

#include "ViewMode.h"

struct WorldConfig {
    // * World
    ViewMode view_mode = ViewMode::World;
    int left_margin = 4;
    int width = 9;
    int height = 9;

    // * Nutrient Cluster
    int nutrient_cluster_start_count = 8;
    int nutrient_spawn_interval = 2;

    // * Creature
    int creature_start_count = 4;

    // * Moisture
    int max_moisture = 10;
    int moisture_sources = 4;
    int morning_dew_moisture = 7;
    int dew_spread_distance = 2;
};

#endif