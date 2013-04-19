#include "map.h"

Map::Map(size_t n): Graph<Room*,Link>(n,Link(0,0)), numRooms(n)
{
    for(size_t i = 0; i < numRooms; i++)
        this->Vertex(i) = NULL;
}

Map::~Map() {
    for(size_t i = 0; i < numRooms; i++)
        if(this->Vertex(i))
            delete this->Vertex(i);
}

Room* &Map::getRoom(size_t index)
{
    return this->Vertex(index);
}

Link &Map::getLink(size_t from, size_t to)
{
    return this->Edge(from,to);
}
