#include <vector>

#include "Entity.h"
#include "EntityType.h"
#include "Position.h"
#include "Tile.h"

#ifndef NUTRIENTCLUSTER_H
#define NUTRIENTCLUSTER_H

class NutrientCluster : public Entity {
   public:
    NutrientCluster(Tile* current_tile);
};

#endif