#ifndef TILE_H
#define TILE_H

#include "Entity.h"
#include "EntityType.h"
#include "Position.h"

enum class FertilityLevel { None, Low, Moderate, High };
enum class MoistureLevel { Dry, Damp, Ideal, Wet, Saturated };

class Tile {
   public:
    Tile();
    Tile(int id, int x, int y);
    int GetId();
    bool HasType(EntityType type) const;
    bool HasCreature() const;
    bool HasNutrientCluster() const;
    Entity* GetOccupant() const;
    void SetOccupant(Entity* occupant);
    bool IsEmpty() const;
    char GetSymbol() const;
    Position GetPosition();
    static Position IdToCoordinates(int tile_id, int world_width,
                                    int world_height);
    int GetMoisture() const;
    void SetMoisture(int moisture);
    void AdjustMoisture(int amount);
    void SetFertility(int fertility);
    void AdjustFertility(int adjustment);
    int GetFertility() const;
    FertilityLevel GetFertilityLevel() const;
    MoistureLevel GetMoistureLevel() const;

   private:
    int id;
    int x;
    int y;
    Entity* occupant = nullptr;
    int moisture;
    int fertility;
    static constexpr int MinMoisture = 0;
    static constexpr int MaxMoisture = 10;
    static constexpr int MinFertility = 0;
    static constexpr int MaxFertility = 10;
};

#endif