#ifndef NARRATION_H
#define NARRATION_H

#include <string>

#include "Creature.h"

namespace Narration {

enum class Priority { Low, Medium, High, Critical };

struct Event {
    int day;
    Priority priority;
    std::string text;
};

extern std::vector<Narration::Event> current_narration_events;
extern std::vector<Narration::Event> narration_history;

void AddEvent(Event event);
std::vector<Event*> GetCriticalEvents();

// * Events
Event NutrientFound(Creature& creature, int day);
Event FrequentVisitor(Creature& creature, int day, std::string description);

std::string GetNutrientFoundText(Creature& creature);
std::string FrequentVisitorText(Creature& creature, std::string description);

void SetEnabled(bool value);
bool IsEnabled();

constexpr Priority nutrient_found_priority = Priority::Low;
constexpr Priority frequent_visitor_priority = Priority::Low;

}  // namespace Narration

#endif