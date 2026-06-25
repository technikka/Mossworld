#include "Narration.h"

#include "Creature.h"

using namespace std;

namespace Narration {

string NutrientFound(int energy, CreatureType type, const string& trait) {
    string creature_type = CreatureTypeToString(type);
    if (energy <= 2) {
        return "The " + trait + ' ' + creature_type +
               " uncovers a nutrient cluster not a moment too soon.";
    }

    if (energy <= 6) {
        return "After a long search, the " + trait + ' ' + creature_type +
               " discovers a welcome nutrient cluster.";
    }

    return "The " + trait + ' ' + creature_type +
           " happens upon a nutrient cluster nestled deep among the "
           "moss.";
}

}  // namespace Narration