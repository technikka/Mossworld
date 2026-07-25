#ifndef CREATURE_H
#define CREATURE_H

#include <string>
#include <vector>

#include "Entity.h"
#include "Tile.h"

struct WorldConfig;

enum class NutrientNeed { Low, Medium, High };

// * add creature types here
enum CreatureType { MOSSLING };
std::string CreatureTypeToString(CreatureType type);

class Creature : public Entity {
   public:
    Creature(CreatureType type, int id, Tile* current_tile, std::string trait,
             const WorldConfig& config);
    std::vector<Tile*> tile_history;

    // * Getters
    std::string GetTypeString();
    CreatureType GetType();
    int GetId();
    std::string GetTrait();
    int GetMaxEnergy();
    Tile* GetObjective() const;
    int GetEnergy() const;
    NutrientNeed GetNutrientNeed() const;
    int GetIdealMoisture();
    int GetIdealSunlight();

    // * Setters
    void SetObjective(Tile* tile);

    // * Behaviors
    void ClearObjective();
    bool HasObjective() const;
    void LoseDailyEnergy();
    void RestoreEnergy();

   private:
    const WorldConfig& config;
    int energy;
    int id;
    int ideal_moisture;
    int ideal_sunlight;
    CreatureType type;
    std::string trait;
    // more than one creature can share an objective
    Tile* objective_tile = nullptr;
    std::string TraitToSymbol() const;
};

#endif