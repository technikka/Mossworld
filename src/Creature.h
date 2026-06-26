#ifndef CREATURE_H
#define CREATURE_H

#include <string>
#include <vector>

#include "Entity.h"
#include "Tile.h"

enum class NutrientNeed { Low, Medium, High };

// * add creature types here
enum CreatureType { MOSSLING };
std::string CreatureTypeToString(CreatureType type);

class Creature : public Entity {
   public:
    Creature(CreatureType type, int id, Tile* current_tile, std::string trait);
    std::vector<Tile*> tile_history;

    // * Getters
    std::string GetTypeString();
    char GetTypeSymbol();
    CreatureType GetType();
    int GetId();
    std::string GetTrait();
    int GetMaxEnergy();
    Tile* GetObjective() const;
    int GetEnergy() const;
    NutrientNeed GetNutrientNeed() const;

    // * Setters
    void SetObjective(Tile* tile);

    // * Behaviors
    void ClearObjective();
    bool HasObjective() const;
    void LoseDailyEnergy();
    void RestoreEnergy();

   private:
    int energy;
    int id;
    CreatureType type;
    std::string trait;
    int max_energy;
    // more than one creature can share an objective
    Tile* objective_tile = nullptr;
};

#endif