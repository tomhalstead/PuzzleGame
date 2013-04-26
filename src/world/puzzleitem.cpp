#include "puzzleitem.h"
#include <QDebug>

Action::Action(): Type(0),Room(0), Row(0), Col(0){}
PuzzleItem::PuzzleItem(int required, bool initial): Required(required), SetTile(0),ClearTile(0), value(initial),init(initial){
    Reset();
}

bool PuzzleItem::Set() {
    value = true;
    if(Required == PuzzleItem::REQUIRED_CLEAR)
        solved = false;
    else
        solved = true;
    return solved;
}

bool PuzzleItem::Clear() {
    value = false;
    if(Required == PuzzleItem::REQUIRED_SET)
        solved = false;
    else
        solved = true;
    return solved;
}

bool PuzzleItem::Toggle() {
    value = !value;
    if( (Required == PuzzleItem::REQUIRED_SET && !value) || (Required == PuzzleItem::REQUIRED_CLEAR && value))
        solved = false;
    else
        solved = true;
    return solved;
}

void PuzzleItem::Reset()
{
    if(init)
        Set();
    else
        Clear();
}

bool PuzzleItem::Solved() const{
    return solved;
}

bool PuzzleItem::Status() const
{
    return value;
}
