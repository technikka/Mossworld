#include <iostream>
using namespace std;

#include "NutrientCluster.h"
#include "Position.h"
#include "Tile.h"

NutrientCluster::NutrientCluster(EntityType type, Tile* current_tile) {
    this->type = type;
    this->current_tile = current_tile;
}

Tile* NutrientCluster::GetCurrentTile() const { return current_tile; }

void NutrientCluster::SetCurrentTile(Tile* tile) { current_tile = tile; }