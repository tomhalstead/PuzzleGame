#ifndef TILE_H
#define TILE_H

#include "link.h"

struct Tile
{
    Tile(): Type(0), Data(0), Index(0){}
    int Type;
    int Data;
    size_t Index;

    static const unsigned int CONNECTION_NONE = -1;
    static const unsigned int CONNECTION_NORMAL = 0;

    static const int DATA_MAPEDGE = -1;
    static const int DATA_ROOMEDGE  = -2;
    static const int DATA_VICTORY = -3;
};

#endif // TILE_H
