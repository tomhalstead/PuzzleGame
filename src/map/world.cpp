#include "world.h"

World::World(): numTileSets(0), tileSets(NULL) {}

World::World(const QString &fileName): numTileSets(0), tileSets(NULL)
{
    Load(fileName);
}

StartInfo World::getStartInfo() const
{
    return si;
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
    QDomElement root = d.firstChildElement();
    QDomElement child = root.firstChildElement("start");
    if(child.isNull())
        throw WE_INVALID_FILE;
    getStartInfo(child);
    child = root.firstChildElement("tilesets");
    if(!child.isNull()) {
        numTileSets = child.childNodes().count();
        tileSets = new std::vector<TileInfo>[numTileSets];
        for(QDomElement subChild = child.firstChildElement(); !subChild.isNull(); subChild = subChild.nextSiblingElement())
            loadTileSet(subChild);
    }
    child = root.firstChildElement("map");
    if(child.isNull())
        throw WE_INVALID_FILE;
    rooms.resize(child.childNodes().count(),NULL);
    for(QDomElement subChild = child.firstChildElement(); !subChild.isNull(); subChild = subChild.nextSiblingElement())
        loadRoom(subChild);
}

void World::getStartInfo(const QDomElement &startElement)
{
    si.RoomIndex = startElement.firstChildElement("room").text().toUInt();
    si.Row = startElement.firstChildElement("row").text().toUInt();
    si.Col = startElement.firstChildElement("col").text().toUInt();
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
    if(startElement.hasChildNodes()) {
        QDomElement tile = startElement.firstChildElement("Puzzle");
        if(!tile.isNull()) {
            curTile->Collision |= TileInfo::COLLIDE_PUZZLE;
            curTile->Index = tile.attribute("id").toUInt();
        }
        tile = startElement.firstChildElement("Item");
        if(!tile.isNull()) {
            curTile->Collision |= TileInfo::COLLIDE_ITEM;
            curTile->Index = tile.attribute("id").toInt();
        }
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
    room->Name = startElement.attribute("name").toStdString();
    if(tileSet >= numTileSets)
        throw WE_INVALID_FILE;
    room->TileSet = tileSets + tileSet;
    child = startElement.firstChildElement("locations");
    if(child.isNull() || (size_t)child.childNodes().count() != rows * cols)
        throw WE_INVALID_FILE;
    for(child = child.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
        loadLocation(child,curRoom,index++);
    child = startElement.firstChildElement("puzzles");
    room->Puzzles.resize(child.childNodes().count(),NULL);
    for(child = child.firstChildElement(); !child.isNull();child = child.nextSiblingElement() )
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
        PuzzleItem* i = new PuzzleItem();
        i->ClearTile = child.attribute("type").toUInt();
        if(child.hasAttribute("setType"))
            i->SetTile = child.attribute("setType").toUInt();
        if(child.hasAttribute("required")) {
            if(child.attribute("required").toInt())
                i->Required = PuzzleItem::REQUIRED_SET;
            else
                i->Required = PuzzleItem::REQUIRED_CLEAR;
        }
        if(child.attribute("value").toInt())
            i->Set();
        else
            i->Clear();
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

