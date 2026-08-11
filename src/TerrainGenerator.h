#pragma once

#include <PerlinNoise.hpp>

#include "TileMap.h"
#include "WorldConfig.h"

class TerrainGenerator {
   public:
    explicit TerrainGenerator(const WorldConfig& config);
    void Generate(TileMap& tile_map);

   private:
    const WorldConfig& config;
    siv::PerlinNoise perlin;
    void SeedPerlin();
    void GenerateNoiseField(TileMap& tile_map, double noise_scale,
                            const std::function<void(Tile&, double)>& callable);
};