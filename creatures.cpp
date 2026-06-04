#include <iostream>
using namespace std;

#include "creatures.h"

Creature::Creature(string type) { this->type = type; }

// * add creature types here
enum CreatureType { MOSSLING };

char CreatureTypeToSymbol(CreatureType type) {
  switch (type) {
    case MOSSLING:
      return 'k';
  }
  return '?';  // should never be reached
}

string CreatureTypeToString(CreatureType type) {
  switch (type) {
    case MOSSLING:
      return "Mossling";
  }
  return "?";  // should never be reached
}