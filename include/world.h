#ifndef WORLD_H
#define WORLD_H

#include "room.h"
#include "worldinfo.h"
#include <QString>
#include <QFile>
#include <QtXml>

enum WORLD_ERROR { WE_IO_ERROR, WE_INVALID_INDEX, WE_INVALID_FILE };

struct StartInfo;
class  WorldInfo;

class World {
public:
    World();
    World(const QString& fileName);
    Room& getRoom(size_t index);
    size_t Size() const;
    void Load(const QString& fileName);
    void Load(const QDomElement& root);
    WorldInfo Info;
private:
    void loadInfo(const QDomElement& startElement);
    void getStartInfo(const QDomElement& startElement);
    void loadTileSet(const QDomElement& startElement);
    void loadTile(const QDomElement& startElement, int tileSet);
    void loadRoom(const QDomElement& startElement);
    void loadLocation(const QDomElement& startElement, size_t roomIndex, size_t tileIndex);
    void loadPuzzle(const QDomElement& startElement, size_t roomIndex);
    void loadActions(const QDomElement &startElement, size_t roomIndex, size_t puzzleIndex, size_t tileIndex);
    size_t numTileSets;
    std::vector<Room*> rooms;
    std::vector<TileInfo>* tileSets;
};

#endif // WORLD_H
