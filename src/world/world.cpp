#include "world.h"
#include <QDebug>

World::World(): numTileSets(0), tileSets(NULL) {}

World::World(const QString &fileName): numTileSets(0), tileSets(NULL)
{
    Load(fileName);
}

Room &World::getRoom(size_t index)
{
    if(index < rooms.size())
        return *rooms[index];
    else
        throw WE_INVALID_INDEX;
}

size_t World::Size() const
{
    return rooms.size();
}

void World::Load(const QString &fileName)
{
    QFile f(fileName);
    QDomDocument d;
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        throw WE_IO_ERROR;
    if(!d.setContent(&f)) {
        f.close();
        throw WE_INVALID_FILE;
    }
    Load(d.firstChildElement());
}

void World::Load(const QDomElement &root)
{
    QDomElement child;
    loadInfo(root);
    if(!Info.Start.size())
        throw WE_INVALID_FILE;

    child = root.firstChildElement("tilesets");
    if(!child.isNull()) {
        numTileSets = child.childNodes().count();
        tileSets = new std::vector<TileInfo>[numTileSets];
        for(QDomElement subChild = child.firstChildElement();
            !subChild.isNull(); subChild = subChild.nextSiblingElement())
                loadTileSet(subChild);
    }
    child = root.firstChildElement("map");
    if(child.isNull())
        throw WE_INVALID_FILE;
    rooms.resize(child.childNodes().count(),NULL);
    for(QDomElement subChild = child.firstChildElement();
        !subChild.isNull();
        subChild = subChild.nextSiblingElement())
        loadRoom(subChild);
}

void World::loadInfo(const QDomElement &startElement)
{
    getStartInfo(startElement);
    QDomElement child = startElement.firstChildElement("info");
    child = child.firstChildElement("intro");
    if(child.hasAttribute("image"))
        Info.IntroImage->load(child.attribute("image"));
    Info.IntroText = child.text();
    child = child.nextSiblingElement("victory");
    if(child.hasAttribute("image"))
        Info.VictoryImage->load(child.attribute("image"));
    Info.VictoryText = child.text();
}

void World::getStartInfo(const QDomElement &startElement)
{
    size_t index = 0;
    for(QDomElement child = startElement.firstChildElement("start");
        !child.isNull(); child = child.nextSiblingElement("start"))
    {
        Info.Start.push_back(StartInfo());
        Info.Start[index].RoomIndex = child.attribute("room").toUInt();
        Info.Start[index].Row = child.attribute("row").toUInt();
        Info.Start[index].Col = child.attribute("col").toUInt();
        index++;
    }
}

void World::loadTileSet(const QDomElement &startElement)
{
    int curTileSet = startElement.attribute("id").toInt();
    tileSets[curTileSet].resize(startElement.childNodes().count());
    for(QDomElement tile = startElement.firstChildElement(); !tile.isNull(); tile = tile.nextSiblingElement())
        loadTile(tile,curTileSet);
}

void World::loadTile(const QDomElement &startElement, int tileSet)
{
    TileInfo* curTile = &tileSets[tileSet][startElement.attribute("id").toUInt()];
    curTile->Draw = startElement.attribute("draw").toInt();
    if(startElement.hasAttribute("puzzle")) {
        qDebug() << "Puzzle collision tile";
        curTile->Collision |= TileInfo::COLLIDE_PUZZLE;
        curTile->Index = startElement.attribute("puzzle").toUInt();
    }
    if(startElement.hasAttribute("item")) {
        curTile->Collision |= TileInfo::COLLIDE_ITEM;
        curTile->Index = startElement.attribute("id").toInt();
    }
    if(startElement.hasChildNodes()) {
        if(!startElement.firstChildElement("All").isNull())
            curTile->Collision |= TileInfo::COLLIDE_ALL;
        else {
            if(!startElement.firstChildElement("North").isNull())
                curTile->Collision |= TileInfo::COLLIDE_NORTH;
            if(!startElement.firstChildElement("East").isNull())
                curTile->Collision |= TileInfo::COLLIDE_EAST;
            if(!startElement.firstChildElement("West").isNull())
                curTile->Collision |= TileInfo::COLLIDE_WEST;
            if(!startElement.firstChildElement("South").isNull())
                curTile->Collision |= TileInfo::COLLIDE_SOUTH;
        }
    }
}

void World::loadRoom(const QDomElement &startElement)
{
    size_t curRoom = startElement.attribute("id").toUInt();
    size_t tileSet = startElement.attribute("tileSet").toUInt();
    size_t rows = startElement.attribute("rows").toUInt();
    size_t cols = startElement.attribute("cols").toUInt();
    QDomElement child;
    size_t index=0;
    Room* room = new Room(rows,cols);
    rooms[curRoom] = room;
    room->Name = startElement.attribute("name");
    if(tileSet >= numTileSets)
        throw WE_INVALID_FILE;
    room->setTileSet(tileSet, tileSets + tileSet);
    child = startElement.firstChildElement("locations");
    if(child.isNull() || (size_t)child.childNodes().count() != rows * cols)
        throw WE_INVALID_FILE;
    for(child = child.firstChildElement();
        !child.isNull(); child = child.nextSiblingElement())
            loadLocation(child,curRoom,index++);
    child = startElement.firstChildElement("puzzles");
    room->Puzzles.resize(child.childNodes().count(),NULL);
    for(child = child.firstChildElement();
        !child.isNull(); child = child.nextSiblingElement() )
            loadPuzzle(child,curRoom);
}

void World::loadLocation(const QDomElement &startElement,size_t roomIndex, size_t tileIndex)
{
    Room* room = rooms[roomIndex];
    Tile* tile = &room->getTile(tileIndex);
    size_t row, col;
    room->Coordinates(tileIndex,row,col);
    tile->Data = startElement.attribute("data").toInt();
    room->setTile(tileIndex,startElement.attribute("type").toUInt());
    QDomElement child = startElement.firstChildElement("link");
    if(!child.isNull()) {
        tile->RoomLink.Room = child.attribute("room").toUInt();
        tile->RoomLink.Row = child.attribute("row").toUInt();
        tile->RoomLink.Col = child.attribute("col").toUInt();
    }
}

void World::loadPuzzle(const QDomElement &startElement, size_t roomIndex)
{
    Puzzle* p = new Puzzle(&rooms,roomIndex);
    size_t curPuzzle = startElement.attribute("id").toUInt();
    size_t row,col;
    rooms[roomIndex]->Puzzles[curPuzzle] = p;
    p->TileSet = &tileSets[startElement.attribute("tileSet").toUInt()];
    for(QDomElement child = startElement.firstChildElement("pieces").firstChildElement();
        !child.isNull(); child = child.nextSiblingElement())
    {
        PuzzleItem* i;
        if(child.hasAttribute("required")) {
            if(child.attribute("required").toInt())
                i = new PuzzleItem(PuzzleItem::REQUIRED_SET,child.attribute("value").toInt()>0);
            else
                i = new PuzzleItem(PuzzleItem::REQUIRED_CLEAR,child.attribute("value").toInt()>0);
        }
        else
            i = new PuzzleItem(PuzzleItem::REQUIRED_NONE,child.attribute("value").toInt()>0);
        i->ClearTile = child.attribute("type").toUInt();
        if(child.hasAttribute("setType"))
            i->SetTile = child.attribute("setType").toUInt();

        row = child.attribute("row").toUInt();
        col = child.attribute("col").toUInt();
        p->getItem(row,col) = i;
        loadActions(child,roomIndex,curPuzzle,rooms[roomIndex]->ItemIndex(row,col));
    }
    p->getItem(p->Size()-1) = new PuzzleItem();
    loadActions(startElement.firstChildElement("actions"),roomIndex,curPuzzle,rooms[roomIndex]->Size());
}

void World::loadActions(const QDomElement &startElement, size_t roomIndex, size_t puzzleIndex, size_t tileIndex)
{
    size_t row, col;
    bool isAction;
    rooms[roomIndex]->Coordinates(tileIndex,row,col);
    for(QDomElement action = startElement.firstChildElement(); !action.isNull(); action = action.nextSiblingElement()) {
        isAction = false;
        Action a;
        if(action.tagName().toLower()=="set") {
            a.Type = Action::SET;
            isAction = true;
        }
        else if(action.tagName().toLower()=="clear") {
            a.Type = Action::CLEAR;
            isAction = true;
        }
        else if(action.tagName().toLower()=="toggle") {
            a.Type = Action::TOGGLE;
            isAction = true;
        }
        else if(action.tagName().toLower()=="tile") {
            a.Type = Action::TILE;
            a.Copy = new Tile();
            a.Copy->Type = action.attribute("type").toUInt();
            a.Copy->Data = action.hasAttribute("data") ?  action.attribute("data").toInt() : Tile::DATA_NOCOPY;
            QDomElement link;
            if( !( link = action.firstChildElement("link")).isNull() ) {
                a.Copy->RoomLink.Room = link.attribute("room").toUInt();
                a.Copy->RoomLink.Row = link.attribute("row").toUInt();
                a.Copy->RoomLink.Col = link.attribute("col").toUInt();
            }
            isAction = true;
        }
        if(isAction) {
            a.Room = action.hasAttribute("room") ? action.attribute("room").toUInt() : roomIndex;
            a.Index = action.hasAttribute("id") ? action.attribute("id").toUInt(): puzzleIndex;
            a.Row = action.hasAttribute("row") ? action.attribute("row").toUInt() : row;
            a.Col = action.hasAttribute("col") ? action.attribute("col").toUInt() : col;
            rooms[roomIndex]->Puzzles[puzzleIndex]->getItem(tileIndex)->Actions.push_back(a);
        }
    }
}

