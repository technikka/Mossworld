#include <string>
#include <vector>

#include "Position.h"
#include "Tile.h"
using namespace std;
#ifndef CREATURE_H
#define CREATURE_H

// * add creature types here
enum CreatureType { MOSSLING };

class Creature {
   public:
    Creature(CreatureType type, int id, Tile* current_tile, string trait);
    Tile* GetCurrentTile();
    void SetCurrentTile(Tile* tile);
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
    Tile* current_tile = nullptr;
    int id;
    CreatureType type;
    string trait;
    int max_energy;
};

#endif