#include "puzzle.h"

Puzzle::Puzzle(std::vector<Room *> *Rooms, size_t ParentIndex):  solved(false), rooms(Rooms), parentIndex(ParentIndex)
{
    if(!rooms)
        throw PE_NULL_ROOM;
    if(parentIndex > rooms->size())
        throw PE_INVALID_ROOM;
    parent = rooms->at(parentIndex);
    items.resize(parent->Size() + 1,NULL);
}

void Puzzle::Activate(size_t row, size_t col)
{
    size_t index = parent->ItemIndex(row,col);
    PuzzleItem* p= items[index];
    if(p) {
        unsigned int i = 0;
        if(perform(index) && !solved) {
            solved = true;
            while(solved && i < items.size()) {
                if(items[i])
                    solved &= items[i]->Solved();
                i++;
            }
            if(solved && items[items.size()-1])
                perform(items.size()-1);
        }
    }
}

PuzzleItem* &Puzzle::getItem(size_t row, size_t col)
{
    return items[parent->ItemIndex(row,col)];
}

PuzzleItem *&Puzzle::getItem(size_t index)
{
    return items[index];
}

size_t Puzzle::Size() const
{
    return items.size();
}

bool Puzzle::Solved()
{
    return solved;
}

bool Puzzle::perform(size_t index)
{
    PuzzleItem* p= items[index];
    size_t destIndex;
    for(unsigned int i = 0; i < p->Actions.size(); i++) {
        Action* a = &p->Actions[i];
        Room* target = (rooms->at(a->Room));
        switch(a->Type) {
        case Action::SET:
            return target->Puzzles[a->Index]->getItem(a->Row,a->Col)->Set();
        case Action::CLEAR:
            return target->Puzzles[a->Index]->getItem(a->Row,a->Col)->Clear();
        case Action::TOGGLE:
            return target->Puzzles[a->Index]->getItem(a->Row,a->Col)->Toggle();
        case Action::TILE:
            destIndex = target->ItemIndex(a->Row,a->Col);
            if(target->getTile(destIndex).Type != a->Copy->Type) {
                target->setTile(destIndex,a->Copy->Type);
                if(!(a->Copy->Data & Tile::DATA_NOCOPY))
                    target->getTile(destIndex).Data = a->Copy->Data;
                target->getTile(destIndex).RoomLink = a->Copy->RoomLink;
            }
            return true;
        default:
            break;
        }
    }
    return false;
}


