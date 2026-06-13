#include <vector>

#include "EntityType.h"
#include "Position.h"
using namespace std;

#ifndef NUTRIENTCLUSTER_H
#define NUTRIENTCLUSTER_H

class NutrientCluster {
   public:
    NutrientCluster(EntityType type, Position position);
    Position GetPosition() const;

   private:
    EntityType type;
    Position position;
};

#endif