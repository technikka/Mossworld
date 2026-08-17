#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Tile.h"
#include "TileMap.h"
#include "WorldConfig.h"

class Environment {
   public:
    Environment(TileMap& tile_map, const WorldConfig& config, const int& day);
    void Update();
    void Initialize();

   private:
    TileMap& tile_map;
    const WorldConfig& config;
    const int& day;
    // * Fertility
    void InitializeTileFertility();
    void UpdateFertility();
    void UpdateTileFertility(Tile& tile);

    // * Moisure
    void IntializeMoisture();
    void ApplyMorningDew();
    void ApplyEvaporation();
    void PlaceMoistureSources(double initial_amount, int sources,
                              int spread_distance);
    void PlaceMoistureSource(double amount, int start_id, int end_id,
                             int spread_distance);
    void PlaceMoistureSpread(Tile* tile, double amount, int spread_distance);

    // * Sunlight
    void InitializeSunlight();
    void UpdateSunlight();
    void PlaceShadeSpread(Tile* tile, double amount, int spread_distance);
};

#endif