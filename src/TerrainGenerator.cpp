#include "TerrainGenerator.h"
#include <random>

using namespace std;

TerrainGenerator::TerrainGenerator(const WorldConfig& config)
    : config(config) {}

void TerrainGenerator::SeedPerlin() {
    random_device rd;
    perlin.reseed(rd());
}

void TerrainGenerator::GenerateNoiseField(
    TileMap& tile_map, double noise_scale,
    const function<void(Tile&, double)>& callable) {
    SeedPerlin();

    tile_map.ForEachTileWithPosition([&](Tile& tile, Position position) {
        double x = position.x * noise_scale;
        double y = position.y * noise_scale;

        double noise = perlin.noise2D_01(x, y);
        callable(tile, noise);
    });
}

void TerrainGenerator::Generate(TileMap& tile_map) {
    double noise_scale = 0.1;
    GenerateNoiseField(tile_map, noise_scale, [](Tile& tile, double noise) {
        tile.SetElevation(noise);
    });
    GenerateNoiseField(tile_map, noise_scale, [](Tile& tile, double noise) {
        tile.SetCanopyCover(noise);
    });
}