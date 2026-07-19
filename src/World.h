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

    template <typename Callable>
    void PrintTileView(Callable callable);

    void PrintStatusBar() const;
    void PrintCreatureBar();
    void PrintObserverMenu() const;
    void BeginDay();
    void Observe();
    void RunCreatures();
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
    int next_creature_id = 1;
    int day = 1;

    int GetEntityCount(EntityType type) const;
    void createTiles();
    void PlaceEntity(EntityType entity, Tile& tile);
    void PlaceNutrientCluster(Tile& tile);
    void InitializeNutrientClusters();
    void InitializeCreatures();
    Entity* CreateEntity(EntityType type, Tile* tile);
    void RemoveEntity(EntityType type, Tile* tile);
    std::vector<Tile*> GetAdjacentOpenTiles(Tile* tile);
    Tile* SelectCreatureTile(Creature& creature);
    void MoveCreature(Creature& creature);
    std::string GetTrait();
    void ManageNutrientClusters();
    int CalculateNutrientGrowth(const Tile& tile);
    int CalculateFertilityGrowthModifier(const Tile& tile);
    int CalculateMoistureGrowthModifier(const Tile& tile);
    int CalculateSunlightGrowthModifier(const Tile& tile);
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
    void InitializeSunlight();
    void ApplyMorningDew();
    void ApplyEvaporation();
    void PlaceMoistureSources(int initial_amount, int sources,
                              int spread_distance);
    void PlaceMoistureSource(int amount, int start_id, int end_id,
                             int spread_distance);
    void PlaceMoistureSpread(Tile* tile, int amount, int spread_distance);
    void PlaceSunlightSpread(Tile* tile, int amount, int spread_distance);

    template <typename Callable>
    void ForEachTileInRing(Tile* tile, int spread_amount, int distance,
                           Callable callable);

    Tile* SelectRandomEmptyTile(int start_id, int end_id);
    Tile* SelectRandomTile(int start_id, int end_id);
    Tile* SelectRandomNutrientGrowthTile();
    string EnergyBar(int energy, int max_energy);
    string MoistureBar(int current, int ideal);
    int ScoreTile(Tile& tile, Creature& creature);
    int ScoreObjective(Tile& tile, Creature& creature);
    int ScoreMoisture(Tile& tile, Creature& creature);
    int ScoreBacktracking(Tile& tile, Creature& creature);
};

#endif