#include <iostream>
using namespace std;

#include "World.h"

World::World(int creature_count, int nutrient_count, int width, int height) {
  this->width = width;
  this->height = height;
  this->creature_count = creature_count;
  this->nutrient_count = nutrient_count;
  this->createTiles();
}

int World::GetWidth() { return width; }
int World::GetHeight() { return height; }
int World::GetCreatureCount() { return creature_count; }
int World::GetNutrientCount() { return nutrient_count; }

void World::createTiles() {
  tiles.resize(height, vector<Tile>(width));
  for (int row = 0; row < height; row++) {
    for (int column = 0; column < width; column++) {
      Tile& tile = tiles[row][column];
      tile.id = row * width + column;
      tile.x = column;
      tile.y = row;
      //   tile.occupant = EMPTY;
    }
  }
}

void World::print() {
  for (int row = 0; row < height; row++) {
    for (int column = 0; column < width; column++) {
      //   char symbol = OccupantTypeToChar(world[row][column].occupant);
      //   cout << ' ' << symbol << ' ';
      cout << ' ' << '.' << ' ';
    }
    cout << endl;
  }
  cout << endl;
}