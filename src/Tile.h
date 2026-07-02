#ifndef TILE_H
#define TILE_H

#include "Entity.h"
#include "EntityType.h"
#include "Position.h"

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
    void AddMoisture(int amount, int max_moisture);

   private:
    int id;
    int x;
    int y;
    Entity* occupant = nullptr;
    int moisture;
};

#endif