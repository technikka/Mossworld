#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "Creature.h"
#include "NutrientCluster.h"
#include "Tile.h"
#include "WorldConfig.h"

class World {
   public:
    World(WorldConfig& config);
    int GetWidth();
    int GetHeight();
    void PrintView();
    void PrintWorldView();
    void PrintMoistureView();
    void PrintStatusBar() const;
    void PrintCreatureBar();
    void PrintObserverMenu() const;
    void PrintFertilityView();
    void BeginDay();
    void Observe();
    void RunCreatures();
    int GetEntityCount(EntityType type);
    void SetViewMode(ViewMode mode);
    ViewMode GetViewMode(ViewMode mode);

    // * Grid indexing:
    // * tiles[row][column]
    // * tiles[0] = first row
    // * tiles[0][1] = first row, second column
    std::vector<std::vector<Tile>> tiles;

    std::vector<std::unique_ptr<Creature>> creatures;
    std::vector<std::unique_ptr<NutrientCluster>> nutrient_clusters;
    std::vector<std::string> available_traits;

   private:
    WorldConfig config;
    ViewMode view_mode;
    int height;
    int width;
    int creature_count;
    int nutrient_cluster_count;
    int next_creature_id = 1;
    int day = 1;

    void createTiles();
    void PlaceEntity(EntityType entity, Tile& tile);
    void PlaceNutrientCluster(FertilityLevel fertility_level);
    void InitializeNutrientClusters();
    void InitializeCreatures();
    Entity* CreateEntity(EntityType type, Tile* tile);
    void RemoveEntity(EntityType type, Tile* tile);
    std::vector<Tile*> GetAdjacentOpenTiles(Tile* tile);
    Tile* SelectCreatureTile(Creature& creature);
    void MoveCreature(Creature& creature);
    std::string GetTrait();
    void ManageNutrientClusters();
    void HandleNutrientConsumption(Creature& creature, Tile* tile);
    void AssessNeeds(Creature& creature);
    void SelectNutrientObjective(Creature& creature, int max_energy);
    Tile* FindNearestNutrientCluster(Creature& creature);
    std::vector<std::pair<int, int>> GetLinearZones(int zone_count) const;
    void UpdateEnvironment();
    void UpdateFertility();
    void UpdateTileFertility(Tile& tile);
    void InitializeEnvironment();
    void IntializeMoisture();
    void InitializeTileFertility();
    void ApplyMorningDew();
    void ApplyEvaporation();
    void PlaceMoistureSources(int initial_amount, int sources,
                              int spread_distance);
    void PlaceMoistureSource(int amount, int start_id, int end_id,
                             int spread_distance);
    void PlaceMoistureSpread(Tile* tile, int amount, int spread_distance);
    void ApplyMoistureRing(Tile* tile, int amount, int radius, double percent);
    Tile* SelectRandomEmptyTile(int start_id, int end_id);
    Tile* SelectRandomFertileTile(FertilityLevel fertility_level);
    string EnergyBar(int energy, int max_energy);
    string MoistureBar(int current, int ideal);
    int ScoreTile(Tile& tile, Creature& creature);
    int ScoreObjective(Tile& tile, Creature& creature);
    int ScoreMoisture(Tile& tile, Creature& creature);
    int ScoreBacktracking(Tile& tile, Creature& creature);
};

#endif