#include <iostream>
using namespace std;

#include "Creature.h"

Creature::Creature(CreatureType type) { this->type = type; }

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

string Creature::GetTypeString() { return CreatureTypeToString(this->type); }
char Creature::GetTypeSymbol() { return CreatureTypeToSymbol(this->type); }