#ifndef NARRATION_H
#define NARRATION_H

#include <string>

#include "Creature.h"

namespace Narration {

std::string NutrientFound(NutrientNeed need, CreatureType type,
                          const std::string& trait);

void SetEnabled(bool value);
bool IsEnabled();

}  // namespace Narration

#endif