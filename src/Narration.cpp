#include "Narration.h"
#include "Creature.h"

#include <cctype>
#include <string>

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
    trait[0] = static_cast<char>(tolower(static_cast<unsigned char>(trait[0])));

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

string FrequentVisitorText(Creature& creature, string description) {
    int option = rand() % 4;

    string trait = creature.GetTrait();
    if (!trait.empty()) {
        trait[0] =
            static_cast<char>(tolower(static_cast<unsigned char>(trait[0])));
    }

    string lower_description = description;
    if (!lower_description.empty()) {
        lower_description[0] = static_cast<char>(
            tolower(static_cast<unsigned char>(lower_description[0])));
    }

    string mossling = "the " + trait + " mossling";
    string sentence_start_mossling = "The " + trait + " mossling";

    switch (option) {
        case 0:
            return sentence_start_mossling + "'s path often leads past " +
                   lower_description + ".";

        case 1:
            return "Once again, " + mossling + " wanders past " +
                   lower_description + ".";

        case 2:
            return description + " has grown familiar with " + mossling +
                   "'s passing.";

        case 3:
            return description + " has quietly come to know " + mossling + ".";

        default:
            return sentence_start_mossling + "'s path often leads past " +
                   lower_description + ".";
    }
}

Event NutrientFound(Creature& creature, int day) {
    Event event;
    event.day = day;
    event.priority = nutrient_found_priority;
    event.text = GetNutrientFoundText(creature);

    return event;
}

Event FrequentVisitor(Creature& creature, int day, string description) {
    Event event;
    event.day = day;
    event.priority = frequent_visitor_priority;
    event.text = FrequentVisitorText(creature, description);

    return event;
}

}  // namespace Narration