#ifndef WORLD_H
#define WORLD_H

#include "map.h"
#include <QString>
#include <QFile>
#include <QtXml>

enum WORLD_ERRORS { WE_IO_ERROR, WE_INVALID_INDEX, WE_INVALID_FILE };

struct StartInfo {
    size_t MapIndex;
    size_t RoomIndex;
    size_t Row;
    size_t Col;
};


class World {
public:
    World();
    World(const QString& fileName);
    Map& getMap(size_t index);
    StartInfo getStartInfo() const;
    void Load(const QString& fileName);
private:
    void getStartInfo(const QDomElement& startElement);
    void loadTileSet(const QDomElement& startElement);
    void loadTile(const QDomElement& startElement, int tileSet);
    void loadMap(const QDomElement& startElement);
    void loadRoom(const QDomElement& startElement, int map);
    void loadLocation(const QDomElement& startElement, Map &map, Room &room, size_t roomIndex, size_t tileIndex);
    size_t numTileSets;
    std::vector<Map> maps;
    std::vector<TileInfo>* tileSets;
    StartInfo si;
};

#endif // WORLD_H
