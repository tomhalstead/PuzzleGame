#ifndef ROOM_H
#define ROOM_H

#include "puzzle.h"
#include "tileinfo.h"
#include "graph.h"
#include <QString>

class Puzzle;

class Room: private Graph<Tile,int>
{
public:
    Room(size_t rows=1, size_t cols=1, int defaultConnection = Tile::CONNECTION_NORMAL);
    ~Room();
    size_t Rows() const;
    size_t Cols() const;
    size_t Size() const;
    QString Name;
    Tile& getTile(size_t row, size_t col);
    Tile& getTile(size_t index);
    void setTile(size_t index, size_t type);
    size_t ItemIndex(size_t row, size_t col);
    void Coordinates(size_t index, size_t& row, size_t &col);
    int& Connection(size_t from, size_t to);
    void setTileSet(TileSet* ptr);
    TileSet& getTileSet() const;
    std::vector<Puzzle*> Puzzles;
private:
    size_t numRows, numCols;
    TileSet* tileSet;
};

#endif // ROOM_H
