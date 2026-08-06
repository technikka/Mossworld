#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Tile.h"
#include "TileMap.h"
#include "WorldConfig.h"

class Environment {
   public:
    Environment(TileMap& tile_map, const WorldConfig& config, const int& day);
    void Update();

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
    void PlaceMoistureSources(int initial_amount, int sources,
                              int spread_distance);
    void PlaceMoistureSource(int amount, int start_id, int end_id,
                             int spread_distance);
    void PlaceMoistureSpread(Tile* tile, int amount, int spread_distance);

    // * Sunlight
    void InitializeSunlight();
    void UpdateSunlight();
    int CalculateEffectiveSunlight(Tile& tile);
    void PlaceSunlightSpread(Tile* tile, int amount, int spread_distance);

    void PlaceShadeSpread(Tile* tile, int amount, int spread_distance);
};

#endif