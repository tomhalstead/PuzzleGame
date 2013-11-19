#include "gltileset.h"

GLTileSet::GLTileSet(const QString& fileName, int TileSize): tileSize(TileSize)
{
    loadTileSet(fileName);
}

void GLTileSet::loadTileSet(const QString &fileName)
{
    if(fileName.length()) {
        Tiles.load(fileName);
        tilesWide = Tiles.width() / tileSize;
        tilesHigh = Tiles.height() / tileSize;
        qDebug() << "Load tileset: " << fileName << ", " << tilesWide << "x" << tilesHigh;
    }
}

int GLTileSet::getTileSize() const
{
    return tileSize;
}

QRect GLTileSet::Tile(int index)
{
    QRect result;
    if(!Tiles.isNull() && index <= (Tiles.height() * Tiles.width() / tileSize) )
    {
        int row = index / tilesWide;
        int col = index % tilesWide;
        result.setLeft(col * tileSize);
        result.setTop(row * tileSize);
        result.setWidth(tileSize);
        result.setHeight(tileSize);
    }
    return result;
}

QPixmap* GLTileSet::TileImages()
{
    return &Tiles;
}
