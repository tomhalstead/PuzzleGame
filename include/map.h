#ifndef MAP_H
#define MAP_H

#include "graph.h"
#include "room.h"
#include "link.h"

class Map: private Graph<Room*,Link>
{
public:
    Map(size_t n);
    Room* &getRoom(size_t index);
    Link& getLink(size_t from, size_t to);
    ~Map();
private:
    size_t numRooms;
};

#endif // MAP_H
