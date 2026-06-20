#include "Narration.h"

using namespace std;

namespace Narration {

string NutrientFound(int energy, const string& trait) {
    if (energy <= 2) {
        return "The " + trait +
               " Mossling uncovers a nutrient cluster not a moment too soon.";
    }

    if (energy <= 6) {
        return "After a long search, the " + trait +
               " Mossling discovers a welcome nutrient cluster.";
    }

    return "The " + trait +
           " Mossling happens upon a nutrient cluster nestled deep among the "
           "moss.";
}

}