#include <algorithm>
#include <cstdlib>

#include "TileMap.h"
#include "WorldConfig.h"

using namespace std;

TileMap::TileMap(const WorldConfig& config)
    : config(config), width(config.width), height(config.height) {
    CreateTiles();
}

int TileMap::GetWidth() const { return width; }
int TileMap::GetHeight() const { return height; }

void TileMap::CreateTiles() {
    tiles.clear();
    tiles.resize(height);

    for (int row = 0; row < height; row++) {
        tiles[row].reserve(width);

        for (int column = 0; column < width; column++) {
            int id = row * width + column;
            tiles[row].emplace_back(id, column, row, config);
        }
    }
}

const Tile& TileMap::GetTile(int row, int column) const {
    return tiles[row][column];
}

void TileMap::ForEachTile(const std::function<void(Tile&)>& callable) {
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            callable(tiles[row][column]);
        }
    }
}

void TileMap::ForEachTileWithPosition(
    const std::function<void(Tile&, Position)>& callable) {
    for (int row = 0; row < height; ++row)
        for (int column = 0; column < width; ++column)
            callable(tiles[row][column], Position{column, row});
}

vector<pair<int, int>> TileMap::GetLinearZones(int zone_count) const {
    vector<pair<int, int>> zones;

    int number_of_tiles = width * height;

    for (int i = 0; i < zone_count; i++) {
        int start_id = (i * number_of_tiles) / zone_count;
        int end_id = ((i + 1) * number_of_tiles) / zone_count;

        zones.push_back({start_id, end_id});
    }

    return zones;
}

// Will return a tile regardless of occupant or other effects.
Tile* TileMap::SelectRandomTile(int start_id, int end_id) {
    Tile* tile = nullptr;
    int rand_pos = rand() % (end_id - start_id);
    int tile_id = rand_pos + start_id;

    Position position = Tile::IdToCoordinates(tile_id, width);
    tile = &tiles[position.y][position.x];

    return tile;
}

Tile* TileMap::SelectRandomEmptyTile(int start_id, int end_id) {
    Tile* tile = nullptr;
    do {
        int rand_pos = rand() % (end_id - start_id);
        int tile_id = rand_pos + start_id;

        Position position = Tile::IdToCoordinates(tile_id, width);
        tile = &tiles[position.y][position.x];

    } while (!tile->IsEmpty());  // * Potential infinite loop
    return tile;
}

void TileMap::ForEachTileInRing(Tile* tile, int distance,
                                const std::function<void(Tile&)>& callable) {
    Position pos = tile->GetPosition();

    for (int y_offset = -distance; y_offset <= distance; y_offset++) {
        for (int x_offset = -distance; x_offset <= distance; x_offset++) {
            if (x_offset == 0 && y_offset == 0) {
                continue;
            }
            if (abs(x_offset) != distance && abs(y_offset) != distance) {
                continue;
            }

            int new_x = pos.x + x_offset;
            int new_y = pos.y + y_offset;

            if (new_y < 0 || new_y >= height || new_x < 0 || new_x >= width) {
                continue;
            }
            callable(tiles[new_y][new_x]);
        }
    }
}

vector<Tile*> TileMap::GetAdjacentTiles(const Tile& tile) {
    vector<Tile*> adjacent_tiles;
    vector<Position> possible_positions;
    Position position = tile.GetPosition();

    int x = position.x;
    int y = position.y;

    possible_positions.push_back({x - 1, y});  // left
    possible_positions.push_back({x + 1, y});  // right
    possible_positions.push_back({x, y - 1});  // up
    possible_positions.push_back({x, y + 1});  // down

    for (const Position& position : possible_positions) {
        if (position.x < 0 || position.x >= width) continue;
        if (position.y < 0 || position.y >= height) continue;

        Tile* tile = &tiles[position.y][position.x];
        adjacent_tiles.push_back(tile);
    }
    return adjacent_tiles;
}

// For Creature; NutrientCluster doesn't block openness.
vector<Tile*> TileMap::GetAdjacentOpenTiles(Tile* current_tile) {
    vector<Tile*> adjacent_tiles = GetAdjacentTiles(*current_tile);
    vector<Tile*> valid_tiles;

    for (Tile* tile : adjacent_tiles) {
        if (tile->HasNutrientCluster() || tile->IsEmpty()) {
            valid_tiles.push_back(tile);
        }
    }

    return valid_tiles;
}
