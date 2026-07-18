#include <algorithm>
#include <iostream>
using namespace std;

#include "Tile.h"
#include "WorldConfig.h"

Tile::Tile(int id, int x, int y) {
    this->id = id;
    this->x = x;
    this->y = y;
    moisture = 0;
};

Tile::Tile() {
    id = -1;
    x = -1;
    y = -1;
}

int Tile::GetId() { return id; }

Entity* Tile::GetOccupant() const { return occupant; }

void Tile::SetOccupant(Entity* occupant) { this->occupant = occupant; };

bool Tile::IsEmpty() const { return occupant == nullptr; };

bool Tile::HasType(EntityType type) const {
    return occupant != nullptr && occupant->GetType() == type;
}

bool Tile::HasCreature() const { return HasType(CREATURE); }

bool Tile::HasNutrientCluster() const { return HasType(NUTRIENT_CLUSTER); }

char Tile::GetSymbol() const {
    if (occupant == nullptr) {
        return '.';
    }

    return occupant->GetSymbol();
}

Position Tile::GetPosition() {
    Position position;
    position.x = x;
    position.y = y;
    return position;
}

Position Tile::IdToCoordinates(int tile_id, int world_width, int world_height) {
    int x = tile_id % world_width;
    int y = tile_id / world_width;
    Position position;
    position.x = x;
    position.y = y;
    return position;
}

int Tile::GetMoisture() const { return moisture; };

void Tile::AdjustMoisture(int adjustment) {
    moisture = clamp(moisture + adjustment, min_moisture, max_moisture);
};

void Tile::SetMoisture(int moisture) { this->moisture = moisture; }

int Tile::GetFertility() const { return fertility; }

void Tile::SetFertility(int fertility) { this->fertility = fertility; }

void Tile::AdjustFertility(int adjustment) {
    fertility = clamp(fertility + adjustment, min_fertility, max_fertility);
}

void Tile::SetSunlight(int sunlight) { this->sunlight = sunlight; }

int Tile::GetSunlight() const { return sunlight; }

void Tile::SetSunlightIfGreater(int amount) {
    if (amount > sunlight) {
        sunlight = amount;
    }
}

FertilityLevel Tile::GetFertilityLevel() const {
    if (fertility == 0) return FertilityLevel::None;
    if (fertility <= 4) return FertilityLevel::Low;
    if (fertility <= 7) return FertilityLevel::Moderate;
    return FertilityLevel::High;
}

MoistureLevel Tile::GetMoistureLevel() const {
    if (moisture <= 2) return MoistureLevel::Dry;
    if (moisture <= 4) return MoistureLevel::Damp;
    if (moisture <= 6) return MoistureLevel::Ideal;
    if (moisture <= 8) return MoistureLevel::Wet;
    return MoistureLevel::Saturated;
}

SunlightLevel Tile::GetSunlightLevel() const {
    if (sunlight <= 2) return SunlightLevel::Dark;
    if (sunlight <= 4) return SunlightLevel::Low;
    if (sunlight <= 6) return SunlightLevel::Moderate;
    return SunlightLevel::Bright;
}

int Tile::GetNutrientGrowthProgress() const { return nutrient_growth_progress; }

void Tile::AdjustNutrientGrowthProgress(int adjustment) {
    nutrient_growth_progress =
        clamp(nutrient_growth_progress + adjustment,
              nutrient_growth_progress_floor, nutrient_growth_progress_ceiling);
}

void Tile::ResetNutrientGrowthProgress() {
    nutrient_growth_progress = nutrient_growth_progress_floor;
}

bool Tile::CanGrowNutrient() {
    return nutrient_growth_progress >= nutrient_growth_progress_ceiling;
}
