#ifndef PUZZLEITEM_H
#define PUZZLEITEM_H

#include "room.h"
#include <vector>

struct Tile;

struct Action {
    Action();
    unsigned int Type;
    size_t Room;
    size_t Index;
    size_t Row;
    size_t Col;
    Tile*  Copy;

    static const unsigned int SET    = 1;
    static const unsigned int CLEAR  = 2;
    static const unsigned int TOGGLE = 3;
    static const unsigned int TILE   = 4;
};

class PuzzleItem {
public:
    PuzzleItem(int required = REQUIRED_NONE, bool initial = false);
    int Required;
    unsigned int SetTile;
    unsigned int ClearTile;
    std::vector<Action> Actions;
    bool Set();
    bool Clear();
    bool Toggle();
    void Reset();
    bool Solved() const;
    bool Status() const;
    static const int REQUIRED_NONE  = 0;
    static const int REQUIRED_SET   = 1;
    static const int REQUIRED_CLEAR = 2;
private:
    bool value, solved, init;
};

#endif // PUZZLEITEM_H
