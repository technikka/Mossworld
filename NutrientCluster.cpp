#include <iostream>
using namespace std;

#include "NutrientCluster.h"
#include "Position.h"
#include "Tile.h"

NutrientCluster::NutrientCluster(EntityType type, Tile* current_tile)
    : Entity(NUTRIENT_CLUSTER, current_tile) {
    this->type = type;
    this->current_tile = current_tile;
}
