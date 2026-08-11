#include <iostream>

#include "Creature.h"
#include "Position.h"
#include "WorldConfig.h"

using namespace std;

double RandomDouble(double min, double max) {
    return min + static_cast<double>(rand()) / RAND_MAX * (max - min);
}

Creature::Creature(CreatureType type, int id, Tile* current_tile, string trait,
                   const WorldConfig& config)
    : Entity(CREATURE, current_tile),
      config(config),
      energy(config.creature.initial_energy),
      id(id),
      type(type),
      trait(trait) {
    symbol = TraitToSymbol();
    tile_history.push_back(current_tile);
    {
        ideal_moisture = RandomDouble(config.moisture.min, config.moisture.max);
        ideal_sunlight = RandomDouble(config.sunlight.min, config.sunlight.max);
    }
}

string Creature::TraitToSymbol() const {
    char symbol = static_cast<char>(tolower(trait[0]));
    return string(1, symbol);
}

string CreatureTypeToString(CreatureType type) {
    switch (type) {
        case MOSSLING:
            return "Mossling";
    }
    return "?";  // should never be reached
}

int Creature::GetId() { return id; }

int Creature::GetEnergy() const { return energy; }

NutrientNeed Creature::GetNutrientNeed() const {
    if (energy <= 2) {
        return NutrientNeed::High;
    } else if (energy <= 6) {
        return NutrientNeed::Medium;
    } else {
        return NutrientNeed::Low;
    }
}

CreatureType Creature::GetType() { return type; }

string Creature::GetTypeString() { return CreatureTypeToString(this->type); }

string Creature::GetTrait() { return trait; }

int Creature::GetMaxEnergy() { return config.creature.max_energy; }

void Creature::LoseDailyEnergy() {
    if (energy > 0) {
        energy -= 1;
    }
}

void Creature::RestoreEnergy() { energy = config.creature.max_energy; }

Tile* Creature::GetObjective() const { return objective_tile; }

void Creature::SetObjective(Tile* tile) { objective_tile = tile; }

void Creature::ClearObjective() { objective_tile = nullptr; }

bool Creature::HasObjective() const {
    if (objective_tile != nullptr) {
        return true;
    }
    return false;
}

double Creature::GetIdealMoisture() { return ideal_moisture; }

double Creature::GetIdealSunlight() { return ideal_sunlight; }