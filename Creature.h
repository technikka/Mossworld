#ifndef CREATURE_H
#define CREATURE_H

#include <string>
#include <vector>

#include "Entity.h"
#include "Position.h"
#include "Tile.h"

// * add creature types here
enum CreatureType { MOSSLING };
std::string CreatureTypeToString(CreatureType type);

class Creature : public Entity {
   public:
    Creature(CreatureType type, int id, Tile* current_tile, std::string trait);
    std::string GetTypeString();
    char GetTypeSymbol();
    CreatureType GetType();
    int GetId();
    std::vector<Tile*> tile_history;
    std::string GetTrait();
    int energy;
    int GetMaxEnergy();
    void LoseDailyEnergy();
    void RestoreEnergy();

   private:
    int id;
    CreatureType type;
    std::string trait;
    int max_energy;
};

#endif