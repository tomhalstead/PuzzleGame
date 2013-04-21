#include "room.h"

Room::Room(size_t rows, size_t cols, unsigned int defaultConnection): Graph<Tile,unsigned int>(rows*cols,defaultConnection), TileSet(NULL),numRows(rows),numCols(cols) {}

size_t Room::Rows() const {
    return numRows;
}

size_t Room::Cols() const {
    return numCols;
}

Tile &Room::getTile(size_t row, size_t col)
{
    return this->Vertex(ItemIndex(row,col));
}

Tile &Room::getTile(size_t index)
{
    return this->Vertex(index);
}

size_t Room::ItemIndex(size_t row, size_t col)
{
    return row * numCols + col;
}

void Room::Coordinates(size_t index, size_t &row, size_t &col)
{
    row = index / numRows;
    col = index % numCols;
}

unsigned int &Room::Connected(size_t from, size_t to)
{
    return this->Edge(from,to);
}
