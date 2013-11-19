#ifndef GLSPRITE_H
#define GLSPRITE_H

#include <QtGui>

class GLSprite
{
public:
    GLSprite();
    GLSprite(const QString& fileName, int width, int height, int DefaultFrame);
    ~GLSprite();
    int Width() const;
    int Height() const;
    void Advance();
    void Default();
    void Draw(QPainter& painter, const QRect& dest);
    void setTile(int tile);
    void Load(const QString& fileName, int width, int height, int DefaultFrame);
protected:
    QPixmap TileSet;
    int spriteWidth, spriteHeight, curFrame, numFrames, defaultFrame, curTile;
};

#endif // GLSPRITE_H
