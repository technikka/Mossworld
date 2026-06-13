#include <string>
#include <vector>

#include "Position.h"
using namespace std;
#ifndef CREATURE_H
#define CREATURE_H

// * add creature types here
enum CreatureType { MOSSLING };

class Creature {
   public:
    Creature(CreatureType type, int id, Position position, string trait);
    string GetTypeString();
    char GetTypeSymbol();
    int GetId();
    Position position;
    vector<Position> position_history;
    string GetTrait();
    int energy;
    int GetMaxEnergy();
    void LoseDailyEnergy();
    void RestoreEnergy();

   private:
    int id;
    CreatureType type;
    string trait;
    int max_energy;
};

#endif