#ifndef STONE_H
#define STONE_H

#include <unordered_map>

#include "Entity.h"

using CreatureId = int;

class Stone : public Entity {
   public:
    Stone(Tile* current_tile);
    void RecordVisit(CreatureId id, int current_day);
    int GetVisitCount(CreatureId creature_id);

   private:
    struct VisitRecord {
        int visit_count = 0;
        int first_visit_day = -1;
        int last_visit_day = -1;
    };

    std::unordered_map<CreatureId, VisitRecord> visitor_history;
};

#endif