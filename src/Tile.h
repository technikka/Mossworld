#ifndef TILE_H
#define TILE_H

#include "Entity.h"
#include "EntityType.h"
#include "Position.h"

struct WorldConfig;

enum class FertilityLevel { None, Low, Moderate, High };
enum class MoistureLevel { Dry, Damp, Ideal, Wet, Saturated };
enum class SunlightLevel { Dark, Low, Moderate, Bright };

std::string ToString(MoistureLevel level);

class Tile {
   public:
    Tile(int id, int x, int y, const WorldConfig& config);
    int GetId();
    bool HasType(EntityType type) const;
    bool HasCreature() const;
    bool HasNutrientCluster() const;
    bool HasStone() const;
    Entity* GetOccupant() const;
    void SetOccupant(Entity* occupant);
    void RemoveOccupant();
    bool IsEmpty() const;
    std::string GetSymbol() const;
    Position GetPosition() const;
    static Position IdToCoordinates(int tile_id, int world_width);
    int GetMoisture() const;
    void SetMoisture(int moisture);
    void AdjustMoisture(int amount);
    int GetFertility() const;
    void SetFertility(int fertility);
    void AdjustFertility(int adjustment);
    int GetBaseSunlight() const;
    void SetBaseSunlight(int sunlight);
    int GetEffectiveSunlight() const;
    void SetEffectiveSunlight(int amount);
    SunlightLevel GetEffectiveSunlightLevel() const;
    FertilityLevel GetFertilityLevel() const;
    MoistureLevel GetMoistureLevel() const;
    int GetNutrientGrowthProgress() const;
    void AdjustNutrientGrowthProgress(int adjustment);
    void ResetNutrientGrowthProgress();
    bool CanGrowNutrient();

   private:
    const WorldConfig& config;
    int id;
    int x;
    int y;
    Entity* occupant = nullptr;
    int moisture = 0;
    int fertility = 0;
    int base_sunlight = 0;
    int effective_sunlight = 0;
    int nutrient_growth_progress = 0;
};

#endif