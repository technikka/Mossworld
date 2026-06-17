#include <vector>

#include "Entity.h"
#include "EntityType.h"
#include "Position.h"
#include "Tile.h"
using namespace std;

#ifndef NUTRIENTCLUSTER_H
#define NUTRIENTCLUSTER_H

class NutrientCluster : public Entity {
   public:
    NutrientCluster(EntityType type, Tile* current_tile);

   private:
    EntityType type;
};

#endif