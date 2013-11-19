#include "player.h"

Player::Player():Facing(2){}

Player::Player(const StartInfo& si):Facing(2) {
    X = si.Col;
    Y = si.Row;
    Room = si.RoomIndex;
}

size_t Player::numItems() const
{
    return inventory.size();
}

int Player::getItemQuantity(const Item& find)
{
    qDebug() << "Get item quantity (ID:" << find.ID << ")";
    if(inventory.empty()) {
        qDebug() << "Inventory empty.";
        return 0;
    }
    else
        qDebug() << "Search inventory.";
    int check, left = 0, right = inventory.size() ;
    do {
        check = (left + right) / 2;
        if(inventory[check] < find) {
            if(inventory[left]==find)
                check = left;
            else {
                left++;
                right = check-1;
            }
        }
        else if(inventory[check] > find) {
            if(inventory[right]==find)
                check = right;
            else {
                right--;
                left = check+1;
            }
        }
    }while(inventory[check] != find && left < right);
    if(inventory[check]== find) {
        qDebug() << "Has " << inventory[check].Count;
        return inventory[check].Count;
    }
    else {
        qDebug() << "Not found.";
        return 0;
    }
}

void Player::addItem(Item item)
{
    unsigned int i = 0;
    if(inventory.empty())
        inventory.push_back(item);
    else {
        while(inventory[i] < item)
            i++;
        if(i == inventory.size())
            inventory.push_back(item);
        else if(inventory[i]==item)
            inventory[i].Count += item.Count;
        else
            inventory.insert(inventory.begin()+i,item);
    }
}

Item &Player::getItem(size_t index)
{
    return inventory[index];
}
