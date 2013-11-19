#ifndef PLAYER_H
#define PLAYER_H

#include <cstdlib>
#include <QtGui>
#include "worldinfo.h"
#include "glsprite.h"
#include "items.h"

class Player {
public:
    Player();
    Player(const StartInfo& si);
    size_t Room;
    size_t X;
    size_t Y;
    int Facing;
    int Sprite;
    int Health;
    Item Armor;
    Item Weapon;
    size_t numItems() const;
    int getItemQuantity(const Item &find);
    void addItem(Item item);
    Item& getItem(size_t index);
private:
    std::vector<Item> inventory;
};

#endif // PLAYER_H
