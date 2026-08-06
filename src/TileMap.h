#ifndef TILEMAP_H
#define TILEMAP_H

#include <functional>
#include <utility>
#include <vector>

#include "Tile.h"

class TileMap {
   public:
    TileMap(const WorldConfig& config);
    int GetWidth() const;
    int GetHeight() const;

    const Tile& GetTile(int row, int column) const;
    void ForEachTile(const std::function<void(Tile&)>& callable);
    void ForEachTileWithPosition(
        const std::function<void(Tile&, Position)>& callable);
    void ForEachTileInRing(Tile* tile, int distance,
                           const std::function<void(Tile&)>& callable);
    Tile* SelectRandomTile(int start_id, int end_id);
    Tile* SelectRandomEmptyTile(int start_id, int end_id);
    std::vector<std::pair<int, int>> GetLinearZones(int zone_count) const;
    std::vector<Tile*> GetAdjacentTiles(const Tile& tile);
    std::vector<Tile*> GetAdjacentOpenTiles(Tile* tile);

   private:
    const WorldConfig& config;
    int width;
    int height;

    // * Grid indexing:
    // * tiles[row][column]
    // * tiles[0] = first row
    // * tiles[0][1] = first row, second column
    std::vector<std::vector<Tile>> tiles;

    void CreateTiles();
};

#endif