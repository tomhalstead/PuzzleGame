#include "items.h"

Item::Item(int id): ID(id), Type(0), Data(0), Count(0) {}

bool Item::operator<(const Item& RHS) {
    bool result = false;
    if(Type < RHS.Type)
        result = true;
    else if(Type == RHS.Type && Name < RHS.Name)
        result = true;
    return result;
}

bool Item::operator>(const Item& RHS) {
    bool result = false;
    if(Type > RHS.Type)
        result = true;
    else if(Type == RHS.Type && Name > RHS.Name)
        result = true;
    return result;
}

bool Item::operator ==(const Item &RHS)
{
    return (ID == RHS.ID);
}

bool Item::operator !=(const Item &RHS)
{
    return (ID != RHS.ID);
}
