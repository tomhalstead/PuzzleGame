#include "world.h"
#include <iostream>
using namespace std;

World::World(): numTileSets(0), tileSets(NULL) {}

World::World(const QString &fileName): numTileSets(0), tileSets(NULL)
{
    Load(fileName);
}

Map &World::getMap(size_t index)
{
    if(index < maps.size())
        return maps[index];
    else {
        cout << endl << "Invalid index: " << index << ", size: " << maps.size() << endl;
        throw WE_INVALID_INDEX;
    }
}

StartInfo World::getStartInfo() const
{
    return si;
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
    cout << "Found " << child.childNodes().count() << " start nodes." << endl;
    getStartInfo(child);
    child = root.firstChildElement("tilesets");
    if(!child.isNull()) {
        numTileSets = child.childNodes().count();
        tileSets = new std::vector<TileInfo>[numTileSets];
        cout << "Found " << numTileSets << " tilesets." << endl;
        for(QDomElement subChild = child.firstChildElement(); !subChild.isNull(); subChild = subChild.nextSiblingElement())
            loadTileSet(subChild);
    }
    cout << "Check for maps: ";
    child = root.firstChildElement("maps");
    if(child.isNull())
        throw WE_INVALID_FILE;
    maps.resize(child.childNodes().count());
    cout << "Found " << maps.size() << " maps, load: " << endl;
    for(QDomElement subChild = child.firstChildElement(); !subChild.isNull(); subChild = subChild.nextSiblingElement())
        loadMap(subChild);
    system("pause");
}

void World::getStartInfo(const QDomElement &startElement)
{
    si.MapIndex = startElement.firstChildElement("map").text().toUInt();
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
    int curTile = startElement.attribute("id").toInt();
    tileSets[tileSet][curTile].Draw = startElement.attribute("draw").toInt();
    QDomElement tile = startElement.firstChildElement();
    if(!tile.isNull()) {
        if(!tile.firstChildElement("North").isNull())
            tileSets[tileSet][curTile].Collision |= TileInfo::COLLIDE_NORTH;
        if(!tile.firstChildElement("East").isNull())
            tileSets[tileSet][curTile].Collision |= TileInfo::COLLIDE_EAST;
        if(!tile.firstChildElement("West").isNull())
            tileSets[tileSet][curTile].Collision |= TileInfo::COLLIDE_WEST;
        if(!tile.firstChildElement("South").isNull())
            tileSets[tileSet][curTile].Collision |= TileInfo::COLLIDE_SOUTH;
    }
    cout << "Loaded tile: " << curTile << ", Draw: " << tileSets[tileSet][curTile].Draw << ", Collision: " << int(tileSets[tileSet][curTile].Collision) << endl;
}

void World::loadMap(const QDomElement &startElement)
{
    int curMap = startElement.attribute("id").toInt();
    maps[curMap] = Map(startElement.childNodes().count());
    maps[curMap].Name = startElement.attribute("name").toStdString();
    for(QDomElement child = startElement.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
        loadRoom(child,curMap);
}

void World::loadRoom(const QDomElement &startElement, int map)
{
    size_t curRoom = startElement.attribute("id").toUInt();
    size_t tileSet = startElement.attribute("tileSet").toUInt();
    cout << "\tLoading room: " << curRoom << endl;
    int height,width, index=0;
    QDomElement child, location;
    cout << "\tLoad height: ";
    child = startElement.firstChildElement("height");
    if(child.isNull())
        throw WE_INVALID_FILE;
    height = child.text().toUInt();
    cout << height << endl << "\tLoad Width: ";
    child = startElement.firstChildElement("width");
    if(child.isNull())
        throw WE_INVALID_FILE;
    width = child.text().toUInt();
    cout << width <<endl;
    Room room(height,width);
    room.Name = startElement.attribute("name").toStdString();
    if(tileSet >= numTileSets)
        throw WE_INVALID_FILE;
    room.TileSet = tileSets + tileSet;
    child = child.nextSiblingElement("locations");
    if(child.isNull() || child.childNodes().count() != height * width)
        throw WE_INVALID_FILE;
    location = child.firstChildElement();
    while(!location.isNull())  {
        loadLocation(location,maps[map],room,curRoom,index);
        location = location.nextSiblingElement();
        index++;
    }
}

void World::loadLocation(const QDomElement &startElement,Map& map, Room &room, size_t roomIndex, size_t tileIndex)
{
    cout << "\t\tLoading tile " << tileIndex;
    TileInfo curTile = (*room.TileSet).at(startElement.attribute("type").toUInt());
    room.getTile(tileIndex).Data = startElement.attribute("data").toInt();
    size_t row, col;
    room.Coordinates(tileIndex,row,col);
    cout << " (" << row << ", " << col << ")" << endl;
    if(curTile.Collision & TileInfo::COLLIDE_EAST)
        if(col < room.Cols()-1 )
            room.Connected(room.ItemIndex(row,col+1), tileIndex) = Tile::CONNECTION_NONE;
    if(curTile.Collision & TileInfo::COLLIDE_WEST)
        if(col)
            room.Connected(room.ItemIndex(row,col-1), tileIndex) = Tile::CONNECTION_NONE;
    if(curTile.Collision & TileInfo::COLLIDE_NORTH)
        if(row)
            room.Connected(room.ItemIndex(row-1,col), tileIndex) = Tile::CONNECTION_NONE;
    if(curTile.Collision & TileInfo::COLLIDE_SOUTH)
        if(row < room.Rows()-1)
            room.Connected(room.ItemIndex(row+1,col), tileIndex) = Tile::CONNECTION_NONE;
    for(QDomElement child = startElement.firstChildElement(); !child.isNull(); child = child.nextSiblingElement()) {
        Link link(true,child.attribute("row").toUInt(),child.attribute("col").toUInt());
        map.getLink(roomIndex,child.attribute("id").toUInt()) = link;
    }
}

