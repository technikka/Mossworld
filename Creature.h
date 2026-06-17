#ifndef CREATURE_H
#define CREATURE_H

#include <string>
#include <vector>

#include "Entity.h"
#include "Position.h"
#include "Tile.h"

using namespace std;

// * add creature types here
enum CreatureType { MOSSLING };

class Creature : public Entity {
   public:
    Creature(CreatureType type, int id, Tile* current_tile, string trait);
    string GetTypeString();
    char GetTypeSymbol();
    int GetId();
    vector<Tile*> tile_history;
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