#ifndef PUZZLE_H
#define PUZZLE_H

#include "puzzleitem.h"
#include "tileinfo.h"
#include <vector>

enum PUZZLE_ERROR { PE_NULL_ROOM, PE_INVALID_ROOM, PE_INVALID_TILE };

class Room;
class PuzzleItem;

class Puzzle {
public:
    Puzzle(std::vector<Room*>* Rooms, size_t ParentIndex);
    ~Puzzle();
    void Activate(size_t row, size_t col);
    PuzzleItem* &getItem(size_t row, size_t col);
    PuzzleItem* &getItem(size_t index);
    size_t Size() const;
    bool Solved();
    void Reset();
    void setTileSet(TileSet* ptr);
    TileSet& getTileSet() const;
private:
    bool solved;
    bool perform(size_t index);
    std::vector<PuzzleItem*> items;
    std::vector<Room*>* rooms;
    size_t parentIndex;
    Room* parent;
    TileSet* tileSet;
};

#endif // PUZZLE_H
