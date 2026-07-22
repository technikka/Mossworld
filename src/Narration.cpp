#include "Narration.h"

#include "Creature.h"

using namespace std;

static bool enabled = true;

namespace Narration {

vector<Narration::Event> current_narration_events;
vector<Narration::Event> narration_history;

void SetEnabled(bool value) { enabled = value; }
bool IsEnabled() { return enabled; }

void AddEvent(Event event) {
    current_narration_events.push_back(event);
    narration_history.push_back(event);
}

vector<Event*> GetCriticalEvents() {
    vector<Event*> critical_events;
    for (Event& event : current_narration_events) {
        if (event.priority == Priority::Critical) {
            critical_events.push_back(&event);
        }
    }
    return critical_events;
}

string GetNutrientFoundText(Creature& creature) {
    NutrientNeed need = creature.GetNutrientNeed();
    string creature_type = CreatureTypeToString(creature.GetType());
    string trait = creature.GetTrait();

    if (need == NutrientNeed::High) {
        return "The " + trait + ' ' + creature_type +
               " uncovers a nutrient cluster not a moment too soon.\n";
    }

    if (need == NutrientNeed::Medium) {
        return "After a long search, the " + trait + ' ' + creature_type +
               " discovers a welcome nutrient cluster.\n";
    }

    return "The " + trait + ' ' + creature_type +
           " happens upon a nutrient cluster nestled deep among the "
           "moss.\n";
}

Event NutrientFound(Creature& creature, int day) {
    Event event;

    event.day = day;
    event.priority = nutrient_found_priority;
    event.text = GetNutrientFoundText(creature);

    return event;
}

}  // namespace Narration