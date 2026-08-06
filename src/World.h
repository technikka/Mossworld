#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include <vector>

#include "Creature.h"
#include "Environment.h"
#include "Narration.h"
#include "NutrientCluster.h"
#include "Stone.h"
#include "Tile.h"
#include "WorldConfig.h"

class World {
   public:
    World(const WorldConfig& config);
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
    void PrintCentered(const std::string& text) const;
    void PrintLine(const std::string& text) const;
    void PrintJournal() const;
    void ToggleJournal();
    bool IsJournalOpen() const;
    void BeginDay();
    void Observe();
    void RunCreatures();
    void SetViewMode(ViewMode mode);
    void UpdateMemory();
    void UpdateStoneMemory();

   private:
    std::vector<std::unique_ptr<Creature>> creatures;
    std::vector<std::unique_ptr<NutrientCluster>> nutrient_clusters;
    std::vector<std::unique_ptr<Stone>> stones;
    std::vector<std::string> available_traits;

    const WorldConfig config;
    ViewMode view_mode;
    int day = 1;
    TileMap tile_map;
    Environment environment;
    bool journal_open = false;
    int next_creature_id = 1;

    void PlaceEntity(EntityType entity, Tile& tile);
    void PlaceNutrientCluster(Tile& tile);
    void InitializeNutrientClusters();
    void InitializeCreatures();
    Entity* CreateEntity(EntityType type, Tile* tile);
    void RemoveEntity(Tile* tile);
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
    void InitializeStone();
    Tile* SelectRandomNutrientGrowthTile();
    std::string EnergyBar(int energy, int max_energy);
    std::string PreferenceBar(Creature& creature);
    int ScoreTile(Tile& tile, Creature& creature);
    int ScoreObjective(Tile& tile, Creature& creature);
    int ScoreMoisture(Tile& tile, Creature& creature);
    int ScoreSunlight(Tile& tile, Creature& creature);
    int ScoreBacktracking(Tile& tile, Creature& creature);

    // * Narration Helpers
    std::string DescribeStone(Stone& stone);
};

#endif