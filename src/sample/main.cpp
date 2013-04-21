#include <iostream>
#include "world.h"
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;

void draw(Room& room, size_t* player, size_t size);
void draw(Room& room, size_t* player, size_t row, size_t col);
Map  generate();
Room* generate(int index);
void setTile(Room &room, size_t row, size_t col, int type);
unsigned int randType();

int main()
{
    srand(time(NULL));
    size_t size = 4;
    World w;
    try {
        w.Load("default.map");
    }
    catch(WORLD_ERRORS e) {
        switch(e) {
        case WE_IO_ERROR:
            cout << "IO ERROR" << endl;
            break;
        case WE_INVALID_FILE:
            cout << "INVALID FILE" << endl;
            break;
        case WE_INVALID_INDEX:
            cout << "INVALID MAP INDEX" << endl;
            break;
        }
        exit(1);
    }
    Map m = w.getMap(0);
    Room* room;
    try {
        for(int i = 0; i < 4; i++) {
            m.getRoom(i) = generate(i);
        }
    }

    catch(MATRIX_ERROR e) {
        switch(e) {
        case INDEX_OOB:
            cout << "Index OOB" << endl;
            break;
        default:
            cout << "Other Matrix Error" << endl;
        }
        exit(1);
    }
    catch(...) {
        cout << "Unspecified error" << endl;
    }
    size_t index = 0;
    size_t player[2] = {1,1};
    char ans;
    do {
        system("cls");
        cout << "Find the 2, use WASD to move." << endl;
        room = m.getRoom(index);
        draw(*room,player,size);
        cout << "? ";
        cin >> ans;
        switch(toupper(ans)) {
        case 'W':
            if(player[0])
                if(room->Connected(room->ItemIndex(player[0],player[1]),room->ItemIndex(player[0]-1,player[1])) == Tile::CONNECTION_NORMAL)
                    player[0]--;
            break;
        case 'A':
            if(player[1])
                if(room->Connected(room->ItemIndex(player[0],player[1]),room->ItemIndex(player[0],player[1]-1)) == Tile::CONNECTION_NORMAL)
                    player[1]--;
            break;
        case 'S':
            if(player[0]<size-1)
                if(room->Connected(room->ItemIndex(player[0],player[1]),room->ItemIndex(player[0]+1,player[1])) == Tile::CONNECTION_NORMAL)
                    player[0]++;
            break;
        case 'D':
            if(player[1]<size-1)
                if(room->Connected(room->ItemIndex(player[0],player[1]),room->ItemIndex(player[0],player[1]+1)) == Tile::CONNECTION_NORMAL)
                    player[1]++;
            break;
            break;
        case '0':
            break;
        default:
            cout << "Invalid option." << endl;
        }
        Tile check = room->getTile(player[0],player[1]);
        Link link;
        switch(check.Data) {
        case Tile::DATA_ROOMEDGE:
            link = m.getLink(index,check.Index);
            index = check.Index;
            if(link.Linked) {
                player[0] = link.Row;
                player[1] = link.Col;
                room = m.getRoom(index);
            }
            else {
                cout << "INVALID ROOM LINK DETECTED" << endl;
                ans = '0';
            }
            break;
        case Tile::DATA_VICTORY:
            cout << "VICTORY!" << endl;
            ans = '0';
            break;
        default:
            break;
        }
    }while(ans != '0');
    return 0;
}



void draw(Room& room, size_t* player, size_t size) {
    for(size_t i = 0; i < size; i++) {
        for(size_t j = 0; j < size; j++)
            draw(room,player,i,j);
        cout << endl;
    }
}

void draw(Room &room, size_t* player, size_t row, size_t col) {
    if(player[0] == row && player[1] == col)
        cout << setw(4) << "P";
    else
        cout << setw(4) << room.getTile(row,col).Type;
}

unsigned int randType() {
    unsigned int value = rand() % 100;
    if(value > 19)
        return 0;
    else if(value > 9)
        return 1; // Wall
    else
        return 2; // Other
}

void setTile(Room &room, size_t row, size_t col, int type) {
    room.getTile(row,col).Type = type;
    size_t index = room.ItemIndex(row,col);
    switch(type) {
    case 1:
        if(row > 0)
            room.Connected(room.ItemIndex(row-1,col),index) = Tile::CONNECTION_NONE;
        if(row < room.Rows()-1)
            room.Connected(room.ItemIndex(row+1,col),index) = Tile::CONNECTION_NONE;
        if(col > 0)
            room.Connected(room.ItemIndex(row,col-1),index) = Tile::CONNECTION_NONE;
        if(col < room.Cols()-1)
            room.Connected(room.ItemIndex(row,col+1),index) = Tile::CONNECTION_NONE;
        break;
    default:
        break;
    }
}

Room* generate(int index) {
    Room* result = new Room(4,4);
    for(size_t i = 0; i < 4; i++)
        for(size_t j = 0; j < 4; j++)
            setTile(*result,i,j,(!i || !j || i==3 || j==3 ));
    switch(index) {
    case 0:
        result->getTile(3,2).Type = 3;
        result->getTile(3,2).Data = Tile::DATA_ROOMEDGE;
        result->Connected(result->ItemIndex(2,2),result->ItemIndex(3,2)) = Tile::CONNECTION_NORMAL;
        result->getTile(3,2).Index = 1;
        break;
    case 1:
        result->getTile(0,2).Type = 3;
        result->getTile(0,2).Data = Tile::DATA_ROOMEDGE;
        result->Connected(result->ItemIndex(1,2),result->ItemIndex(0,2)) = Tile::CONNECTION_NORMAL;
        result->getTile(0,2).Index = 0;
        result->getTile(2,0).Type = 3;
        result->getTile(2,0).Data = Tile::DATA_ROOMEDGE;
        result->Connected(result->ItemIndex(2,1),result->ItemIndex(2,0)) = Tile::CONNECTION_NORMAL;
        result->getTile(2,0).Index = 2;
        result->Connected(result->ItemIndex(2,2),result->ItemIndex(2,3)) = Tile::CONNECTION_NORMAL;
        result->getTile(2,3).Type = 3;
        result->getTile(2,3).Data = Tile::DATA_ROOMEDGE;
        result->getTile(2,3).Index = 3;
        break;
    case 2:
        result->getTile(1,1).Type = 2;
        result->getTile(1,1).Data = Tile::DATA_VICTORY;
        result->Connected(result->ItemIndex(2,2),result->ItemIndex(2,3)) = Tile::CONNECTION_NORMAL;
        result->getTile(2,3).Type = 3;
        result->getTile(2,3).Data = Tile::DATA_ROOMEDGE;
        result->getTile(2,3).Index = 1;
        break;
    case 3:
        result->Connected(result->ItemIndex(2,1),result->ItemIndex(2,0)) = Tile::CONNECTION_NORMAL;
        result->getTile(2,0).Type = 3;
        result->getTile(2,0).Data = Tile::DATA_ROOMEDGE;
        result->getTile(2,0).Index = 1;
        break;
    default:
        break;
    }
    return result;
}

Map  generate() {
    Map result(4);
    result.getLink(0,1).SetLink(1,2);
    result.getLink(1,0).SetLink(2,2);
    result.getLink(1,2).SetLink(2,2);
    result.getLink(2,1).SetLink(2,1);
    result.getLink(1,3).SetLink(2,1);
    result.getLink(3,1).SetLink(2,2);
    return result;
}
