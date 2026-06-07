#include <string>
using namespace std;
#ifndef CREATURE_H
#define CREATURE_H

// * add creature types here
enum CreatureType { MOSSLING };

class Creature {
 public:
  Creature(CreatureType type);
  string GetTypeString();
  char GetTypeSymbol();

 private:
  CreatureType type;
};

#endif