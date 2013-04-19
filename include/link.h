#ifndef LINK_H
#define LINK_H
#include <cstdlib>

struct Link {
    Link(bool value = false, size_t row = 0, size_t col = 0);
    void SetLink(size_t row, size_t col);
    void RemoveLink();
    bool Linked;
    size_t Row;
    size_t Col;
};

#endif // LINK_H
