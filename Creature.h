#include <string>

#include "Position.h"
using namespace std;
#ifndef CREATURE_H
#define CREATURE_H

// * add creature types here
enum CreatureType { MOSSLING };

class Creature {
   public:
    Creature(CreatureType type, int id, Position position);
    string GetTypeString();
    char GetTypeSymbol();
    int GetId();
    int id;
    Position position;

   private:
    CreatureType type;
};

#endif