#ifndef TILEINFO_H
#define TILEINFO_H

#include "tile.h"

class TileInfo {
public:
    TileInfo(int draw = 0, char collisionFlags = COLLIDE_NONE): Draw(draw),Collision(collisionFlags){}
    int Draw;
    char Collision;
    static const char COLLIDE_NONE  = 0;
    static const char COLLIDE_NORTH = 1;
    static const char COLLIDE_SOUTH = 2;
    static const char COLLIDE_EAST  = 4;
    static const char COLLIDE_WEST  = 8;
    static const char COLLIDE_ALL   = 15;
};

#endif // TILEINFO_H
