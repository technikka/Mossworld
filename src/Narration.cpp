#include "Narration.h"

#include "Creature.h"

using namespace std;

static bool enabled = true;

namespace Narration {

void SetEnabled(bool value) { enabled = value; }
bool IsEnabled() { return enabled; }

string NutrientFound(NutrientNeed need, CreatureType type,
                     const string& trait) {
    string creature_type = CreatureTypeToString(type);
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

}  // namespace Narration