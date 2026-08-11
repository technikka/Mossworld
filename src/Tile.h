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
    double GetMoisture() const;
    void AdjustMoisture(double amount);
    double GetFertility() const;
    void SetFertility(double fertility);
    void AdjustFertility(double adjustment);
    double GetBaseSunlight() const;
    void SetBaseSunlight(double sunlight);
    double GetEffectiveSunlight() const;
    void SetEffectiveSunlight(double amount);
    SunlightLevel GetEffectiveSunlightLevel() const;
    FertilityLevel GetFertilityLevel() const;
    MoistureLevel GetMoistureLevel() const;
    int GetNutrientGrowthProgress() const;
    void AdjustNutrientGrowthProgress(int adjustment);
    void ResetNutrientGrowthProgress();
    bool CanGrowNutrient();
    double GetElevation() const;
    void SetElevation(double elevation);
    double GetCanopyCover() const;
    void SetCanopyCover(double density);

   private:
    const WorldConfig& config;
    int id;
    int x;
    int y;
    Entity* occupant = nullptr;
    double moisture = 0;
    double fertility = 0;
    double base_sunlight = 0.0;
    double effective_sunlight = 0.0;
    int nutrient_growth_progress = 0;
    double elevation = 0.0;
    double canopy_cover = 0.0;
};

#endif