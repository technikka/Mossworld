#include <algorithm>
#include <cmath>
#include <iostream>

#include "Tile.h"
#include "WorldConfig.h"

using namespace std;

Tile::Tile(int id, int x, int y, const WorldConfig& config)
    : config(config), id(id), x(x), y(y), moisture(config.moisture.min) {}

int Tile::GetId() { return id; }

Entity* Tile::GetOccupant() const { return occupant; }

void Tile::SetOccupant(Entity* occupant) { this->occupant = occupant; };

void Tile::RemoveOccupant() { occupant = nullptr; }

bool Tile::IsEmpty() const { return occupant == nullptr; };

bool Tile::HasType(EntityType type) const {
    return occupant != nullptr && occupant->GetType() == type;
}

bool Tile::HasCreature() const { return HasType(CREATURE); }

bool Tile::HasNutrientCluster() const { return HasType(NUTRIENT_CLUSTER); }

bool Tile::HasStone() const { return HasType(STONE); }

string Tile::GetSymbol() const {
    if (occupant == nullptr) {
        return ".";
    }

    return occupant->GetSymbol();
}

Position Tile::GetPosition() const {
    Position position;
    position.x = x;
    position.y = y;
    return position;
}

Position Tile::IdToCoordinates(int tile_id, int world_width) {
    int x = tile_id % world_width;
    int y = tile_id / world_width;
    Position position;
    position.x = x;
    position.y = y;
    return position;
}

double Tile::GetMoisture() const { return moisture; };

void Tile::AdjustMoisture(double adjustment) {
    moisture =
        clamp(moisture + adjustment, config.moisture.min, config.moisture.max);
};

double Tile::GetFertility() const { return fertility; }

void Tile::SetFertility(double fertility) {
    this->fertility = max(config.fertility.min, fertility);
}

void Tile::AdjustFertility(double adjustment) {
    fertility = clamp(fertility + adjustment, config.fertility.min,
                      config.fertility.max);
}

void Tile::SetBaseSunlight(double sunlight) {
    base_sunlight = max(config.sunlight.min, sunlight);
}

double Tile::GetBaseSunlight() const { return base_sunlight; }

double Tile::GetEffectiveSunlight() const { return effective_sunlight; }

void Tile::SetEffectiveSunlight(double amount) {
    effective_sunlight = max(0.0, amount);
}

FertilityLevel Tile::GetFertilityLevel() const {
    if (fertility == 0.0) return FertilityLevel::None;
    if (fertility <= 4.0) return FertilityLevel::Low;
    if (fertility <= 7.0) return FertilityLevel::Moderate;
    return FertilityLevel::High;
}

MoistureLevel Tile::GetMoistureLevel() const {
    if (moisture <= 2.0) return MoistureLevel::Dry;
    if (moisture <= 4.0) return MoistureLevel::Damp;
    if (moisture <= 6.0) return MoistureLevel::Ideal;
    if (moisture <= 8.0) return MoistureLevel::Wet;
    return MoistureLevel::Saturated;
}

string ToString(MoistureLevel level) {
    switch (level) {
        case MoistureLevel::Dry:
            return "dry";
        case MoistureLevel::Damp:
            return "damp";
        case MoistureLevel::Saturated:
            return "saturated";
        case MoistureLevel::Ideal:
            return "ideal";
        case MoistureLevel::Wet:
            return "wet";
    }
    return "unknown";
}

SunlightLevel Tile::GetEffectiveSunlightLevel() const {
    if (effective_sunlight <= 2.0) return SunlightLevel::Dark;
    if (effective_sunlight <= 4.0) return SunlightLevel::Low;
    if (effective_sunlight <= 6.0) return SunlightLevel::Moderate;
    return SunlightLevel::Bright;
}

int Tile::GetNutrientGrowthProgress() const { return nutrient_growth_progress; }

void Tile::AdjustNutrientGrowthProgress(int adjustment) {
    nutrient_growth_progress = clamp(nutrient_growth_progress + adjustment,
                                     config.nutrient_growth.progress_floor,
                                     config.nutrient_growth.progress_ceiling);
}

void Tile::ResetNutrientGrowthProgress() {
    nutrient_growth_progress = config.nutrient_growth.progress_floor;
}

bool Tile::CanGrowNutrient() {
    return nutrient_growth_progress >= config.nutrient_growth.progress_ceiling;
}

double Tile::GetElevation() const { return elevation; }

void Tile::SetElevation(double elevation) { this->elevation = elevation; }

double Tile::GetCanopyCover() const { return canopy_cover; }

void Tile::SetCanopyCover(double density) { canopy_cover = density; }
