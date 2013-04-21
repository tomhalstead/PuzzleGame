#ifndef MAP_H
#define MAP_H

#include "graph.h"
#include "room.h"
#include "link.h"
#include "tileinfo.h"
#include <vector>
#include <string>

class Map: private Graph<Room*,Link>
{
public:
    Map(size_t n=0);
    ~Map();
    Room* &getRoom(size_t index);
    Link& getLink(size_t from, size_t to);
    void setTileSet(std::vector<TileInfo>* TileSet);
    std::string Name;
private:
    size_t numRooms;
    std::vector<TileInfo>* tileSet;
};

#endif // MAP_H
