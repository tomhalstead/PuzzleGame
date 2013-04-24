#include "room.h"

Room::Room(size_t rows, size_t cols, int defaultConnection):
    Graph<Tile,int>(rows*cols,defaultConnection), TileSet(NULL),numRows(rows),
    numCols(cols){}//, PuzzleLayer(NULL) {}

size_t Room::Rows() const {
    return numRows;
}

size_t Room::Cols() const {
    return numCols;
}

size_t Room::Size() const
{
    return numRows * numCols;
}

Tile &Room::getTile(size_t row, size_t col)
{
    return this->Vertex(ItemIndex(row,col));
}

Tile &Room::getTile(size_t index)
{
    return this->Vertex(index);
}

void Room::setTile(size_t index, size_t type)
{
    TileInfo info = TileSet->at(type);
    size_t row,col, check;
    int connect = Tile::CONNECTION_NORMAL;
    Coordinates(index,row,col);
    getTile(index).Type = type;
    getTile(index).Draw = info.Draw;
    if(info.Collision & TileInfo::COLLIDE_PUZZLE) {
        connect |= Tile::CONNECTION_PUZZLE;
        getTile(index).RoomLink.Puzzle = info.Index;
    }
    if(info.Collision & TileInfo::COLLIDE_ITEM) {
        connect |= Tile::CONNECTION_ITEM;
        getTile(index).RoomLink.Item = info.Item;
    }
    if(col < Cols()-1 ) {
        check = ItemIndex(row,col+1);
        if(info.Collision & TileInfo::COLLIDE_EAST)
            Connection(ItemIndex(row,col+1), index) = Tile::CONNECTION_NONE;
        else
            Connection(ItemIndex(row,col+1), index) = connect;
    }
    if(col) {
        check = ItemIndex(row,col-1);
        if(info.Collision & TileInfo::COLLIDE_WEST)
            Connection(ItemIndex(row,col-1), index) = Tile::CONNECTION_NONE;
        else
            Connection(ItemIndex(row,col-1), index) = connect;
    }
    if(row) {
        check = ItemIndex(row-1,col);
        if(info.Collision & TileInfo::COLLIDE_NORTH)
            Connection(ItemIndex(row-1,col), index) = Tile::CONNECTION_NONE;
        else
            Connection(ItemIndex(row-1,col), index) = connect;
    }
    if(row < Rows()-1) {
        check = ItemIndex(row+1,col);
        if(info.Collision & TileInfo::COLLIDE_SOUTH)
            Connection(ItemIndex(row+1,col), index) = Tile::CONNECTION_NONE;
        else
            Connection(ItemIndex(row+1,col), index) = connect;
    }
}

size_t Room::ItemIndex(size_t row, size_t col)
{
    return (row * numCols) + col;
}

void Room::Coordinates(size_t index, size_t &row, size_t &col)
{
    row = index / numCols;
    col = index % numCols;
}

int &Room::Connection(size_t from, size_t to)
{
    return this->Edge(from,to);
}
