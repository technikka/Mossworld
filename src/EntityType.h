#ifndef ENTITY_TYPE_H
#define ENTITY_TYPE_H

#include <string>

enum EntityType { CREATURE, NUTRIENT_CLUSTER, STONE };

inline std::string EntityTypeToString(EntityType type) {
    switch (type) {
        case CREATURE:
            return "Creature";

        case NUTRIENT_CLUSTER:
            return "Nutrient Cluster";
        case STONE:
            return "Stone";
    }
    return "?";  // should never be reached
}

#endif