#include <algorithm>
#include <iostream>
using namespace std;

#include "NutrientCluster.h"
#include "Position.h"
#include "Tile.h"
#include "WorldConfig.h"

NutrientCluster::NutrientCluster(Tile* current_tile, const WorldConfig& config)
    : Entity(NUTRIENT_CLUSTER, current_tile), config(config) {
    stress = config.nutrient_cluster.initial_stress;
    symbol = '~';
}

int NutrientCluster::GetStress() { return stress; }

void NutrientCluster::AdjustStress(int adjustment) {
    stress = clamp(stress + adjustment, config.nutrient_cluster.stress_min,
                   config.nutrient_cluster.stress_max);
}
