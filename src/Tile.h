#ifndef TILE_H
#define TILE_H

#include "Entity.h"
#include "EntityType.h"
#include "Position.h"

struct WorldConfig;

enum class FertilityLevel { None, Low, Moderate, High };
enum class MoistureLevel { Dry, Damp, Ideal, Wet, Saturated };
enum class SunlightLevel { Dark, Low, Moderate, Bright };

class Tile {
   public:
    Tile(int id, int x, int y, const WorldConfig& config);
    int GetId();
    bool HasType(EntityType type) const;
    bool HasCreature() const;
    bool HasNutrientCluster() const;
    Entity* GetOccupant() const;
    void SetOccupant(Entity* occupant);
    bool IsEmpty() const;
    char GetSymbol() const;
    Position GetPosition();
    static Position IdToCoordinates(int tile_id, int world_width);
    int GetMoisture() const;
    void SetMoisture(int moisture);
    void AdjustMoisture(int amount);
    int GetFertility() const;
    void SetFertility(int fertility);
    void AdjustFertility(int adjustment);
    int GetSunlight() const;
    void SetSunlight(int sunlight);
    void SetSunlightIfGreater(int amount);
    SunlightLevel GetSunlightLevel() const;
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
    int sunlight = 0;
    int nutrient_growth_progress = 0;
};

#endif