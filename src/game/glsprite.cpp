#include "glsprite.h"

GLSprite::GLSprite(): spriteWidth(0), spriteHeight(0), curFrame(0), numFrames(0), defaultFrame(0), curTile(0){}

GLSprite::GLSprite(const QString &fileName, int width, int height, int DefaultFrame): curTile(0)
{
    qDebug() << "Load sprite: " << fileName;
    Load(fileName,width,height,DefaultFrame);
}

GLSprite::~GLSprite()
{
}

int GLSprite::Width() const
{
    return spriteWidth;
}

int GLSprite::Height() const
{
    return spriteHeight;
}

void GLSprite::Advance()
{
    curFrame++;
    if(curFrame == numFrames)
        curFrame = 0;
}

void GLSprite::Default()
{
    curFrame = defaultFrame;
}

void GLSprite::Draw(QPainter &painter, const QRect &dest)
{
    QRect source(curFrame * spriteWidth, curTile * spriteHeight, spriteWidth, spriteHeight);
    painter.drawPixmap(dest,TileSet,source);
}

void GLSprite::setTile(int tile)
{
    curTile = tile;
}

void GLSprite::Load(const QString &fileName, int width, int height, int DefaultFrame)
{
    if(TileSet.load(fileName))
        qDebug() << "Player sprite loaded successfully";
    else
        qDebug() << "Player sprite load failure";
    spriteHeight = height;
    spriteWidth = width;
    numFrames = TileSet.width() / spriteWidth;
    curFrame = defaultFrame = DefaultFrame;

    qDebug() << "Sprite size: " << spriteWidth << "x" << spriteHeight << " image size:" << TileSet.width() << "x" << TileSet.height() << ", numFrames:" << numFrames << ", curFrame:" << curFrame;
}
