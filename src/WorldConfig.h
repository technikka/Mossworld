#ifndef WORLD_CONFIG_H
#define WORLD_CONFIG_H

struct WorldConfig {
    int width = 9;
    int height = 9;

    int nutrient_cluster_start_count = 6;
    int nutrient_spawn_interval = 5;

    int creature_start_count = 3;

    // int max_moisture = 10;
    // int moisture_sources = 3;
    // int moisture_spread_distance = 3;
};

#endif