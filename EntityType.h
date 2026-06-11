using namespace std;
#ifndef ENTITY_TYPE_H
#define ENTITY_TYPE_H

enum EntityType { EMPTY, CREATURE, NUTRIENT_CLUSTER };

// 'inline' fixes compiler error when multiple files recieve this definition
inline char EntityTypeToChar(EntityType type) {
    switch (type) {
        case EMPTY:
            return '.';

        case CREATURE:
            return 'k';

        case NUTRIENT_CLUSTER:
            return '~';
    }
    return '?';  // should never be reached
}

#endif