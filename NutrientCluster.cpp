#include <iostream>
using namespace std;

#include "NutrientCluster.h"
#include "Position.h"

NutrientCluster::NutrientCluster(EntityType type, Position position) {
    this->type = type;
    this->position = position;
}