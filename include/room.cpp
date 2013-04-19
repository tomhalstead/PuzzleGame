#include "room.h"

Room::Room(size_t rows, size_t cols): Graph<Tile,unsigned int>(rows*cols,1), numRows(rows),numCols(cols) {}

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

size_t Room::ItemIndex(size_t row, size_t col)
{
    return row * numCols + col;
}

unsigned int &Room::Connected(size_t from, size_t to)
{
    return this->Edge(from,to);
}
