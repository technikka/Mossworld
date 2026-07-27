#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <vector>

#include "Creature.h"
#include "Narration.h"
#include "NutrientCluster.h"
#include "Stone.h"
#include "Tile.h"
#include "WorldConfig.h"

class World {
   public:
    World(const WorldConfig& config);
    int GetWidth();
    int GetHeight();
    void PrintView();

    template <typename Callable>
    void PrintTileView(Callable callable);

    void PrintStatusBar() const;
    void PrintCreatureBar();
    void PrintObserverMenu() const;
    void PrintEventNotification() const;
    void PrintWrapped(const std::string& text, std::size_t width,
                      std::size_t indent) const;
    void PrintDivider() const;
    void PrintLeftMargin() const;
    void PrintCentered(const string& text) const;
    void PrintLine(const string& text) const;
    void PrintJournal() const;
    void ToggleJournal();
    bool IsJournalOpen() const;
    void BeginDay();
    void Observe();
    void RunCreatures();
    void SetViewMode(ViewMode mode);

    // * Grid indexing:
    // * tiles[row][column]
    // * tiles[0] = first row
    // * tiles[0][1] = first row, second column
    std::vector<std::vector<Tile>> tiles;

    std::vector<std::unique_ptr<Creature>> creatures;
    std::vector<std::unique_ptr<NutrientCluster>> nutrient_clusters;
    std::vector<std::unique_ptr<Stone>> stones;
    std::vector<std::string> available_traits;

   private:
    const WorldConfig config;
    ViewMode view_mode;
    bool journal_open = false;
    int height;
    int width;
    int next_creature_id = 1;
    int day = 1;

    template <typename Callable>
    void ForEachTile(Callable callable);
    template <typename Callable>
    void ForEachTileWithPosition(Callable callable);

    void createTiles();
    void PlaceEntity(EntityType entity, Tile& tile);
    void PlaceNutrientCluster(Tile& tile);
    void InitializeNutrientClusters();
    void InitializeCreatures();
    Entity* CreateEntity(EntityType type, Tile* tile);
    void RemoveEntity(Tile* tile);
    std::vector<Tile*> GetAdjacentOpenTiles(Tile* tile);
    Tile* SelectCreatureTile(Creature& creature);
    void MoveCreature(Creature& creature);
    std::string GetTrait();
    void ManageNutrientClusters();
    void UpdateNutrientClusterStress(NutrientCluster& nutrient_cluster);
    int CalculateNutrientStress(Tile& tile);
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
    int CalculateEffectiveSunlight(Tile& tile);
    void UpdateSunlight();
    void InitializeStone();
    void ApplyMorningDew();
    void ApplyEvaporation();
    void PlaceMoistureSources(int initial_amount, int sources,
                              int spread_distance);
    void PlaceMoistureSource(int amount, int start_id, int end_id,
                             int spread_distance);
    void PlaceMoistureSpread(Tile* tile, int amount, int spread_distance);
    void PlaceSunlightSpread(Tile* tile, int amount, int spread_distance);
    void PlaceShadeSpread(Tile* tile, int amount, int spread_distance);

    template <typename Callable>
    void ForEachTileInRing(Tile* tile, int distance, Callable callable);

    Tile* SelectRandomEmptyTile(int start_id, int end_id);
    Tile* SelectRandomTile(int start_id, int end_id);
    Tile* SelectRandomNutrientGrowthTile();
    string EnergyBar(int energy, int max_energy);
    string PreferenceBar(Creature& creature);
    int ScoreTile(Tile& tile, Creature& creature);
    int ScoreObjective(Tile& tile, Creature& creature);
    int ScoreMoisture(Tile& tile, Creature& creature);
    int ScoreSunlight(Tile& tile, Creature& creature);
    int ScoreBacktracking(Tile& tile, Creature& creature);
};

#endif