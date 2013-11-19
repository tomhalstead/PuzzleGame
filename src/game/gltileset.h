#ifndef GLTILESET_H
#define GLTILESET_H

#include <QtGui>

class GLTileSet
{
public:
    GLTileSet(const QString& fileName = "", int TileSize=16);
    void loadTileSet(const QString& fileName);
    int getTileSize() const;
    QRect Tile(int index);
    QPixmap* TileImages();
private:
    QPixmap Tiles;
    int tileSize;
    int tilesWide, tilesHigh;
};

#endif // GLTILESET_H
