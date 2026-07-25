#ifndef NUTRIENTCLUSTER_H
#define NUTRIENTCLUSTER_H

#include <vector>

#include "Entity.h"
#include "EntityType.h"
#include "Position.h"
#include "Tile.h"

class NutrientCluster : public Entity {
   public:
    NutrientCluster(Tile* current_tile, const WorldConfig& config);
    int GetStress();
    void AdjustStress(int adjustment);

   private:
    const WorldConfig& config;
    int stress;
};

#endif