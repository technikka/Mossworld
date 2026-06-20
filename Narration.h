#ifndef NARRATION_H
#define NARRATION_H

#include <string>

#include "Creature.h"

namespace Narration {

std::string NutrientFound(int energy, CreatureType type,
                          const std::string& trait);

}

#endif