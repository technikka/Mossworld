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
    Position position;
    vector<Position> position_history;

   private:
    int id;
    CreatureType type;
};

#endif