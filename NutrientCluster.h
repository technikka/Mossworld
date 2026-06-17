#include <vector>

#include "EntityType.h"
#include "Position.h"
#include "Tile.h"
using namespace std;

#ifndef NUTRIENTCLUSTER_H
#define NUTRIENTCLUSTER_H

class NutrientCluster {
   public:
    NutrientCluster(EntityType type, Tile* current_tile);
    Tile* GetCurrentTile() const;
    void SetCurrentTile(Tile* tile);

   private:
    EntityType type;
    Tile* current_tile = nullptr;
};

#endif