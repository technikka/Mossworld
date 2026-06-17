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
}

char CreatureTypeToSymbol(CreatureType type) {
    switch (type) {
        case MOSSLING:
            return 'k';
    }
    return '?';  // should never be reached
}

string CreatureTypeToString(CreatureType type) {
    switch (type) {
        case MOSSLING:
            return "Mossling";
    }
    return "?";  // should never be reached
}

int Creature::GetId() { return id; }

string Creature::GetTypeString() { return CreatureTypeToString(this->type); }
char Creature::GetTypeSymbol() { return CreatureTypeToSymbol(this->type); }

string Creature::GetTrait() { return trait; }

int Creature::GetMaxEnergy() { return max_energy; }

void Creature::LoseDailyEnergy() { energy -= 1; }

void Creature::RestoreEnergy() {
    energy = max_energy + 1;
}  // +1 so that new day starts with 10/10