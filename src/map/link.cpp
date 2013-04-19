#include "link.h"

Link::Link(bool value, size_t row, size_t col): Linked(value), Row(row),Col(col){}

void Link::SetLink(size_t row, size_t col) {
    Linked = true;
    Row = row;
    Col = col;
}

void Link::RemoveLink() {
    Linked = false;
}
