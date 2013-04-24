#ifndef LINK_H
#define LINK_H
#include <cstdlib>

struct Link {
    Link(size_t room = 0, size_t row = 0, size_t col=0);
    size_t Room;
    size_t Puzzle;
    size_t Item;
    size_t Row;
    size_t Col;
};

#endif // LINK_H
