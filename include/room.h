#ifndef ROOM_H
#define ROOM_H

#include "tile.h"
#include "graph.h"


class Room: private Graph<Tile,unsigned int>
{
public:
    Room(size_t rows=1, size_t cols=1, unsigned int defaultConnection = Tile::CONNECTION_NORMAL);
    size_t Rows() const;
    size_t Cols() const;
    Tile& getTile(size_t row, size_t col);
    size_t ItemIndex(size_t row, size_t col);
    unsigned int& Connected(size_t from, size_t to);
private:
    size_t numRows, numCols;
};

#endif // ROOM_H
