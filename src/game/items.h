#ifndef ITEMS_H
#define ITEMS_H

#include <QString>

class Item {
public:
    Item(int id = -1);
    int ID;
    int Type;
    int Data;
    int Count;
    QString Name;
    QString Description;
    static const int ITEM_WEAPON     = -3;
    static const int ITEM_ARMOR      = -2;
    static const int ITEM_UNIQUE     = -1;
    static const int ITEM_CONSUMABLE = 0;
    bool operator<(const Item& RHS);
    bool operator>(const Item& RHS);
    bool operator==(const Item& RHS);
    bool operator!=(const Item& RHS);
};

#endif // ITEMS_H
