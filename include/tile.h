#ifndef TILE_H
#define TILE_H

#include "link.h"

struct Tile
{
    Tile(): Type(0), Data(0){}
    int Type;
    int Draw;
    int Data;
    Link RoomLink;
    static const int CONNECTION_ITEM   = 4;
    static const int CONNECTION_PUZZLE = 2;
    static const int CONNECTION_NORMAL = 1;
    static const int CONNECTION_NONE   = 0;
    static const int DATA_ROOMEDGE     = -1;
    static const int DATA_VICTORY      = -2;
    static const int DATA_NOCOPY       = -3;
};

#endif // TILE_H
