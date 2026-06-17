#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "Creature.h"
#include "NutrientCluster.h"
#include "Tile.h"

class World {
   public:
    World(int width = 9, int height = 6);
    int GetWidth();
    int GetHeight();
    void Print();
    void PrintHUD();
    void advanceDay();
    int GetEntityCount(EntityType type);

    // * Grid indexing:
    // * tiles[row][column]
    // * tiles[0] = first row
    // * tiles[0][1] = first row, second column
    vector<vector<Tile>> tiles;

    vector<Creature> creatures;
    vector<NutrientCluster> nutrient_clusters;
    vector<string> available_traits;

   private:
    int height;
    int width;
    int creature_count;
    int nutrient_cluster_start_count = 4;
    int creature_start_count = 3;
    int nutrient_cluster_count;
    int next_creature_id = 1;
    int day = 1;

    void createTiles();
    void PlaceEntity(EntityType entity, int start_id, int end_id);
    void PlaceEntities(EntityType entity);
    Entity* CreateEntity(EntityType type, Tile* tile);
    void RemoveEntity(EntityType type, Tile* tile);
    vector<Tile*> GetAdjacentOpenTiles(Tile* tile);
    Tile* SelectCreatureTile(Creature& creature);
    void MoveCreature(Creature& creature);
    string GetTrait();
    void ManageNutrientClusters();
    void HandleNutrientConsumption(Creature& creature, Tile* tile);
};

#endif