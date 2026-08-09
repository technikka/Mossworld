#include "TerrainGenerator.h"
#include <random>

using namespace std;

TerrainGenerator::TerrainGenerator(const WorldConfig& config) : config(config) {
    random_device rd;
    perlin.reseed(rd());
}

void TerrainGenerator::Generate(TileMap& tile_map) {
    tile_map.ForEachTileWithPosition([&](Tile& tile, Position position) {
        double x = position.x * 0.1;
        double y = position.y * 0.1;

        double elevation = perlin.noise2D_01(x, y);

        tile.SetElevation(elevation);
    });
}