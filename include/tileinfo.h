#ifndef TILEINFO_H
#define TILEINFO_H

#include "tile.h"
#include <vector>
#include <QString>

struct TileInfo {
public:
    TileInfo(int draw = 0, char collisionFlags = COLLIDE_NONE): Draw(draw),Collision(collisionFlags){}
    int Draw;
    char Collision;
    size_t Index;
    size_t Item;
    static const char COLLIDE_NONE   = 0;
    static const char COLLIDE_NORTH  = 1;
    static const char COLLIDE_SOUTH  = 2;
    static const char COLLIDE_EAST   = 4;
    static const char COLLIDE_WEST   = 8;
    static const char COLLIDE_ALL    = 15;
    static const char COLLIDE_PUZZLE = 16;
    static const char COLLIDE_ITEM   = 32;
};

struct TileSet {
    std::vector<TileInfo> Tiles;
    size_t TileSize;
    size_t TilesWide;
    size_t TilesHigh;
    QString FileName;
};

#endif // TILEINFO_H
