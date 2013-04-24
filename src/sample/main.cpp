#include <iostream>
#include "world.h"
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;

void draw(Room& room, size_t* player);
void draw(Room& room, size_t* player, size_t row, size_t col);

int main()
{
    srand(time(NULL));
    World w;
    try {
        w.Load("default.map");
    }
    catch(WORLD_ERROR e) {
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
    catch(PUZZLE_ERROR e) {
        cout << "PUZZLE ERROR" << endl;
    }
    catch(MATRIX_ERROR e) {
        cout << "MATRIX ERROR" << endl;
    }
    catch(...) {
        cout << "OTHER ERROR" << endl;
        exit(1);
    }
    StartInfo si = w.getStartInfo();
    size_t index = si.RoomIndex;
    Room* room = &w.getRoom(index);
    size_t player[2];
    player[0] = si.Row;
    player[1] = si.Col;
    char ans;
    int move[2];
    int connection, check, curPlayer;
    do {
        system("cls");
        move[0] = 0;
        move[1] = 0;
        cout << "Find the V, use WASD to move." << endl;
        room = &w.getRoom(index);
        draw(*room,player);
        cout << "? ";
        cin >> ans;
        curPlayer = room->ItemIndex(player[0],player[1]);
        switch(toupper(ans)) {
        case 'W':
            if(player[0]) {
                check = room->ItemIndex(player[0]-1,player[1]);
                connection = room->Connection(curPlayer,check);
                if(connection) {
                    move[0] = -1;
                    if((connection & Tile::CONNECTION_PUZZLE) && !room->Puzzles.at(room->getTile(check).RoomLink.Puzzle)->Solved() )
                        move[0] = 0;
                }
            }
            break;
        case 'A':
            if(player[1]) {
                check = room->ItemIndex(player[0],player[1]-1);
                connection = room->Connection(curPlayer,check);
                if(connection) {
                    move[1] = -1;
                    if((connection & Tile::CONNECTION_PUZZLE)) {
                        if(!room->Puzzles.at(room->getTile(check).RoomLink.Puzzle)->Solved() )
                            move[1] = 0;
                    }
                }
            }
            break;
        case 'S':
            if(player[0]<room->Rows()-1) {
                check = room->ItemIndex(player[0]+1,player[1]);
                connection = room->Connection(curPlayer,check);
                if(connection) {
                    move[0] = 1;
                    if((connection & Tile::CONNECTION_PUZZLE) && !room->Puzzles.at(room->getTile(check).RoomLink.Puzzle)->Solved() )
                        move[0] = 0;
                }
            }
            break;
        case 'D':
            if(player[1]<room->Cols()-1) {
                check = room->ItemIndex(player[0],player[1]+1);
                connection = room->Connection(curPlayer,check);
                if(connection) {
                    move[1] = 1;
                    if((connection & Tile::CONNECTION_PUZZLE) && !room->Puzzles.at(room->getTile(check).RoomLink.Puzzle)->Solved() )
                        move[1] = 0;
                }
            }
            break;
        case '0':
            break;
        default:
            cout << "Invalid option." << endl;
        }
        player[0] += move[0];
        player[1] += move[1];
        Tile check = room->getTile(player[0],player[1]);
        Link link;
        for(unsigned int i = 0; i < room->Puzzles.size(); i++)
            room->Puzzles.at(i)->Activate(player[0],player[1]);
        switch(check.Data) {
        case Tile::DATA_ROOMEDGE:
            link = check.RoomLink;
            index = link.Room;
            player[0] = link.Row;
            player[1] = link.Col;
            room = &w.getRoom(index);
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



void draw(Room& room, size_t* player) {
    for(size_t i = 0; i < room.Rows(); i++) {
        for(size_t j = 0; j < room.Cols(); j++)
            draw(room,player,i,j);
        cout << endl;
    }
}

void draw(Room &room, size_t* player, size_t row, size_t col) {
    if(player[0] == row && player[1] == col)
        cout << setw(4) << "P";
    else {
        char out;
        std::vector<TileInfo>* info;
        PuzzleItem* p;
        out = char(room.getTile(row,col).Draw);
        for(unsigned int i = 0; i < room.Puzzles.size(); i++) {
            p = room.Puzzles.at(i)->getItem(row,col);
            info = room.Puzzles.at(i)->TileSet;
            if(p)
                out = (p->Status()) ? char(info->at(p->SetTile).Draw) : char(info->at(p->ClearTile).Draw);
        }
        cout << setw(4) << out;
    }
}
