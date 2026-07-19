#include <iostream>
using namespace std;

#include "Creature.h"
#include "Position.h"
#include "WorldConfig.h"

Creature::Creature(CreatureType type, int id, Tile* current_tile, string trait,
                   const WorldConfig& config)
    : Entity(CREATURE, current_tile),
      config(config),
      type(type),
      id(id),
      trait(trait),
      energy(config.creature.initial_energy),
      ideal_moisture(rand() % (config.moisture.max - config.moisture.min + 1) +
                     config.moisture.min),
      ideal_sunlight(rand() % (config.sunlight.max - config.sunlight.min + 1) +
                     config.sunlight.min) {
    symbol = TraitToSymbol();
    tile_history.push_back(current_tile);
}

char Creature::TraitToSymbol() const { return tolower(trait[0]); }

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

int Creature::GetIdealMoisture() { return ideal_moisture; }

int Creature::GetIdealSunlight() { return ideal_sunlight; }