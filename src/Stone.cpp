#include "Stone.h"
using namespace std;

Stone::Stone(Tile* current_tile) : Entity(STONE, current_tile) { symbol = "⬢"; }

void Stone::RecordVisit(CreatureId id, int current_day) {
    auto& record = visitor_history[id];

    if (record.visit_count == 0) {
        record.first_visit_day = current_day;
    }

    ++record.visit_count;
    record.last_visit_day = current_day;
}

int Stone::GetVisitCount(CreatureId creature_id) {
    auto iter = visitor_history.find(creature_id);

    if (iter != visitor_history.end()) {
        return iter->second.visit_count;
    }

    return 0;
}