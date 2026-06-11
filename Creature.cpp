#include <iostream>
using namespace std;

#include "Creature.h"
#include "Position.h"

Creature::Creature(CreatureType type, int id, Position position) {
    this->type = type;
    this->id = id;
    this->position = position;
    position_history.push_back(position);
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