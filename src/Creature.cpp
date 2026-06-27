#include <iostream>
using namespace std;

#include "Creature.h"
#include "Position.h"

Creature::Creature(CreatureType type, int id, Tile* current_tile, string trait)
    : Entity(CREATURE, current_tile) {
    this->type = type;
    this->id = id;
    this->trait = trait;
    tile_history.push_back(current_tile);
    energy = 10;
    max_energy = 10;
    symbol = TraitToSymbol();
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

int Creature::GetMaxEnergy() { return max_energy; }

void Creature::LoseDailyEnergy() {
    if (energy > 0) {
        energy -= 1;
    }
}

void Creature::RestoreEnergy() { energy = max_energy; }

Tile* Creature::GetObjective() const { return objective_tile; }

void Creature::SetObjective(Tile* tile) { objective_tile = tile; }

void Creature::ClearObjective() { objective_tile = nullptr; }

bool Creature::HasObjective() const {
    if (objective_tile != nullptr) {
        return true;
    }
    return false;
}